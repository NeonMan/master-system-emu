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
 *  @file savestate-save.c
 *  @brief Utilities to dump emulator state.
 *
 *  Savestate file format.
 *  [FIELD:]+\ <value>
 *
 *  Known formats.
 *  #: [Comment]
 *  RAM:[16bit chip address, in hex]:[Byte count]: [whitespace-separated bytes]
 *  ROM:NAME: [ROM filename]
 *  ROM:MAPPER:SEGA:SLOT[0-3]: [Register value]
 *  ROM:MAPPER:SEGA:RAM: [Register value]
 *  ROM:DATA:[24bit chip address, in hex]:[Byte count]: [whitespace-separated bytes]
 *  IO: [IO register byte]
 *  PERIPHERAL:[CONTROL AB BM]: [register byte]
 *  PSG:TONE:[0-3]: [16bit value]
 *  PSG:VOLUME:[0-3]: [byte]
 *  Z80:
 */

#include "savestate.h"
#include <ram/ram.h>
#include <rom/rom.h>
#include <io/io_externs.h>
#include <peripheral/peripheral.h>
#include <psg/psg.h>
#include <z80/z80.h>
#include <z80/z80_macros.h>

#define RAM_BYTES_PER_ROW 16
#define ROM_BYTES_PER_ROW 48

