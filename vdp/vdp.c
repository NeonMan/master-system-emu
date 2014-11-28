#include "vdp.h"
#include "vdp_constants.h"
#include <string.h>
#include <assert.h>

struct vdp_s vdp;


//Fill the framebuffer as if the VDP is configured in Mode 0
//32x24 text mode, monochrome.
void* vdp_mode0_pixels(){
    assert(!VDP_FLAG_M1);
    assert(!VDP_FLAG_M2);
    assert(!VDP_FLAG_M3);
    //Pattern name table is 32x24 bytes, base address calculated from VDP_VDP_REG_NAME_TABLE_ADDR
    const uint16_t name_addr = vdp.regs[VDP_REG_NAME_TABLE_ADDR] * VDP_NAME_TABLE_SIZE;
    //Pattern generator table is 256 elements, 8 bytes per char. Base addr from VDP_REG_PATTERN_GENERATOR_ADDR
    const uint16_t generator_addr = vdp.regs[VDP_REG_PATTERN_GENERATOR_ADDR] * VDP_PATTERN_GENERATOR_TABLE_SIZE;
    //Color table colorizes characters in groups of 8. The table is 32 bytes long. Base address from VDP_REG_COLOR_TABLE_ADDR
    const uint16_t color_addr = vdp.regs[VDP_REG_COLOR_TABLE_ADDR] * VDP_COLOR_TABLE_SIZE;
    //Backdrop color
    const uint8_t backdrop_color = vdp.regs[VDP_REG_TEXT_COLOR] & 0x0F;

    //Start making the picture
//#pragma omp parallel for /*OpenMP *may* speed up things here*/
    for (unsigned int i = 0; i < VDP_FRAMEBUFFER_SIZE; i++){ //<-- For each pixel
        // -- Pixel x/y coordinates --
        const uint8_t x = i % VDP_WIDTH_PIXELS;
        const uint8_t y = i / VDP_WIDTH_PIXELS;

        // --- We neeed to calculate which char coresponds to this pixel ---
        //Every 8 pixel rows (y) is one character row.
        const uint8_t char_row = y / 8;
        //Every 8 pixel columns (x) is one character col
        const uint8_t char_col = x / 8;
        //Copy the name byte
        const uint8_t char_name = vdp.vram[name_addr + (char_row * VDP_MODE0_COLS) + char_col];

        // --- Which of the character pattern rows [0-7] ---
        const uint8_t pattern_row = y % 8;

        // --- Which bit of the row ---
        const uint8_t pattern_col = x % 8;

        // --- Which foreground/background color has that pixel ---
        //Each color table element paints 4 characters
        const uint8_t color_table_index = char_name / 8;
        assert(color_table_index < VDP_COLOR_TABLE_SIZE);
        const uint8_t fg_color = vdp.vram[color_addr + color_table_index] & 0x0F;
        const uint8_t bg_color = (vdp.vram[color_addr + color_table_index] & 0xF0)>>4;

        // --- Write the result ---
        const uint8_t pattern_row_byte = vdp.vram[generator_addr + (char_name * VDP_MODE0_PATTERN_SIZE) + pattern_row];
        assert(fg_color < 32);
        assert(bg_color < 32);
        if (pattern_row_byte & (1 << pattern_col)){
            vdp.framebuffer[x + (y * VDP_WIDTH_PIXELS)] = fg_color ? vdp.cram[fg_color] : vdp.cram[backdrop_color];
        }
        else{
            vdp.framebuffer[x + (y * VDP_WIDTH_PIXELS)] = bg_color? vdp.cram[bg_color] : vdp.cram[backdrop_color];
        }
    }
    return (void*)vdp.framebuffer;
}

void* vdp_get_pixels(){
    return vdp_mode0_pixels();
}

void* vdp_get_pallette(){
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
    if (z80_n_ioreq)
        return;

    //Test bits 7,6 and 0
    switch (z80_address & ((3 << 6) | 1)){
    // --- If Address' bit 6 is set and 7 is not. Address is in range [0x40-0x7F]
    case ((1 << 6) | 1) : //Odd address H counter (read)
        if (z80_n_rd) //Only allow reads
            return;
        vdp.ioreq_done = 1; //<-- Set internal IO to prevent spurious IO
        z80_data = vdp.h;
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

void vdp_tick(){
    vdp_io(); //Perform read/writes if needed
}