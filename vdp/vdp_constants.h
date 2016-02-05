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

/**
 * @file vdp_constants.h
 * @brief VDP and VDP mode constants.
 *
 */
#ifndef __VDP_CONSTANTS_H
#define __VDP_CONSTANTS_H

#include <stdint.h>

// --- Sizes ---
#define VDP_VRAM_SIZE (16 * 1024)
#define VDP_CRAM_SIZE 32
#define VDP_WIDTH_PIXELS  256
#define VDP_HEIGHT_PIXELS 192
#define VDP_FRAMEBUFFER_SIZE 49152

// --- Mode 0 constants ---
#define VDP_MODE0_ROWS 24
#define VDP_MODE0_COLS 32
#define VDP_MODE0_PATTERN_SIZE 8

// --- Mode 1 constants ---
#define VDP_MODE1_ROWS 24
#define VDP_MODE1_COLS 40
#define VDP_MODE1_PATTERN_SIZE 8
#define VDP_MODE1_WIDTH_PIXELS 240

// --- Addressing offsets ---
//Address registers are multiplied by a constant to get the effective address in VRAM
#define VDP_NAME_TABLE_SIZE              0x400 /*From TMS9918 datasheet 2.2.3*/
#define VDP_COLOR_TABLE_SIZE             0x40  /*    "    */
#define VDP_PATTERN_GENERATOR_TABLE_SIZE 0x800 /*    "    */
#define VDP_SPRITE_ATTR_TABLE_SIZE       0x80  /*    "    */
#define VDP_SPRITE_GENERATOR_TABLE_SIZE  0x800 /*    "    */

// --- VDP Colors ---
#define VDP_COLOR_RGBA(R,G,B,A) ((uint32_t)((A<<24)+(R<<16)+(G<<8)+B))
#define VDP_COLOR_RGB(R,G,B) VDP_COLOR_RGBA(R,G,B,255)

#define VDP_COLOR_NONE   VDP_COLOR_RGBA(128, 128, 128, 0)
#define VDP_COLOR_BLACK  VDP_COLOR_RGB(  0,   0,   0)
#define VDP_COLOR_MGREEN VDP_COLOR_RGB( 33, 200,  66)
#define VDP_COLOR_LGREEN VDP_COLOR_RGB( 94, 220, 120)

#define VDP_COLOR_DBLUE VDP_COLOR_RGB( 84,  85, 237)
#define VDP_COLOR_LBLUE VDP_COLOR_RGB(125, 118, 252)
#define VDP_COLOR_DRED  VDP_COLOR_RGB(212,  82,  77)
#define VDP_COLOR_CYAN  VDP_COLOR_RGB( 66, 235, 245)

#define VDP_COLOR_MRED    VDP_COLOR_RGB(252,  85,  84)
#define VDP_COLOR_LRED    VDP_COLOR_RGB(255, 121, 120)
#define VDP_COLOR_DYELLOW VDP_COLOR_RGB(212, 193,  84)
#define VDP_COLOR_LYELLOW VDP_COLOR_RGB(230, 206, 128)

#define VDP_COLOR_DGREEN  VDP_COLOR_RGB( 33, 176,  59)
#define VDP_COLOR_MAGENTA VDP_COLOR_RGB(201,  91, 186)
#define VDP_COLOR_GRAY    VDP_COLOR_RGB(204, 204, 204)
#define VDP_COLOR_WHITE   VDP_COLOR_RGB(255, 255, 255)

static uint32_t vdp_tmscolors[16] = {
    VDP_COLOR_NONE, VDP_COLOR_BLACK, VDP_COLOR_MGREEN, VDP_COLOR_LGREEN,
    VDP_COLOR_DBLUE, VDP_COLOR_LBLUE, VDP_COLOR_DRED, VDP_COLOR_CYAN,
    VDP_COLOR_MRED, VDP_COLOR_LRED, VDP_COLOR_DYELLOW, VDP_COLOR_LYELLOW,
    VDP_COLOR_DGREEN, VDP_COLOR_MAGENTA, VDP_COLOR_GRAY, VDP_COLOR_WHITE
};

#endif