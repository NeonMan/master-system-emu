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

// ---------------------
// --- Restore blobs ---
// ---------------------

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

// ----------------------
// --- Restore states ---
// ----------------------

static void restore_mapper(const jsmntok_t* tokens, const uint8_t* sav) {
    const jsmntok_t* token_slot0 = find_token(tokens, sav, "slot0");
    const jsmntok_t* token_slot1 = find_token(tokens, sav, "slot1");
    const jsmntok_t* token_slot2 = find_token(tokens, sav, "slot2");
    assert(token_slot0);
    assert(token_slot1);
    assert(token_slot2);

    char num_str[10];
    int num_str_len;

    //Slot0
    memset(num_str, 0, 10);
    num_str_len = token_slot0->end - token_slot0->start;
    num_str_len = (num_str_len > 9) ? 9 : num_str_len;
    strncpy(num_str, (const char*) (sav + token_slot0->start), num_str_len);
    *romdbg_get_slot(0) = atoi(num_str);

    //Slot1
    memset(num_str, 0, 10);
    num_str_len = token_slot1->end - token_slot1->start;
    num_str_len = (num_str_len > 9) ? 9 : num_str_len;
    strncpy(num_str, (const char*)(sav + token_slot1->start), num_str_len);
    *romdbg_get_slot(1) = atoi(num_str);

    //Slot2
    memset(num_str, 0, 10);
    num_str_len = token_slot2->end - token_slot2->start;
    num_str_len = (num_str_len > 9) ? 9 : num_str_len;
    strncpy(num_str, (const char*)(sav + token_slot2->start), num_str_len);
    *romdbg_get_slot(2) = atoi(num_str);
}

static void restore_io(const jsmntok_t* tokens, const uint8_t* sav) {
    const jsmntok_t* token_io = find_token(tokens, sav, "io_stat");
    assert(token_io);

    char num_str[10];
    int num_str_len;

    //io stat.
    memset(num_str, 0, 10);
    num_str_len = token_io->end - token_io->start;
    num_str_len = (num_str_len > 9) ? 9 : num_str_len;
    strncpy(num_str, (const char*)(sav + token_io->start), num_str_len);
    io_stat = (uint8_t) atoi(num_str);
}

static void restore_peripheral(const jsmntok_t* tokens, const uint8_t* sav) {
    char num_str[10];
    int num_str_len;
    const jsmntok_t* token;

    //control
    token = find_token(tokens, sav, "control");
    assert(token);
    memset(num_str, 0, 10);
    num_str_len = token->end - token->start;
    num_str_len = (num_str_len > 9) ? 9 : num_str_len;
    strncpy(num_str, (const char*)(sav + token->start), num_str_len);
    *perdbg_reg_control() = (uint8_t) atoi(num_str);


    //ab
    token = find_token(tokens, sav, "ab");
    assert(token);
    memset(num_str, 0, 10);
    num_str_len = token->end - token->start;
    num_str_len = (num_str_len > 9) ? 9 : num_str_len;
    strncpy(num_str, (const char*)(sav + token->start), num_str_len);
    *perdbg_reg_ab() = (uint8_t)atoi(num_str);


    //bm
    token = find_token(tokens, sav, "bm");
    assert(token);
    memset(num_str, 0, 10);
    num_str_len = token->end - token->start;
    num_str_len = (num_str_len > 9) ? 9 : num_str_len;
    strncpy(num_str, (const char*)(sav + token->start), num_str_len);
    *perdbg_reg_bm() = (uint8_t)atoi(num_str);
}

