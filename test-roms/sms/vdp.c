/**
 * @file  sms.c
 * @brief Implementation of VDP functions.
 */

#include "vdp.h"

__sfr __at VDP_DATA_PORT    __vdp_data;
__sfr __at VDP_ADDRESS_PORT __vdp_address;
