#ifndef __VDP_RENDER_H
#define __VDP_RENDER_H

#include "vdp.h"
#include "vdp_constants.h"
#include <stdint.h>

///Renders picture to framebuffer assuming Mode0
void vdp_mode0_pixels(uint8_t* framebuffer);
///Renders picture to framebuffer assuming Mode1 (TEXT)
void vdp_mode1_pixels(uint8_t* framebuffer);
///Renders picture to framebuffer assuming Mode2 (GRAPHICS 2)
void vdp_mode2_pixels(uint8_t* framebuffer);
///Renders picture to framebuffer assuming Mode3 (MULTICOLOR)
void vdp_mode3_pixels(uint8_t* framebuffer);

#endif