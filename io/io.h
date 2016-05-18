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

/** @file io.h
 *  @brief IO chip function declarations.
 */
#ifndef __IO_H
#define __IO_H

#include <stdint.h>
#include "io_externs.h"
#include "../z80/z80_externs.h"

#ifdef __cplusplus
extern "C" {
#endif

///Updates the IO chip state.
void io_tick();

#ifdef __cplusplus
}
#endif

#endif