static int dump_ram(FILE* f){
    uint8_t* ram_bytes = (uint8_t*) ramdbg_get_mem();
    /*Dump in blocks all the RAM bytes*/
    fprintf(f, "#: Console RAM\n");
    for (int i = 0; i < RAM_SIZE; i = i + RAM_BYTES_PER_ROW) {
        fprintf(f, "RAM:%04X:%02X:", i, RAM_BYTES_PER_ROW);
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
        fprintf(f, "ROM:DATA:%06X:%02X:", i, ROM_BYTES_PER_ROW);
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
    fprintf(f, "ROM:MAPPER:SEGA:SLOT:0: %02X\n", *romdbg_get_slot(0));
    fprintf(f, "ROM:MAPPER:SEGA:SLOT:1: %02X\n", *romdbg_get_slot(1));
    fprintf(f, "ROM:MAPPER:SEGA:SLOT:2: %02X\n", *romdbg_get_slot(2));
    fprintf(f, "ROM:MAPPER:SEGA:RAM: %02X\n", *romdbg_get_slot(3));
    return 0;
}

static int dump_io(FILE* f){
    fprintf(f, "#: IO select state\n");
    fprintf(f, "IO: %02X\n", io_stat);
    return 0;
}

static int dump_peripheral(FILE* f){
    fprintf(f, "#: Peripheral state\n");
    fprintf(f, "PERIPHERAL:CONTROL: %02X\n", *perdbg_reg_control());
    fprintf(f, "PERIPHERAL:AB: %02X\n", *perdbg_reg_ab());
    fprintf(f, "PERIPHERAL:BM: %02X\n", *perdbg_reg_bm());
    return 0;
}

static int dump_psg(FILE* f){
    uint16_t* tones = psgdbg_get_tone();
    uint8_t* volumes = psgdbg_get_volume();
    
    fprintf(f, "#: PSG state\n");
    
    fprintf(f, "PSG:TONE:0: %04X\n", tones[0]);
    fprintf(f, "PSG:TONE:1: %04X\n", tones[1]);
    fprintf(f, "PSG:TONE:2: %04X\n", tones[2]);
    fprintf(f, "PSG:TONE:3: %04X\n", tones[3]);

    fprintf(f, "PSG:VOLUME:0: %02X\n", volumes[0]);
    fprintf(f, "PSG:VOLUME:1: %02X\n", volumes[1]);
    fprintf(f, "PSG:VOLUME:2: %02X\n", volumes[2]);
    fprintf(f, "PSG:VOLUME:3: %02X\n", volumes[3]);
    return 0;
}

static int dump_z80(FILE* f){
    struct z80_s* z80_ref = z80dbg_get_z80();
    struct z80_s z80 = *z80_ref;
    fprintf(f, "#: Z80 extern pins and buses\n");
    fprintf(f, "Z80:BUS:DATA: %02X\n", z80_data);
    fprintf(f, "Z80:BUS:ADDRESS: %04X\n", z80_address);

    fprintf(f, "Z80:PIN:RD: %02X\n", z80_n_rd);
    fprintf(f, "Z80:PIN:WR: %02X\n", z80_n_wr);
    fprintf(f, "Z80:PIN:IOREQ: %02X\n", z80_n_ioreq);
    fprintf(f, "Z80:PIN:MREQ: %02X\n", z80_n_mreq);
    fprintf(f, "Z80:PIN:RFSH: %02X\n", z80_n_rfsh);
    fprintf(f, "Z80:PIN:M1: %02X\n", z80_n_m1);

    fprintf(f, "Z80:PIN:INT: %02X\n", z80_n_int);
    fprintf(f, "Z80:PIN:NMI: %02X\n", z80_n_nmi);
    fprintf(f, "Z80:PIN:RESET: %02X\n", z80_n_reset);
    fprintf(f, "Z80:PIN:WAIT: %02X\n", z80_n_wait);

    fprintf(f, "Z80:PIN:BUSREQ: %02X\n", z80_n_busreq);
    fprintf(f, "Z80:PIN:BUSACK: %02X\n", z80_n_busack);

    fprintf(f, "#: Z80 internal state\n");
    //Registers
    fprintf(f, "Z80:A: %02X\n", Z80_A);
    fprintf(f, "Z80:F: %02X\n", Z80_F);
    fprintf(f, "Z80:B: %02X\n", Z80_B);
    fprintf(f, "Z80:C: %02X\n", Z80_C);
    fprintf(f, "Z80:D: %02X\n", Z80_D);
    fprintf(f, "Z80:E: %02X\n", Z80_E);
    fprintf(f, "Z80:H: %02X\n", Z80_H);
    fprintf(f, "Z80:L: %02X\n", Z80_L);
    fprintf(f, "Z80:AP: %02X\n", Z80_Ap);
    fprintf(f, "Z80:FP: %02X\n", Z80_Fp);
    fprintf(f, "Z80:BP: %02X\n", Z80_Bp);
    fprintf(f, "Z80:CP: %02X\n", Z80_Cp);
    fprintf(f, "Z80:DP: %02X\n", Z80_Dp);
    fprintf(f, "Z80:EP: %02X\n", Z80_Ep);
    fprintf(f, "Z80:HP: %02X\n", Z80_Hp);
    fprintf(f, "Z80:LP: %02X\n", Z80_Lp);
    fprintf(f, "Z80:IX: %02X\n", Z80_IX);
    fprintf(f, "Z80:IY: %02X\n", Z80_IY);
    fprintf(f, "Z80:I: %02X\n", Z80_I);
    fprintf(f, "Z80:R: %02X\n", Z80_R);
    fprintf(f, "Z80:SP: %02X\n", Z80_PC);
    fprintf(f, "Z80:PC: %02X\n", Z80_SP);
    //Data latch
    fprintf(f, "Z80:DATA_LATCH: %02X\n", z80.data_latch);
    //IFF
    fprintf(f, "Z80:IFF:0: %02X\n", z80.iff[0]);
    fprintf(f, "Z80:IFF:1: %02X\n", z80.iff[1]);
    //Current Opcode 
    fprintf(f, "Z80:OPCODE_INDEX: %02X\n", z80.opcode_index);
    fprintf(f, "Z80:OPCODE: %02X %02X %02X %02X\n", z80.opcode[0], z80.opcode[1], z80.opcode[2], z80.opcode[3]);
    //Stages
    fprintf(f, "Z80:STAGE: %02X\n", z80.stage);
    fprintf(f, "Z80:STAGE:TICKS:1: %02X\n", z80.m1_tick_count);
    fprintf(f, "Z80:STAGE:TICKS:2: %02X\n", z80.m2_tick_count);
    fprintf(f, "Z80:STAGE:TICKS:3: %02X\n", z80.m3_tick_count);
    //Read buffer
    fprintf(f, "Z80:READ:ADDRESS: %04X\n", z80.read_address);
    fprintf(f, "Z80:READ:BUFFER: %02X %02X\n", z80.read_buffer[0], z80.read_buffer[1]);
    fprintf(f, "Z80:READ:INDEX: %02X\n", z80.read_index);
    fprintf(f, "Z80:READ:IS_IO: %02X\n", z80.read_is_io);
    //Write buffer
    fprintf(f, "Z80:WRITE:ADDRESS: %04X\n", z80.write_address);
    fprintf(f, "Z80:WRITE:BUFFER: %02X %02X\n", z80.write_buffer[0], z80.write_buffer[1]);
    fprintf(f, "Z80:WRITE:INDEX: %02X\n", z80.write_index);
    fprintf(f, "Z80:WRITE:IS_IO: %02X\n", z80.write_is_io);
    
    return 0;
}

int ss_save(FILE* f, const char* rom_name){
    fprintf(f, "SAVESTATE:VERSION: %s\n", SAVESTATE_VERSION);
    dump_ram(f);

    dump_rom_name(f, rom_name);
    dump_rom_mapper(f);
    //dump_rom(f, ROM_MAX_SIZE);
    
    dump_io(f);
    
    dump_peripheral(f);

    dump_psg(f);

    dump_z80(f);

    fprintf(f, "END: \n");
    return 0;
}
