// Copyright 2015 Juan Luis Álvarez Martínez
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "vdp.h"
#include "vdp_render.h"
#include "vdp_constants.h"
#include "debug/sms_debug.h"

extern struct vdp_s vdp; //<-- Provided by vdp.c

// --- TMS Screen modes ---
//Fill the framebuffer as if the VDP is configured in Mode 0
//32x24 text mode, monochrome.
void vdp_mode0_pixels(uint8_t* fb){
    //Pattern name table is 32x24 bytes, base address calculated from VDP_VDP_REG_NAME_TABLE_ADDR
    const uint16_t name_addr = vdp.regs[VDP_REG_NAME_TABLE_ADDR] * VDP_NAME_TABLE_SIZE;
    //Pattern generator table is 256 elements, 8 bytes per char. Base addr from VDP_REG_PATTERN_GENERATOR_ADDR
    const uint16_t generator_addr = vdp.regs[VDP_REG_PATTERN_GENERATOR_ADDR] * VDP_PATTERN_GENERATOR_TABLE_SIZE;
    //Color table colorizes characters in groups of 8. The table is 32 bytes long. Base address from VDP_REG_COLOR_TABLE_ADDR
    const uint16_t color_addr = vdp.regs[VDP_REG_COLOR_TABLE_ADDR] * VDP_COLOR_TABLE_SIZE;
    //Backdrop color
    const uint8_t backdrop_color = vdp.regs[VDP_REG_TEXT_COLOR] & 0x0F;

    //Start making the picture
    int i; //<-- OpenMP for variable must be declared C89-style.
//#pragma omp parallel for private(i) /*OpenMP might speedup things*/
    for (i = 0; i < VDP_FRAMEBUFFER_SIZE; i++)
    {
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
//40x24 characters. 6x8 pixels per char. Name table 960 bytes.
//Color table not used. PC works as Mode0 but lower 2 bits are ignored.
void vdp_mode1_pixels(uint8_t* fb){
    //Pattern name table address
    const uint16_t name_addr = vdp.regs[VDP_REG_NAME_TABLE_ADDR] * VDP_NAME_TABLE_SIZE;
    //Pattern generator table
    const uint16_t generator_addr = vdp.regs[VDP_REG_PATTERN_GENERATOR_ADDR] * VDP_PATTERN_GENERATOR_TABLE_SIZE;
    //Backdrop color
    const uint8_t bg_color = vdp.regs[VDP_REG_TEXT_COLOR] & 0x0F;
    //Foreground color
    const uint8_t fg_color = (vdp.regs[VDP_REG_TEXT_COLOR] & 0xF0)>>4;
    //CRAM colors
    const uint8_t bg_cram_color = bg_color ? vdp.cram[bg_color] : 0;
    const uint8_t fg_cram_color = vdp.cram[fg_color];

    //Start making the picture
    int i;
//#pragma omp parallel for /*OpenMP *may* speed up things here*/
    for (i = 0; i < VDP_FRAMEBUFFER_SIZE; i++){ //<-- For each pixel
        //Calculate all the relevant info.
        const uint8_t x = i % VDP_WIDTH_PIXELS;
        const uint8_t y = i / VDP_WIDTH_PIXELS;
        const uint8_t char_row = y / 8; //<-- Each character uses 8 rows
        const uint8_t char_col = x / 6; //<-- Each character uses 6 columns
        const uint8_t char_name = vdp.vram[name_addr + (char_row * VDP_MODE1_COLS) + char_col];
        const uint8_t pattern_row = y % 8;
        const uint8_t pattern_col = x % 6;
        //Write the result.
        const uint8_t pattern_row_byte = vdp.vram[generator_addr + (char_name * VDP_MODE1_PATTERN_SIZE) + pattern_row];
        if (pattern_row_byte & (0x80 >> pattern_col)){
            fb[x + (y * VDP_WIDTH_PIXELS)] = fg_cram_color;
        }
        else{
            fb[x + (y * VDP_WIDTH_PIXELS)] = bg_cram_color;
        }
    }
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
    //Mode 4 repurposes some registers and ignores others
    //  r00: Mode Control 1
    //  r01: Mode Control 2
    //  r02: Name Table base addr. (Handled different)
    //  r03: [IGNORED] Should be all set in mode 4.
    //  r04: [IGNORED] Bits 2-0 must be set.
    //  r05: Sprite Attribute Table base. (Same-ish, bit 0 selects sprite upper/lower 128B bank)
    //  r06: Sprite pattern generator base addr. (Bits 0,1 mask bits 8 and 6 of the tile index)
    //  r07: Overscan color.
    //  r08: X Scroll (new on VDP)
    //  r09: Y Scroll (new on VPD)
    //  r0A: V counter (same)
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