static void restore_psg(const jsmntok_t* tokens, const uint8_t* sav) {
    char num_str[10];
    int num_str_len;
    const jsmntok_t* token;

    //volume
    token = find_token(tokens, sav, "volume");
    assert(token);
    assert(token->size >= 4);
    {
        //[0]
        token++;
        memset(num_str, 0, 10);
        num_str_len = token->end - token->start;
        num_str_len = (num_str_len > 9) ? 9 : num_str_len;
        strncpy(num_str, (const char*)(sav + token->start), num_str_len);
        psgdbg_get_volume()[0] = (uint8_t)atoi(num_str);

        //[1]
        token++;
        memset(num_str, 0, 10);
        num_str_len = token->end - token->start;
        num_str_len = (num_str_len > 9) ? 9 : num_str_len;
        strncpy(num_str, (const char*)(sav + token->start), num_str_len);
        psgdbg_get_volume()[1] = (uint8_t)atoi(num_str);

        //[2]
        token++;
        memset(num_str, 0, 10);
        num_str_len = token->end - token->start;
        num_str_len = (num_str_len > 9) ? 9 : num_str_len;
        strncpy(num_str, (const char*)(sav + token->start), num_str_len);
        psgdbg_get_volume()[2] = (uint8_t)atoi(num_str);

        //[3]
        token++;
        memset(num_str, 0, 10);
        num_str_len = token->end - token->start;
        num_str_len = (num_str_len > 9) ? 9 : num_str_len;
        strncpy(num_str, (const char*)(sav + token->start), num_str_len);
        psgdbg_get_volume()[3] = (uint8_t)atoi(num_str);
    }

    //Tone
    token = find_token(tokens, sav, "tone");
    assert(token);
    assert(token->size >= 4);
    {
        //[0]
        token++;
        memset(num_str, 0, 10);
        num_str_len = token->end - token->start;
        num_str_len = (num_str_len > 9) ? 9 : num_str_len;
        strncpy(num_str, (const char*)(sav + token->start), num_str_len);
        psgdbg_get_tone()[0] = (uint16_t)atoi(num_str);

        //[1]
        token++;
        memset(num_str, 0, 10);
        num_str_len = token->end - token->start;
        num_str_len = (num_str_len > 9) ? 9 : num_str_len;
        strncpy(num_str, (const char*)(sav + token->start), num_str_len);
        psgdbg_get_tone()[1] = (uint16_t)atoi(num_str);

        //[2]
        token++;
        memset(num_str, 0, 10);
        num_str_len = token->end - token->start;
        num_str_len = (num_str_len > 9) ? 9 : num_str_len;
        strncpy(num_str, (const char*)(sav + token->start), num_str_len);
        psgdbg_get_tone()[2] = (uint16_t)atoi(num_str);

        //[3]
        token++;
        memset(num_str, 0, 10);
        num_str_len = token->end - token->start;
        num_str_len = (num_str_len > 9) ? 9 : num_str_len;
        strncpy(num_str, (const char*)(sav + token->start), num_str_len);
        psgdbg_get_tone()[3] = (uint16_t)atoi(num_str);
    }
}

