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

#define MAX_JSON_TOKENS 256

#include "b64.h"
#include "jsmn.h"
#include "savestate.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <ram/ram.h>
#include <io/io.h>
#include <peripheral/peripheral.h>
#include <psg/psg.h>
#include <rom/rom.h>
#include <z80/z80.h>
#include <z80/z80_macros.h>
#include <vdp/vdp.h>

static const jsmntok_t* find_token(const jsmntok_t* tokens, const uint8_t* sav, const char* name) {
    jsmntok_t root;
    root = tokens[0];

    //Find a STRING token with _one_ child then return the next token.
    assert(root.size > 0);
    tokens++; //<-- Enter the first child token
    for (int i = 0; i < MAX_JSON_TOKENS; i++) {
        if ((tokens[i].type == JSMN_STRING) && (tokens[i].size == 1)) {
            size_t begin, size;
            begin = tokens[i].start;
            size = tokens[i].end - begin;
            if (strncmp(name, (const char*)(sav + begin), strlen(name)) == 0) {
                return tokens + i + 1;
            }
        }
    }
    return 0;
}

static void decode_blob(const uint8_t* in, uint8_t* out, size_t str_size) {
    assert(in);
    assert(out);
    ///@bug do some boundary check since it can be off-by-3 depending on b64 padding.
    while (str_size > 3) {
        b64_decodeblock(in, out);
        in += 4;
        out += 3;
        str_size -= 4;
    }
    if (str_size) {
        b64_decodeblock(in, out);
    }
}

#define MAX_RAM_STR_SIZE 10925
static void restore_ram(const jsmntok_t* tokens, const uint8_t* sav) {
    //find the [root].ram token
    const jsmntok_t* ram_object = find_token(tokens, sav, "ram");
    assert(ram_object);

    //Copy at most 10925 characters from the ram object content.
    size_t str_size = ram_object->end - ram_object->start;
    str_size = (str_size > MAX_RAM_STR_SIZE) ? MAX_RAM_STR_SIZE : str_size;

    decode_blob(sav + ram_object->start, (uint8_t*) ramdbg_get_mem(), str_size);
}

#define MAX_VRAM_STR_SIZE 21848
static void restore_vram(const jsmntok_t* tokens, const uint8_t* sav) {
    //find the [root].vram token
    const jsmntok_t* vram_object = find_token(tokens, sav, "vram");
    assert(vram_object);

    //Copy at most 21848 characters from the vram object content.
    size_t str_size = vram_object->end - vram_object->start;
    str_size = (str_size > MAX_VRAM_STR_SIZE) ? MAX_VRAM_STR_SIZE : str_size;

    decode_blob(sav + vram_object->start, (uint8_t*) vdp_get_vram(), str_size);
}

#define MAX_ROM_STR_SIZE 5592408
static void restore_rom(const jsmntok_t* tokens, const uint8_t* sav) {
    //find the [root].rom token
    const jsmntok_t* rom_object = find_token(tokens, sav, "rom");
    assert(rom_object);

    //Copy at most 5592408 characters from the vram object content.
    size_t str_size = rom_object->end - rom_object->start;
    str_size = (str_size > MAX_ROM_STR_SIZE) ? MAX_ROM_STR_SIZE : str_size;

    decode_blob(sav + rom_object->start, (uint8_t*)romdbg_get_rom(), str_size);
}

int ss_restore(FILE* f){
    //Read file, 8MB should be enough for everyone
    uint8_t* sav_buffer = (uint8_t*) malloc((1024 * 1024 * 8) + 1);
    int rv = 0;
    size_t size = fread(sav_buffer, 1, 1024 * 1024 * 8, f);
    sav_buffer[size] = 0;

    //Parse JSON tokens
    jsmn_parser parser;
    jsmntok_t tokens[MAX_JSON_TOKENS];
    jsmn_init(&parser);
    for (int i = 0; i < MAX_JSON_TOKENS; i++) {
        tokens[i].type = JSMN_UNDEFINED;
    }

    jsmn_parse(
        &parser,
        (const char*)sav_buffer,
        strlen((const char*)sav_buffer),
        tokens,
        MAX_JSON_TOKENS
    );

    restore_ram(tokens, sav_buffer);
    restore_vram(tokens, sav_buffer);
    restore_rom(tokens, sav_buffer);

    /*
    dump_rom_mapper(f);
    dump_io(f);
    dump_peripheral(f);
    dump_psg(f);
    dump_z80(f);
    dump_vdp(f);
    */

    free(sav_buffer);
    return rv;
}
