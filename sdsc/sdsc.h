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

/**
* @file sdsc.h
* @brief Sample implementations of SDSC callbacks.
*
*/
#ifndef __SDSC_H
#define __SDSC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

///Sample SDSC data callback function.
void sdsc_write(uint8_t b);
///Sample SDSC control callback function.
void sdsc_control(uint8_t b);

#ifdef __cplusplus
}
#endif

#endif