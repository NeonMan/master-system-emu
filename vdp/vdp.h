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
#define VDP_REG_TEXT_COLOR 7
#define VDP_REG_BKG_X_SCROLL 8
#define VDP_REG_BKG_Y_SCROLL 9
#define VDP_REG_LINE_COUNTER 10

//Status register flags and values
#define VDP_STAT_INT (vdp.status & (1<<7))
#define VDP_STAT_5S (vdp.status & (1<<6))
#define VDP_STAT_C (vdp.status & (1<<5))
#define VDP_STAT_FS (vdp.status & (7<<5)) /*Lower 5 bits*/

//Register flags
#define VDP_FLAG_EXTVID (vdp.regs[0] & 1)
#define VDP_FLAG_M2 (vdp.regs[0] & (1<<1))
#define VDP_FLAG_4_16K (vdp.regs[1] & (1<<7))
#define VDP_FLAG_BL (vdp.regs[1] & (1<<6))
#define VDP_FLAG_GINT (vdp.regs[1] & (1<<5))
#define VDP_FLAG_M1 (vdp.regs[1] & (1<<4))
#define VDP_FLAG_M3 (vdp.regs[1] & (1<<3))
#define VDP_FLAG_SI (vdp.regs[1] & (1<<1))
#define VDP_FLAG_MAG (vdp.regs[1] & (1<<0))

#ifdef __cplusplus
//extern "C" {
#endif

//Types
struct vdp_s {
    //IO status (prevents spurious writes)
    uint8_t ioreq_done;

    //Ram, registers and stuff
    uint8_t cram[32]; ///<-- Color RAM, 32 bytes
    uint8_t vram[16 * 1024]; ///<-- VRAM, 16K
    //uint8_t v; ///<-- V counter (Register #10)
    uint8_t h; ///<-- H counter
	uint8_t buffer;
	uint8_t status;

    uint16_t address; ///<-- Address register (14bit)
    uint8_t regs[16]; ///<-- Registers. Regs 11 to 15 have no effect.

	uint8_t control_word[2]; ///<-- Stores the current control word
	uint8_t control_index; ///<-- Which control word byte are we expecting [0,1]
	uint8_t control_mode;

    uint8_t framebuffer[256 * 192]; ///<-- Current picture being displayed
};

///executes the minimum relevant period of time.
void vdp_tick();

///Sets up all the VDP state and stuff. Call once.
void vdp_init();

///Returns the current picture. Pixels in VDP format.
void* vdp_get_pixels();

#ifdef __cplusplus
//}
#endif

#endif