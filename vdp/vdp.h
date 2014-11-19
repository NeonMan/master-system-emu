#ifndef __VDP_H
#define __VDP_H

//VDP requires the Address bus, data bus and the #INT pin
#include "../z80/z80_externs.h"
#include <stdint.h>

//Values for the upper 2-bits from a control word (bits 14,15).
#define VDP_CTRL_VRAM_READ  (0 << 6)
#define VDP_CTRL_VRAM_WRITE (1 << 6)
#define VDP_CTRL_REGISTER   (2 << 6)
#define VDP_CTRL_CRAM       (3 << 6)
#define VDP_CTRL_MASK       (3 << 6)

#define VDP_CRAM_ADDR_MASK (0x3FFF)

//Register reference.
#define VDP_REG_MODE_CTRL1 0
#define VDP_REG_MODE_CTRL2 1
#define VDP_REG_NAME_TABLE_ADDR 2
#define VDP_REG_COLOR_TABLE_ADDR 3
#define VDP_REG_BKG_PATTERN_ADDR 4
#define VDP_REG_SPR_ATTRIBUTE_ADDR 5
#define VDP_REG_SPR_PATTERN_ADDR 6
#define VDP_REG_OVERSCAN_COLOR 7
#define VDP_REG_BKG_X_SCROLL 8
#define VDP_REG_BKG_Y_SCROLL 9
#define VDP_REG_LINE_COUNTER 10

#ifdef __cplusplus
//extern "C" {
#endif

//Types
struct vdp_s {
    uint8_t cram[32]; ///<-- Color RAM, 32 bytes
    uint8_t vram[16 * 1024]; ///<-- VRAM, 16K
    //uint8_t v; ///<-- V counter (Register #10)
    uint8_t h; ///<-- H counter
	uint8_t buffer;
	uint8_t status;

    uint16_t address; ///<-- Address register (14bit)
    uint8_t regs[16]; ///<-- Registers. Regs 11 to 15 have no effect.

	uint8_t control_word[2]; //<-- Stores the current control word
	uint8_t control_index; //<-- Which control word byte are we expecting [0,1]
	uint8_t control_mode;
};

void vdp_tick();
void vdp_init();

#ifdef __cplusplus
//}
#endif

#endif