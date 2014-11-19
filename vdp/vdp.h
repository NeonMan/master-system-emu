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
};

void vdp_tick();
void vdp_init();

#ifdef __cplusplus
//}
#endif

#endif