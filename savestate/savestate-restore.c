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
 *  @file  savestate/savestate-restore.c
 *  @brief Utilities to dump emulator state.
 */

#include "savestate.h"
#include <string.h>

#include <ram/ram.h>
#include <io/io.h>
#include <peripheral/peripheral.h>
#include <psg/psg.h>
#include <rom/rom.h>
#include <z80/z80.h>
#include <z80/z80_macros.h>

int ss_restore(FILE* f){
    int rv = 0;
    return rv;
}
