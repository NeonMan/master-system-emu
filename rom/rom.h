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
 *  @file rom.h
 *  @brief ROM module functions.
 */
#ifndef __ROM_H
#define __ROM_H

#include <stdint.h>
#include <stddef.h>
#include "../z80/z80_externs.h"

#ifdef __cplusplus
extern "C" {
#endif

//Constants
#define ROM_MAX_SIZE (1024*1024*4) /**<-- Largest possible rom is 4M. */
#define ROM_SLOT0_ADDR 0xFFFD
#define ROM_SLOT1_ADDR 0xFFFE
#define ROM_SLOT2_ADDR 0xFFFF

//Functions

/**
* @brief Perform a ROM read/write. Sega mapper is implied.
*
* ROM is async, so every time this function is called, a read/write
* operation will be completed.
*/
void rom_tick();

/**
 * @brief Writes the rom image to be used.
 *
 * @param data Pointer to the rom bytes
 * @param count Size of the rom
 */
void rom_set_image(uint8_t* data, size_t count);

//Debug functions
///Return a pointer to the whole ROM image.
void* romdbg_get_rom();

///Return the mapper status
uint8_t* romdbg_get_slot(uint8_t slot);

#ifdef __cplusplus
}
#endif

#endif
