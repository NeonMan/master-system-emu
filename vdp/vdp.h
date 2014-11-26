#ifndef __VDP_H
#define __VDP_H

//VDP requires the Address bus, data bus and the #INT pin
#include "../z80/z80_externs.h"
#include <stdint.h>

#ifdef __cplusplus
//extern "C" {
#endif

//Types
struct vdp_s {
    uint8_t cram[32]; ///<-- Color RAM, 32 bytes
    uint8_t vram[16 * 1024]; ///<-- VRAM, 16K
    uint8_t v; ///<-- V counter
    uint8_t h; ///<-- H counter

    uint8_t address[2]; ///<-- Address register (14bit)
    uint8_t regs[16]; ///<-- Registers. Regs 11 to 15 have no effect.

    uint8_t framebuffer[256 * 192]; ///<-- The current image. Always 256*192 acc. to Sean Young.
};

struct vdp_pixels_s {
    int pixel_format;
    void* bytes;
};
typedef struct vdp_pixels_s vdp_pixels;

///executes the minimum relevant period of time.
void vdp_tick();

///Sets up all the VDP state and stuff. Call once.
void vdp_init();

///Returns the current picture. Pixels in VDP format.
vdp_pixels vdp_get_pixels();

#ifdef __cplusplus
//}
#endif

#endif