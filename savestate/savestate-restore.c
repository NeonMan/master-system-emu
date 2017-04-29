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
#include <z80/z80_internals.h>
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
               if (size == strlen(name)) {
                    return tokens + i + 1;
               }
            }
        }
    }
    return 0;
}

static int get_token_int(const jsmntok_t* token, const uint8_t* sav) {
    char num_str[10];
    int num_str_len;

    assert(token);
    memset(num_str, 0, 10);
    num_str_len = token->end - token->start;
    num_str_len = (num_str_len > 9) ? 9 : num_str_len;
    strncpy(num_str, (const char*)(sav + token->start), num_str_len);
    return atoi(num_str);
}

static int get_json_integer(const jsmntok_t* tokens, const uint8_t* sav, const char* name) {
    const jsmntok_t* token;
    token = find_token(tokens, sav, name);
    assert(token);
    return get_token_int(token, sav);
}

static void decode_blob(const uint8_t* in, uint8_t* out, size_t str_size) {
    assert(in);
    assert(out);
    assert((str_size % 4) == 0);
    ///@bug do some boundary check since it can be off-by-3 depending on b64 padding.

    //Decode all b64 blocks except the last
    while (str_size > 4) {
        b64_decodeblock(in, out);
        in += 4;
        out += 3;
        str_size -= 4;
    }
    //Decode last block to a temporary buffer
    {
        uint8_t last_block[3];
        b64_decodeblock(in, last_block);
        out[0] = last_block[0];
        if (in[2] == '=') {
            ;//Block contains one byte. Do nothing.
        }
        else if (in[3] == '=') {
            //Block contains 2 bytes.
            out[1] = last_block[1];
        }
        else {
            //Block contains 3 bytes.
            out[1] = last_block[1];
            out[2] = last_block[2];
        }
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

    //If available, restore, if not, handle rom_name and skip.
    if (rom_object) {
        //Copy at most 5592408 characters from the vram object content.
        size_t str_size = rom_object->end - rom_object->start;
        str_size = (str_size > MAX_ROM_STR_SIZE) ? MAX_ROM_STR_SIZE : str_size;

        decode_blob(sav + rom_object->start, (uint8_t*)romdbg_get_rom(), str_size);
    }
    else {
        rom_object = find_token(tokens, sav, "rom_name");
        assert(rom_object);
    }
}

// ----------------------
// --- Restore states ---
// ----------------------

static void restore_mapper(const jsmntok_t* tokens, const uint8_t* sav) {
    *romdbg_get_slot(0) = (uint8_t) get_json_integer(tokens, sav, "slot0");
    *romdbg_get_slot(1) = (uint8_t)get_json_integer(tokens, sav, "slot1");
    *romdbg_get_slot(2) = (uint8_t)get_json_integer(tokens, sav, "slot2");
}

static void restore_io(const jsmntok_t* tokens, const uint8_t* sav) {
    io_stat = (uint8_t)get_json_integer(tokens, sav, "io_stat");
}

static void restore_peripheral(const jsmntok_t* tokens, const uint8_t* sav) {
    *perdbg_reg_control() = (uint8_t)get_json_integer(tokens, sav, "control");
    *perdbg_reg_ab() = (uint8_t)get_json_integer(tokens, sav, "ab");
    *perdbg_reg_bm() = (uint8_t)get_json_integer(tokens, sav, "bm");
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
    z80_n_rd = (uint8_t)get_json_integer(tokens, sav, "n_rd");
    z80_n_wr = (uint8_t)get_json_integer(tokens, sav, "n_wr");
    z80_n_mreq = (uint8_t)get_json_integer(tokens, sav, "n_mreq");
    z80_n_ioreq = (uint8_t)get_json_integer(tokens, sav, "n_ioreq");
    z80_n_rfsh = (uint8_t)get_json_integer(tokens, sav, "n_rfsh");
    z80_n_m1 = (uint8_t)get_json_integer(tokens, sav, "n_m1");
    z80_n_int = (uint8_t)get_json_integer(tokens, sav, "n_int");
    z80_n_nmi = (uint8_t)get_json_integer(tokens, sav, "n_nmi");
    z80_n_reset = (uint8_t)get_json_integer(tokens, sav, "n_reset");
    z80_n_wait = (uint8_t)get_json_integer(tokens, sav, "n_wait");
    z80_n_busreq = (uint8_t)get_json_integer(tokens, sav, "n_busreq");
    z80_n_busack = (uint8_t)get_json_integer(tokens, sav, "n_busack");
    z80_address = (uint16_t)get_json_integer(tokens, sav, "address");
    z80_data = (uint8_t)get_json_integer(tokens, sav, "data");
}

static void restore_z80(const jsmntok_t* tokens, const uint8_t* sav) {
    Z80_AF  = (uint16_t)get_json_integer(tokens, sav, "af");
    Z80_BC  = (uint16_t)get_json_integer(tokens, sav, "bc");
    Z80_DE  = (uint16_t)get_json_integer(tokens, sav, "de");
    Z80_HL  = (uint16_t)get_json_integer(tokens, sav, "hl");
    Z80_AFp = (uint16_t)get_json_integer(tokens, sav, "afp");
    Z80_BCp = (uint16_t)get_json_integer(tokens, sav, "bcp");
    Z80_DEp = (uint16_t)get_json_integer(tokens, sav, "dep");
    Z80_HLp = (uint16_t)get_json_integer(tokens, sav, "hlp");

    Z80_IX = (uint16_t)get_json_integer(tokens, sav, "ix");
    Z80_IY = (uint16_t)get_json_integer(tokens, sav, "iy");

    Z80_I = (uint8_t)get_json_integer(tokens, sav, "i");
    Z80_R = (uint8_t)get_json_integer(tokens, sav, "r");

    Z80_SP = (uint16_t)get_json_integer(tokens, sav, "sp");
    Z80_PC = (uint16_t)get_json_integer(tokens, sav, "pc");

    z80.data_latch = (uint8_t)get_json_integer(tokens, sav, "data_latch");
    Z80_IFF1 = (uint8_t)get_json_integer(tokens, sav, "iff1");
    Z80_IFF2 = (uint8_t)get_json_integer(tokens, sav, "iff2");
    z80.opcode_index = (uint8_t)get_json_integer(tokens, sav, "opcode_index");
    //{Opcode buffer}
    {
        const jsmntok_t* tok = find_token(tokens, sav, "opcode");
        assert(tok);
        assert((tok->size) >= 4);
        tok++;
        z80.opcode[0] = get_token_int(tok, sav);
        z80.opcode[1] = get_token_int(tok+1, sav);
        z80.opcode[2] = get_token_int(tok+2, sav);
        z80.opcode[3] = get_token_int(tok+3, sav);
    }
    z80.int_mode = (uint8_t)get_json_integer(tokens, sav, "int_mode");
    z80.stage = (uint8_t)get_json_integer(tokens, sav, "stage");
    z80.m1_tick_count = (uint16_t)get_json_integer(tokens, sav, "m1_tick");
    z80.m2_tick_count = (uint16_t)get_json_integer(tokens, sav, "m2_tick");
    z80.m3_tick_count = (uint16_t)get_json_integer(tokens, sav, "m3_tick");

    z80.read_address = (uint16_t)get_json_integer(tokens, sav, "read_address");
    {
        const jsmntok_t* tok = find_token(tokens, sav, "read_buffer");
        assert(tok);
        assert((tok->size) >= 2);
        tok++;
        z80.read_buffer[0] = get_token_int(tok, sav);
        z80.read_buffer[1] = get_token_int(tok + 1, sav);
    }
    z80.read_index = (uint8_t)get_json_integer(tokens, sav, "read_index");
    z80.read_is_io = (uint8_t)get_json_integer(tokens, sav, "read_is_io");

    z80.write_address = (uint16_t)get_json_integer(tokens, sav, "write_address");
    {
        const jsmntok_t* tok = find_token(tokens, sav, "write_buffer");
        assert(tok);
        assert((tok->size) >= 2);
        tok++;
        z80.write_buffer[0] = get_token_int(tok, sav);
        z80.write_buffer[1] = get_token_int(tok + 1, sav);
    }
    z80.write_index = (uint8_t)get_json_integer(tokens, sav, "write_index");
    z80.write_is_io = (uint8_t)get_json_integer(tokens, sav, "write_is_io");
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
    restore_z80(tokens, sav_buffer);
    /*
    dump_vdp(f);
    */

    free(sav_buffer);
    return rv;
}
