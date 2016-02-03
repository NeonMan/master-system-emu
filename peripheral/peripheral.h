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
 * @file peripheral.h
 * @brief Peripheral ports module.
 */
#ifndef __PERIPHERIAL_H
#define __PERIPHERIAL_H

#include <stdint.h>
#include "../z80/z80_externs.h"
#include "../io/io_externs.h"

#ifdef __cplusplus
extern "C" {
#endif

///Updates the peripheral ports.
void per_tick();

//Debug
uint8_t* perdbg_reg_control();
uint8_t* perdbg_reg_ab();
uint8_t* perdbg_reg_bm();

#ifdef __cplusplus
}
#endif
#endif