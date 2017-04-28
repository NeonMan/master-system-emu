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

#include "b64.h"
#include "jsmn.h"
#include "savestate.h"
#include <stdlib.h>
#include <string.h>

#include <ram/ram.h>
#include <io/io.h>
#include <peripheral/peripheral.h>
#include <psg/psg.h>
#include <rom/rom.h>
#include <z80/z80.h>
#include <z80/z80_macros.h>

int ss_restore(FILE* f){
    //Read file, 8MB should be enough for everyone
    uint8_t* sav_buffer = (uint8_t*) malloc((1024 * 1024 * 8) + 1);
    int rv = 0;
    size_t size = fread(sav_buffer, 1, 1024 * 1024 * 8, f);
    sav_buffer[size] = 0;

    //Parse JSON tokens
    jsmn_parser parser;
    jsmntok_t tokens[100];
    jsmn_init(&parser);

    jsmn_parse(&parser, sav_buffer, strlen(sav_buffer), tokens, 100);

    /*
    dump_rom_mapper(f);
    dump_io(f);
    dump_peripheral(f);
    dump_psg(f);
    dump_z80(f);
    dump_vdp(f);
    dump_ram(f);
    dump_vram(f);
    */

    free(sav_buffer);
    return rv;
}
