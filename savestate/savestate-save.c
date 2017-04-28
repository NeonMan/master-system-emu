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
 *  @file  savestate/savestate-save.c
 *  @brief Utilities to dump emulator state.
 *
 *  Savestate file format.
 *  [FIELD:]+\ <value>
 *
 */

#include "savestate.h"
#include <ram/ram.h>
#include <rom/rom.h>
#include <io/io_externs.h>
#include <peripheral/peripheral.h>
#include <psg/psg.h>
#include <z80/z80.h>
#include <z80/z80_macros.h>
#include <vdp/vdp_internals.h>
#include <glue/glue.h>
#include "b64.h"

#include <stdlib.h>
#include <string.h>

#define WRITE_COMMA fprintf(f, ",\n");
#define WRITE_NL    fprintf(f, "\n");

static void encode_blob(uint8_t* in, uint8_t* out, int size) {
    out[0] = '\0';
    while (size >= 3) {
        b64_encodeblock(in, out, 3);
        in += 3;
        out += 4;
        size -= 3;
    }
    if (size != 0) {
        out[0] = 0;
        out[1] = 0;
        out[2] = 0;
        out[3] = 0;
        out[4] = 0;
        b64_encodeblock(in, out, size);
    }
}

// --------------------
// --- Binary blobs ---
// --------------------

static int dump_ram(FILE* f){
    //Make a buffer to contain a base64 encoding of the whole RAM. 
    uint8_t* b64_buffer = (uint8_t*) malloc(((RAM_SIZE * 4)/3) + 7);

    encode_blob((uint8_t*)ramdbg_get_mem(), b64_buffer, RAM_SIZE);
    fprintf(f, "\t\"ram\": \"%s\",\n", b64_buffer);

    free(b64_buffer);
    return 0;
}

static int dump_vram(FILE* f) {
    uint8_t* b64_buffer = (uint8_t*)malloc(((VDP_VRAM_SIZE * 4) / 3) + 7);

    encode_blob((uint8_t*)vdp_get_vram(), b64_buffer, VDP_VRAM_SIZE);
    fprintf(f, "\t\"vram\": \"%s\",\n", b64_buffer);

    free(b64_buffer);
    return 0;
}

static int dump_rom(FILE* f, int rom_size){
    uint8_t* b64_buffer = (uint8_t*)malloc(((ROM_MAX_SIZE * 4) / 3) + 7);
    const uint8_t* rom_p = (const uint8_t*) romdbg_get_rom();

    encode_blob((uint8_t*) romdbg_get_rom(), b64_buffer, ROM_MAX_SIZE);
    fprintf(f, "\t\"rom\": \"%s\",\n", b64_buffer);

    free(b64_buffer);
    return 0;
}

// ----------------------
// --- Emulator state ---
// ----------------------

static int write_str(FILE* f, int depth, const char* name, const char* val) {
    while (depth) {
        fprintf(f, "\t");
        depth--;
    }

    fprintf(f, "\"%s\": \"%s\"", name, val);
    return 0;
}

static int write_int(FILE* f, int depth, const char* name, int32_t val) {
    while (depth) {
        fprintf(f, "\t");
        depth--;
    }

    fprintf(f, "\"%s\": %d", name, val);
    return 0;
}

static int dump_rom_name(FILE* f, const char* rom_name){
    write_str(f, 1, "rom_name", rom_name); WRITE_COMMA;
    return 0;
}

static int dump_rom_mapper(FILE* f){
    fprintf(f, "\t\"mapper\": {\n");
    {
        write_int(f, 2, "slot0", *romdbg_get_slot(0)); WRITE_COMMA;
        write_int(f, 2, "slot1", *romdbg_get_slot(1)); WRITE_COMMA;
        write_int(f, 2, "slot2", *romdbg_get_slot(2)); WRITE_NL;
    }
    fprintf(f, "\t}"); WRITE_COMMA;
    return 0;
}

static int dump_io(FILE* f){
    {
        write_int(f, 1, "io_stat", io_stat); WRITE_COMMA;
    }
    return 0;
}

static int dump_peripheral(FILE* f){
    fprintf(f, "\t\"peripheral\": {\n");
    {
        write_int(f, 2, "control", *perdbg_reg_control()); WRITE_COMMA;
        write_int(f, 2, "ab", *perdbg_reg_ab()); WRITE_COMMA;
        write_int(f, 2, "bm", *perdbg_reg_bm()); WRITE_NL;
    }
    fprintf(f, "\t}"); WRITE_COMMA;
    return 0;
}

static int dump_psg(FILE* f){
    fprintf(f, "\t\"psg\": {\n");
    {
        const int8_t* vol    = psgdbg_get_volume();
        const uint16_t* tone = psgdbg_get_tone();
        fprintf(f, "\t\t\"volume\": [%d, %d, %d, %d]", vol[0], vol[1], vol[2], vol[3]); WRITE_COMMA;
        fprintf(f, "\t\t\"tone\": [%d, %d, %d, %d]", tone[0], tone[1], tone[2], tone[3]); WRITE_NL;
    }
    fprintf(f, "\t}"); WRITE_COMMA;
    return 0;
}

static int dump_z80(FILE* f){
    fprintf(f, "\t\"z80\": {\n");
    {

    }
    fprintf(f, "\t}"); WRITE_COMMA;
    return 0;
}

static int dump_vdp(FILE* f) {
    fprintf(f, "\t\"vdp\": {\n");
    {

    }
    fprintf(f, "\t}"); WRITE_COMMA;
    return 0;
}

int ss_save(FILE* f, const char* rom_name){
    fprintf(f, "{\n"); //<-- JSON Begin

    dump_rom_mapper(f);
    
    dump_io(f);
    
    dump_peripheral(f);

    dump_psg(f);

    dump_z80(f);

    dump_vdp(f);

    dump_ram(f);

    dump_vram(f);

    if (rom_name) {
        dump_rom_name(f, rom_name);
    }
    else {
        dump_rom(f, ROM_MAX_SIZE);
    }

    fprintf(f, "\t\"emu_version\": \"%s\",\n", SRC_VERSION);
    fprintf(f, "\t\"sav_version\": \"%s\"\n", SAVESTATE_VERSION);

    fprintf(f, "}\n"); //<-- JSON end

    return 0;
}
