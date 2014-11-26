#include "vdp.h"
#include "vdp_constants.h"
#include <string.h>
#include <assert.h>

struct vdp_s vdp;

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
	vdp.address = vdp.control_word[0] + (((uint16_t)vdp.control_word[1]) << 8);

	//Only register writes/VRAM reads do something on this part
	switch (vdp.control_mode){
	case VDP_CTRL_VRAM_READ:
		vdp.buffer = vdp.vram[vdp.address];
		break;
	case VDP_CTRL_REGISTER:
		vdp.regs[vdp.control_word[1] & 0x0F] = vdp.control_word[0];
		break;
	default:
	}
}

void vdp_control_read(){
	z80_data = vdp.status;
}

void vdp_data_read(){
	assert(0); //<-- Unimplemented
}

void vdp_data_write(){
	assert(0); //<-- Unimplemented
}

void vdp_init(){
    //Clear the VDP struct
    memset(&vdp, 0, sizeof(struct vdp_s));
    ///@bug set the real initial VDP state. If known.
}

vdp_pixels vdp_mode0_pixels(){

}

vdp_pixels vdp_get_pixels(){
    vdp_pixels rv = { 0, 0 };
    return rv;
}

void vdp_io(){
    //Check the IO ports (Address/data/IOreq)

    //If not IO, just return
    if (z80_n_ioreq)
        return;

    //Test bits 7,6 and 0
    switch (z80_address & ((3 << 6) | 1)){
    // --- If Address' bit 6 is set and 7 is not. Address is in range [0x40-0x7F]
    case ((1 << 6) | 1) : //Odd address H counter (read)
        if (z80_n_rd) //Only allow reads
            return;
        z80_data = vdp.h;
        return;
    case ((1 << 6) | 0) : //Even address V counter (read)
        if (z80_n_rd) //Only allow reads
            return;
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
        return;
    case ((1 << 7) | 0) : //Even address is data port
		if (!z80_n_rd){ //Data port read
			vdp_data_read();
		}
		else if (!z80_n_wr){ //Data port writes
			vdp_data_write();
		}
        return;
    // --- Otherwise, ignore.
    default:
        return;
    }
}

void vdp_tick(){
    vdp_io(); //Perform read/writes if needed
}