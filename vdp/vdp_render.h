#ifndef __VDP_RENDER_H
#define __VDP_RENDER_H

#include <stdint.h>

// --- TMS modes ---
///Renders picture to framebuffer assuming Mode0
void vdp_mode0_pixels(uint8_t* fb);
///Renders picture to framebuffer assuming Mode1 (TEXT)
void vdp_mode1_pixels(uint8_t* fb);
///Renders picture to framebuffer assuming Mode2 (GRAPHICS 2)
void vdp_mode2_pixels(uint8_t* fb);
///Renders picture to framebuffer assuming Mode3 (MULTICOLOR)
void vdp_mode3_pixels(uint8_t* fb);

// --- SMS Mode 4 ---
///Renders picture to framebuffer assuming Mode 4 (SMS)
void vdp_mode4_pixels(uint8_t* fb);

// --- Undocumented modes ---
///Renders picture to framebuffer assuming mode 1+2
void vdp_mode12_pixels(uint8_t* fb);
///Renders picture to framebuffer assuming mode 1+3
void vdp_mode13_pixels(uint8_t* fb);
///Renders picture to framebuffer assuming mode 2+3
void vdp_mode23_pixels(uint8_t* fb);
///Renders picture to framebuffer assuming mode 1+2+3
void vdp_mode123_pixels(uint8_t* fb);

#endif