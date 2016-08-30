/**
 * @file  vdp.h
 * @brief Video chip functions.
 */

#ifndef __VDP_H
#define __VDP_H

#include <stdint.h>

#define VDP_DATA_PORT    0xBE
#define VDP_CONTROL_PORT 0xBF
#define VDP_V_PORT       0x7E
#define VDP_H_PORT       0x7F

#define VDP_COMMAND_VRAM_READ  0
#define VDP_COMMAND_VRAM_WRITE 1
#define VDP_COMMAND_CRAM_WRITE 3
#define VDP_COMMAND(CMD,ADDR) ( (uint16_t) ((((uint16_t)(CMD))<<14) | (((uint16_t)(ADDR))& 0x3FFF)) )
#define VDP_COMMAND_REGISTER_WRITE(REG,DATA) ( (uint16_t) ((((uint16_t) (( (REG) & 0x0F ) | 0x80)) << 8) | ((DATA) & 0xFF)) )

void vdp_reg_write(uint8_t reg, uint8_t data);

#endif
