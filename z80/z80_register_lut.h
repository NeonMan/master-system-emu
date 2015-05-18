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

#ifndef __Z80_REGISTER_LUT_H
#define __Z80_REGISTER_LUT_H

#include "z80_macros.h"
#include "z80.h"

#ifdef __cplusplus
extern "C" {
#endif

extern struct z80_s z80; //<-- Provided by z80.c, must be forward-declared here.

// --- Register lookup tables ---
// The pointers (all array values) themselves declared here are constant.
// The pointed variable is not.
///Register pairs lookup table
static uint16_t* const z80_rp[4] = { &Z80_BC, &Z80_DE, &Z80_HL, &Z80_SP };
///Register pairs lookup table (AF option)
static uint16_t* const z80_rp2[4] = { &Z80_BC, &Z80_DE, &Z80_HL, &Z80_AF };
///Register lookup table
static uint8_t* const z80_r[8] = { &Z80_B, &Z80_C, &Z80_D, &Z80_E, &Z80_H, &Z80_L, 0, &Z80_A };
///Condition Flag  mask lookup table (NZ,Z,NC,C,PO,PE,P,M)
static uint8_t  const z80_cc[8] = { Z80_FLAG_ZERO, Z80_FLAG_ZERO, Z80_FLAG_CARRY, Z80_FLAG_CARRY, Z80_FLAG_PARITY, Z80_FLAG_PARITY, Z80_FLAG_SIGN, Z80_FLAG_SIGN };
///Expected flag value (after mask) for the condition to be true
static uint8_t  const z80_cc_stat[8] = { 0, Z80_FLAG_ZERO, 0, Z80_FLAG_CARRY, 0, Z80_FLAG_PARITY, 0, Z80_FLAG_SIGN };
///Interrupt mode lookup table
static int8_t const z80_im[][2] = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 2 }, { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 2 } };

//IX version
///Register pairs lookup table
static uint16_t* const z80_rp_ix[4] = { &Z80_BC, &Z80_DE, &Z80_IX, &Z80_SP};
///Register pairs lookup table (AF option)
static uint16_t* const z80_rp2_ix[4] = { &Z80_BC, &Z80_DE, &Z80_IX, &Z80_AF};
///Register lookup table
static uint8_t* const z80_r_ix[8] = { &Z80_B, &Z80_C, &Z80_D, &Z80_E, &Z80_IXH, &Z80_IXL, 0, &Z80_A };
                                    /*0       1       2       3       4         5         6  7      */

//IY version
///Register pairs lookup table
static uint16_t* const z80_rp_iy[4] = { &Z80_BC, &Z80_DE, &Z80_IY, &Z80_SP};
///Register pairs lookup table (AF option)
static uint16_t* const z80_rp2_iy[4] = { &Z80_BC, &Z80_DE, &Z80_IY, &Z80_AF};
///Register lookup table
static uint8_t* const z80_r_iy[8] = { &Z80_B, &Z80_C, &Z80_D, &Z80_E, &Z80_IYH, &Z80_IYL, 0, &Z80_A };

#ifdef __cplusplus
}
#endif

#endif