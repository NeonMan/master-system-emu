// Copyright 2015 Juan Luis Álvarez Martínez
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http ://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
* @file z80_decoder.h
* @brief Header file for the z80 opcode implementations.
*
*/
#ifndef __Z80_DECODER_H
#define __Z80_DECODER_H

#include "z80.h"

#ifdef __cplusplus
extern "C" {
#endif

//Sdsc data/control function pointers (decouple z80 from sdsc.h)
//void(*z80_sdsc_data) (uint8_t) = 0; ///<-- Function pointer for SDSC data port.
//void(*z80_sdsc_control) (uint8_t) = 0; ///<-- Function pointer for SDSC control port.

extern struct z80_s z80; //<-- Requires access to the z80 internals

//Functions
/**
 * @brief executes the currently loaded opcode.
 *
 * @return Z80_STAGE_RESET on completed execution; Z80_STAGE_M1, Z80_STAGE_M2 or Z80_STAGE_M3 if opcode requires more actions.
 */
int z80_instruction_decode();

#ifdef __cplusplus
}
#endif

#endif