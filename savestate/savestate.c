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
 *  @file save-state.c
 *  @brief Utilities to dump/restore emulator state.
 *
 *  Savestate file format.
 *  [FIELD:]+\ <value>
 *
 *  Known formats.
 *  #: [Comment]
 *  RAM:[16bit chip address, in hex]:[Byte count]: [whitespace-separated bytes]
 *  ROM:NAME: [ROM filename]
 *  ROM:MAPPER:[0-4]: [Register value]
 *  ROM:DATA:[24bit chip address, in hex]:[Byte count]: [whitespace-separated bytes]
 *  IO: [IO register byte]
 *  PERIPHERAL:[CONTROL AB BM]: [register byte]
 */


#include "savestate.h"
#include <ram/ram.h>
#include <rom/rom.h>
#include <io/io_externs.h>
#include <peripheral/peripheral.h>

#define RAM_BYTES_PER_ROW 16
#define ROM_BYTES_PER_ROW 64

static int dump_ram(FILE* f){
    uint8_t* ram_bytes = (uint8_t*) ramdbg_get_mem();
    /*Dump in blocks all the RAM bytes*/
    fprintf(f, "#: Console RAM\n");
    for (int i = 0; i < RAM_SIZE; i = i + RAM_BYTES_PER_ROW) {
        fprintf(f, "RAM:0x%04X:0x%02X:", i, RAM_BYTES_PER_ROW);
        for (int j = 0; j < RAM_BYTES_PER_ROW; ++j){
            fprintf(f, " %02X", ram_bytes[i+j]);
        }
        fprintf(f, "\n");
    }
    return 0;
}

static dump_rom(FILE* f, int rom_size){
    uint8_t* rom_bytes = (uint8_t*) romdbg_get_rom();
    /*Dump in blocks all the ROM bytes*/
    fprintf(f, "#: Cartridge ROM\n");
    for (int i = 0; i < rom_size; i = i + ROM_BYTES_PER_ROW) {
        fprintf(f, "ROM:DATA:0x%06X:0x%02X:", i, ROM_BYTES_PER_ROW);
        for (int j = 0; j < ROM_BYTES_PER_ROW; ++j){
            fprintf(f, " %02X", rom_bytes[i + j]);
        }
        fprintf(f, "\n");
    }
    return 0;
}

static int dump_rom_name(FILE* f, const char* rom_name){
    fprintf(f, "#: ROM name (and path)\n");
    fprintf(f, "ROM:NAME: %s\n", rom_name);
    return 0;
}

static int dump_rom_mapper(FILE* f){
    fprintf(f, "#: ROM mapper status\n");
    fprintf(f, "ROM:MAPPER:SEGA:SLOT0: %02X\n", romdbg_get_slot(0));
    fprintf(f, "ROM:MAPPER:SEGA:SLOT1: %02X\n", romdbg_get_slot(1));
    fprintf(f, "ROM:MAPPER:SEGA:SLOT2: %02X\n", romdbg_get_slot(2));
    fprintf(f, "ROM:MAPPER:SEGA:RAM: %02X\n", romdbg_get_slot(3));
    return 0;
}

static int dump_io(FILE* f){
    fprintf(f, "#: IO select state\n");
    fprintf(f, "IO: %02X\n", io_stat);
    return 0;
}

static int dump_peripheral(FILE* f){
    fprintf(f, "#: Peripheral state\n");
    fprintf(f, "PERIPHERAL:CONTROL: %02X\n", perdbg_reg_control());
    fprintf(f, "PERIPHERAL:AB: %02X\n", perdbg_reg_ab());
    fprintf(f, "PERIPHERAL:BM: %02X\n", perdbg_reg_bm());
    return 0;
}

int ss_save(FILE* f){
    dump_ram(f);

    dump_rom_name(f, "SAMPLE.SMS");
    dump_rom_mapper(f);
    dump_rom(f, ROM_MAX_SIZE);
    
    dump_io(f);
    
    dump_peripheral(f);

    return 0;
}

int ss_restore(FILE* f){
    return 0;
}
