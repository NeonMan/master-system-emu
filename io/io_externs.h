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

/** @file io_externs.h
 *  @brief Variable declarations for IO module.
 * 
 *  Provides the variables needed by code using the IO chip outputs.
 *
 */
#ifndef __IO_EXTERNS_H
#define __IO_EXTERNS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//IO mapper enable bits. Active LOW
#define IO_EXPANSION_SLOT (1<<7)
#define IO_CARTRIDGE_SLOT (1<<6)
#define IO_CARD_SLOT      (1<<5)
#define IO_RAM            (1<<4)
#define IO_BIOS           (1<<3)
#define IO_PERIPHERIAL    (1<<2)
#define IO_UNK1           (1<<1)
#define IO_UNK0           (1)

extern uint8_t io_stat; ///<-- The exported CE lines. MREQ/IOREQ dependant

#ifdef __cplusplus
}
#endif

#endif