static void restore_z80_pins(const jsmntok_t* tokens, const uint8_t* sav) {
    char num_str[10];
    int num_str_len;
    const jsmntok_t* token;

    //!RD
    token = find_token(tokens, sav, "n_rd");
    assert(token);
    memset(num_str, 0, 10);
    num_str_len = token->end - token->start;
    num_str_len = (num_str_len > 9) ? 9 : num_str_len;
    strncpy(num_str, (const char*)(sav + token->start), num_str_len);
    z80_n_rd = (uint8_t)atoi(num_str);

    //!WR
    token = find_token(tokens, sav, "n_wr");
    assert(token);
    memset(num_str, 0, 10);
    num_str_len = token->end - token->start;
    num_str_len = (num_str_len > 9) ? 9 : num_str_len;
    strncpy(num_str, (const char*)(sav + token->start), num_str_len);
    z80_n_wr = (uint8_t)atoi(num_str);

    //!MREQ
    token = find_token(tokens, sav, "n_mreq");
    assert(token);
    memset(num_str, 0, 10);
    num_str_len = token->end - token->start;
    num_str_len = (num_str_len > 9) ? 9 : num_str_len;
    strncpy(num_str, (const char*)(sav + token->start), num_str_len);
    z80_n_mreq = (uint8_t)atoi(num_str);

    //!IOREQ
    token = find_token(tokens, sav, "n_ioreq");
    assert(token);
    memset(num_str, 0, 10);
    num_str_len = token->end - token->start;
    num_str_len = (num_str_len > 9) ? 9 : num_str_len;
    strncpy(num_str, (const char*)(sav + token->start), num_str_len);
    z80_n_ioreq = (uint8_t)atoi(num_str);

    //!RFSH
    token = find_token(tokens, sav, "n_rfsh");
    assert(token);
    memset(num_str, 0, 10);
    num_str_len = token->end - token->start;
    num_str_len = (num_str_len > 9) ? 9 : num_str_len;
    strncpy(num_str, (const char*)(sav + token->start), num_str_len);
    z80_n_rfsh = (uint8_t)atoi(num_str);

    //!M1
    token = find_token(tokens, sav, "n_m1");
    assert(token);
    memset(num_str, 0, 10);
    num_str_len = token->end - token->start;
    num_str_len = (num_str_len > 9) ? 9 : num_str_len;
    strncpy(num_str, (const char*)(sav + token->start), num_str_len);
    z80_n_m1 = (uint8_t)atoi(num_str);

    //!INT
    token = find_token(tokens, sav, "n_int");
    assert(token);
    memset(num_str, 0, 10);
    num_str_len = token->end - token->start;
    num_str_len = (num_str_len > 9) ? 9 : num_str_len;
    strncpy(num_str, (const char*)(sav + token->start), num_str_len);
    z80_n_int = (uint8_t)atoi(num_str);

    //!NMI
    token = find_token(tokens, sav, "n_nmi");
    assert(token);
    memset(num_str, 0, 10);
    num_str_len = token->end - token->start;
    num_str_len = (num_str_len > 9) ? 9 : num_str_len;
    strncpy(num_str, (const char*)(sav + token->start), num_str_len);
    z80_n_nmi = (uint8_t)atoi(num_str);

    //!RST
    token = find_token(tokens, sav, "n_reset");
    assert(token);
    memset(num_str, 0, 10);
    num_str_len = token->end - token->start;
    num_str_len = (num_str_len > 9) ? 9 : num_str_len;
    strncpy(num_str, (const char*)(sav + token->start), num_str_len);
    z80_n_reset = (uint8_t)atoi(num_str);

    //!WAIT
    token = find_token(tokens, sav, "n_wait");
    assert(token);
    memset(num_str, 0, 10);
    num_str_len = token->end - token->start;
    num_str_len = (num_str_len > 9) ? 9 : num_str_len;
    strncpy(num_str, (const char*)(sav + token->start), num_str_len);
    z80_n_wait = (uint8_t)atoi(num_str);

    //!BUSREQ
    token = find_token(tokens, sav, "n_busreq");
    assert(token);
    memset(num_str, 0, 10);
    num_str_len = token->end - token->start;
    num_str_len = (num_str_len > 9) ? 9 : num_str_len;
    strncpy(num_str, (const char*)(sav + token->start), num_str_len);
    z80_n_busreq = (uint8_t)atoi(num_str);

    //!BUSACK
    token = find_token(tokens, sav, "n_busack");
    assert(token);
    memset(num_str, 0, 10);
    num_str_len = token->end - token->start;
    num_str_len = (num_str_len > 9) ? 9 : num_str_len;
    strncpy(num_str, (const char*)(sav + token->start), num_str_len);
    z80_n_busack = (uint8_t)atoi(num_str);

    //ADDRESS
    token = find_token(tokens, sav, "address");
    assert(token);
    memset(num_str, 0, 10);
    num_str_len = token->end - token->start;
    num_str_len = (num_str_len > 9) ? 9 : num_str_len;
    strncpy(num_str, (const char*)(sav + token->start), num_str_len);
    z80_address = (uint16_t)atoi(num_str);

    //DATA
    token = find_token(tokens, sav, "data");
    assert(token);
    memset(num_str, 0, 10);
    num_str_len = token->end - token->start;
    num_str_len = (num_str_len > 9) ? 9 : num_str_len;
    strncpy(num_str, (const char*)(sav + token->start), num_str_len);
    z80_data = (uint8_t)atoi(num_str);
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

    restore_mapper(tokens, sav_buffer);
    restore_io(tokens, sav_buffer);
    restore_peripheral(tokens, sav_buffer);
    restore_psg(tokens, sav_buffer);
    restore_z80_pins(tokens, sav_buffer);
    /*
    dump_z80(f);
    dump_vdp(f);
    */

    free(sav_buffer);
    return rv;
}
