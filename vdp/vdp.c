// Copyright 2015 Juan Luis Álvarez Martínez
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http ://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "vdp.h"
#include "vdp_constants.h"
#include "vdp_render.h"
#include <string.h>
#include <assert.h>

struct vdp_s vdp;

//Select which renderer to use.
void vdp_get_pixels(void* fb){
    if (VDP_FLAG_M4){
        //Mode 4 'modes'
        if ((VDP_FLAG_M1) && (!VDP_FLAG_M2)){
            assert(0); //Invalid mode
        }
        else
            vdp_mode4_pixels(fb);
    }
    else{
        //Original TMS modes
        if      ((!VDP_FLAG_M1) && (!VDP_FLAG_M2) && (!VDP_FLAG_M3)) /*Mode 0*/
            vdp_mode0_pixels((uint8_t*)fb);
        else if ((VDP_FLAG_M1) && (!VDP_FLAG_M2) && (!VDP_FLAG_M3))  /*Mode 1 (TEXT)*/
            vdp_mode1_pixels((uint8_t*)fb);
        else if ((!VDP_FLAG_M1) && (VDP_FLAG_M2) && (!VDP_FLAG_M3))  /*Mode 2 (GRAPHIC 2)*/
            vdp_mode2_pixels((uint8_t*)fb);
        else if ((!VDP_FLAG_M1) && (!VDP_FLAG_M2) && (VDP_FLAG_M3))  /*Mode 3 (MULTICOLOR)*/
            vdp_mode3_pixels((uint8_t*)fb);
        //Undocumented modes
        else if ((!VDP_FLAG_M1) && (VDP_FLAG_M2) && (!VDP_FLAG_M3))  /*Mode 1+2*/
            vdp_mode12_pixels((uint8_t*)fb);
        else if ((VDP_FLAG_M1) && (!VDP_FLAG_M2) && (VDP_FLAG_M3))   /*Mode 1+3*/
            vdp_mode13_pixels((uint8_t*)fb);
        else if ((!VDP_FLAG_M1) && (VDP_FLAG_M2) && (VDP_FLAG_M3))   /*Mode 2+3*/
            vdp_mode23_pixels((uint8_t*)fb);
        else if ((VDP_FLAG_M1) && (VDP_FLAG_M2) && (VDP_FLAG_M3))    /*Mode 1+2+3*/
            vdp_mode123_pixels((uint8_t*)fb);
    }
}

void* vdp_get_cram(){
    return (void*)vdp.cram;
}

void vdp_control_write(){
	//First control word byte
	if (vdp.control_index == 0){
		vdp.control_word[0] = z80_data;
		++(vdp.control_index);
		return;
	}
	//Second control word byte
	assert(vdp.control_index == 1);
	vdp.control_word[1] = z80_data;
	vdp.control_index = 0;

	//Store the read/write mode
	vdp.control_mode = vdp.control_word[1] & VDP_CTRL_MASK;
	//Store the address
	vdp.address = (vdp.control_word[0] + (((uint16_t)vdp.control_word[1])<< 8)) & 0x3FFF;

	//Only register writes/VRAM reads do something on this part
	switch (vdp.control_mode){
	case VDP_CTRL_VRAM_READ:
		vdp.buffer = vdp.vram[vdp.address];
		break;
	case VDP_CTRL_REGISTER:
		vdp.regs[vdp.control_word[1] & 0x0F] = vdp.control_word[0];
		break;
	}
}

void vdp_control_read(){
    vdp.control_index = 0;
	z80_data = vdp.status;
}

void vdp_data_read(){
    vdp.control_index = 0;
    z80_data = vdp.buffer;
    vdp.address = (vdp.address + 1) % VDP_VRAM_SIZE;
    vdp.buffer = vdp.vram[vdp.address];
}

void vdp_data_write(){
    vdp.control_index = 0;
    vdp.buffer = z80_data;
    switch (vdp.control_mode){
    case VDP_CTRL_REGISTER:
    case VDP_CTRL_VRAM_READ:
    case VDP_CTRL_VRAM_WRITE:
        //Writes go to VRAM
        vdp.vram[vdp.address] = vdp.buffer;
        break;
    case VDP_CTRL_CRAM:
        //Writes go to CRAM
        vdp.cram[vdp.address % VDP_CRAM_SIZE] = vdp.buffer;
        break;
    default:
        assert(0); //Invalid mode
    }
    vdp.address = (vdp.address + 1) % VDP_VRAM_SIZE;
}

void vdp_init(){
    //Clear the VDP struct
    memset(&vdp, 0, sizeof(struct vdp_s));
    ///@bug set the real initial VDP state. If known.
}

void vdp_io(){
    //Check the IO ports (Address/data/IOreq)

    //If internal ioreq is active, we prevent a spurious IO
    //Reset if z80 ioreq is up
    if (vdp.ioreq_done){
        if (z80_n_ioreq)
            vdp.ioreq_done = 0;
        return;
    }

    //If not IO, just return
    if (z80_n_ioreq || (z80_n_rd && z80_n_wr))
        return;

    //Test bits 7,6 and 0
    switch (z80_address & ((3 << 6) | 1)){
    // --- If Address' bit 6 is set and 7 is not. Address is in range [0x40-0x7F]
    case ((1 << 6) | 1) : //Odd address H counter (read)
        if (z80_n_rd) //Only allow reads
            return;
        vdp.ioreq_done = 1; //<-- Set internal IO to prevent spurious IO
        z80_data = (vdp.h>>1) & 0xFF;
        return;
    case ((1 << 6) | 0) : //Even address V counter (read)
        if (z80_n_rd) //Only allow reads
            return;
        vdp.ioreq_done = 1; //<-- Set internal IO to prevent spurious IO
        z80_data = vdp.regs[VDP_REG_LINE_COUNTER];
        return;
    // --- If Address' bit 7 is set and 6 is not. Address is in range [0x80-0xBF]
    case ((1 << 7) | 1) : //Odd address is control port
		if (!z80_n_rd){ //Control port read
			vdp_control_read();
		}
		else if (!z80_n_wr){ //Control port writes
			vdp_control_write();
		}
        vdp.ioreq_done = 1; //<-- Set internal IO to prevent spurious IO
        return;
    case ((1 << 7) | 0) : //Even address is data port
		if (!z80_n_rd){ //Data port read
			vdp_data_read();
		}
		else if (!z80_n_wr){ //Data port writes
			vdp_data_write();
		}
        vdp.ioreq_done = 1; //<-- Set internal IO to prevent spurious IO
        return;
    // --- Otherwise, ignore.
    default:
        return;
    }
}

void vdp_update(){
    //Update H counter [0,256)
    vdp.h = (vdp.h + 1) % 256; 
    ///Update V counter @bug VCounter behaves differently depending on video mode.
    if (!(vdp.h)){
        ++(vdp.regs[VDP_REG_LINE_COUNTER]);
    }
}

void vdp_tick(){
    vdp_io(); //Perform read/writes if needed
    vdp_update();
}