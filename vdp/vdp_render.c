#include "vdp.h"
#include "vdp_render.h"
#include "vdp_constants.h"
#include <assert.h>

extern struct vdp_s vdp; //<-- Provided by vdp.c

// --- TMS Screen modes ---
//Fill the framebuffer as if the VDP is configured in Mode 0
//32x24 text mode, monochrome.
void vdp_mode0_pixels(uint8_t* fb){
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
        const uint8_t bg_color = (vdp.vram[color_addr + color_table_index] & 0xF0) >> 4;

        // --- Write the result ---
        const uint8_t pattern_row_byte = vdp.vram[generator_addr + (char_name * VDP_MODE0_PATTERN_SIZE) + pattern_row];
        assert(fg_color < 32);
        assert(bg_color < 32);
        if (pattern_row_byte & (0x80 >> pattern_col)){
            fb[x + (y * VDP_WIDTH_PIXELS)] = fg_color ? vdp.cram[fg_color] : vdp.cram[backdrop_color];
        }
        else{
            fb[x + (y * VDP_WIDTH_PIXELS)] = bg_color ? vdp.cram[bg_color] : vdp.cram[backdrop_color];
        }
    }
}

//Renders Mode 1 (TEXT)
void vdp_mode1_pixels(uint8_t* fb){
    assert(0); ///<-- @bug Unimplemented
}

//Renders Mode 2 (GRAPHICS 2)
void vdp_mode2_pixels(uint8_t* fb){
    assert(0); ///<-- @bug Unimplemented
}

//Renders Mode 3 (MULTICOLOR)
void vdp_mode3_pixels(uint8_t* fb){
    assert(0); ///<-- @bug Unimplemented
}

// --- SMS Mode 4 ---
//Renders Mode 4 (SMS custom mode)
void vdp_mode4_pixels(uint8_t* fb){
    assert(0); ///<-- @bug Unimplemented
}

// --- Undocumented modes ---
void vdp_mode12_pixels(uint8_t* fb){
    assert(0); ///<-- @bug unimplemented.
}

void vdp_mode13_pixels(uint8_t* fb){
    assert(0); ///<-- @bug unimplemented.
}

void vdp_mode23_pixels(uint8_t* fb){
    assert(0); ///<-- @bug unimplemented.
}

void vdp_mode123_pixels(uint8_t* fb){
    assert(0); ///<-- @bug unimplemented.
}