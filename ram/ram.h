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
 *  @file ram.h
 *  @brief RAM module functions.
 */
#ifndef __RAM_H
#define __RAM_H
#include <stddef.h>

#define RAM_SIZE (1024 * 8)
#define RAM_BASE_ADDRESS 0xC000

#ifdef __cplusplus
extern "C" {
#endif

///Executes a RAM read/write.
void ram_tick();

//Debug functions
///Returns a pointer to the ram image.
void* ramdbg_get_mem();

#ifdef __cplusplus
}
#endif

#endif
