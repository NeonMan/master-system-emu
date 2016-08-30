/**
 * @file  vdp.c
 * @brief Implementation of VDP functions.
 */

#include "vdp.h"

__sfr __at VDP_DATA_PORT    __vdp_data;
__sfr __at VDP_CONTROL_PORT __vdp_control;

__sfr __at VDP_V_PORT  __vdp_v;
__sfr __at VDP_H_PORT  __vdp_h;

void vdp_reg_write(uint8_t reg, uint8_t data){
    uint16_t control_word;
    control_word = VDP_COMMAND_REGISTER_WRITE(reg,data);
    __vdp_control = control_word & 0xFF;
    __vdp_control = (control_word >> 8) & 0xFF;
}

void vdp_command(uint8_t cmd, uint16_t addr){
    uint16_t control_word;
    control_word = VDP_COMMAND(cmd,addr);
    __vdp_control = control_word & 0xFF;
    __vdp_control = (control_word >> 8) & 0xFF;
}
