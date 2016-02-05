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
 *  @file savestate-restore.c
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

#define TOKEN_RAM        "RAM:"
#define TOKEN_ROM        "ROM:"
#define TOKEN_IO         "IO:"
#define TOKEN_PERIPHERAL "PERIPHERAL:"
#define TOKEN_PSG        "PSG:"
#define TOKEN_Z80        "Z80:"
#define TOKEN_SAVESTATE  "SAVESTATE:"
#define TOKEN_END        "END:"
#define TOKEN_COMMENT    "#"

//Peripheral tokens
#define TOKEN_CONTROL "CONTROL:"
#define TOKEN_AB      "AB:"
#define TOKEN_BM      "BM:"

//PSG tokens
#define TOKEN_TONE   "TONE:"
#define TOKEN_VOLUME "VOLUME:"

//ROM tokens
#define TOKEN_DATA "DATA:"
#define TOKEN_NAME "NAME:"
#define TOKEN_MAPPER_SEGA "MAPPER:SEGA:"
#define TOKEN_SLOT "SLOT:"

//Z80 tokens
#define TOKEN_A "A:"
#define TOKEN_F "F:"
#define TOKEN_B "B:"
#define TOKEN_C "C:"
#define TOKEN_D "D:"
#define TOKEN_E "E:"
#define TOKEN_H "H:"
#define TOKEN_L "L:"
#define TOKEN_AP "AP:"
#define TOKEN_FP "FP:"
#define TOKEN_BP "BP:"
#define TOKEN_CP "CP:"
#define TOKEN_DP "DP:"
#define TOKEN_EP "EP:"
#define TOKEN_HP "HP:"
#define TOKEN_LP "LP:"
#define TOKEN_IX "IX:"
#define TOKEN_IY "IY:"
#define TOKEN_PC "PC:"
#define TOKEN_SP "SP:"
#define TOKEN_I "I:"
#define TOKEN_R "R:"

#define TOKEN_PIN "PIN:"
#define TOKEN_ADDRESS "ADDRESS:"
#define TOKEN_RD "RD:"
#define TOKEN_WR "WR:"
#define TOKEN_MREQ "MREQ:"
#define TOKEN_IOREQ "IOREQ:"
#define TOKEN_M1 "M1:"
#define TOKEN_RFSH "RFSH:"

#define TOKEN_INT "INT:"
#define TOKEN_NMI "NMI:"
#define TOKEN_RESET "RESET:"
#define TOKEN_WAIT "WAIT:"

#define TOKEN_BUSREQ "BUSREQ:"
#define TOKEN_BUSACK "BUSACK:"

#define TOKEN_BUS "BUS:"

#define TOKEN_DATA_LATCH "DATA_LATCH:"
#define TOKEN_IFF "IFF:"
#define TOKEN_OPCODE "OPCODE:"
#define TOKEN_OPCODE_INDEX "OPCODE_INDEX:"
#define TOKEN_STAGE "STAGE:"
#define TOKEN_TICKS "TICKS:"
#define TOKEN_READ "READ:"
#define TOKEN_WRITE "WRITE:"
#define TOKEN_ADDRESS "ADDRESS:"
#define TOKEN_BUFFER "BUFFER:"
#define TOKEN_IS_IO "IS_IO:"
#define TOKEN_INDEX "INDEX:"


static const char* starts_with(const char* prefix, const char* str){
    size_t prefix_len = strlen(prefix);
    size_t str_len = strlen(str);

    if (prefix_len > str_len){
        return 0;
    }
    else{
        if (strncmp(prefix, str, prefix_len) == 0){
            return str + prefix_len;
        }
        else{
            return 0;
        }
    }
}

// <hex> = [0-9A-F]*
//       ;
static const char* parse_hex(const char* line, uint32_t* result){
    unsigned char c = (unsigned char) *line;
    uint32_t tmp_result = 0;
    while (((c >= '0') && (c <= '9')) || ((c >= 'A') && (c <= 'F'))){
        tmp_result = tmp_result * 16;
        if ((c >= '0') && (c <= '9')){
            tmp_result = tmp_result + (c - '0');
        }
        else{
            tmp_result = tmp_result + (c - 'A' + 10);
        }
        ++line;
        c = *line;
    }
    *result = tmp_result;
    return line;
}

// <byte_array> = <hex> ' ' <byte_array>
//              | <hex>
//              ;
static const char* parse_byte_array(const char* line, uint8_t* result){
    int index = 0;
    uint32_t b;
    const char* substr = line;

    while (((*substr >= '0') && (*substr <= '9')) || ((*substr >= 'A') && (*substr <= 'F'))){
        substr = parse_hex(substr, &b);
        result[index] = (uint8_t)(b & 0xFF);
        ++index;
        if (substr[0] == ' '){
            //Skip space, iterate again
            ++substr;
        }
    }
    return substr;
}

// <ram_tail> = <hex> ':' <hex> ': ' <byte_array>
//            ;
static const char* parse_ram_tail(const char* line){
    uint32_t address;
    uint32_t count;
    uint8_t bytes[256];
    const char* substr;

    //Get address
    if ( (substr = parse_hex(line, &address)) ){
        if (substr[0] == ':'){
            ++substr;
        }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }

    //Get count
    if ( (substr = parse_hex(substr, &count)) ){
        //Count is hard limited to 256
        count = (count > 256) ? 256 : count;
        if ((substr[0] == ':') && (substr[1] == ' ')){
            substr += 2;
        }
        else{
            return 0;
        }
    }

    //Parse byte array
    substr = parse_byte_array(substr, bytes);

    //Poke Z80 ram
    uint8_t* ram_bytes = ramdbg_get_mem();
    for (uint32_t i = 0; i < count; ++i){
        if ((address + i) < RAM_SIZE){
            ram_bytes[address + i] = bytes[i];
        }
    }

    return substr;
}

// <io_tail> = ' ' <hex>
//           ;
static const char* parse_io_tail(const char* line){
    uint32_t b;
    if (*line == ' ') ++line; else return 0;
    //Get IO register value
    const char* substr = parse_hex(line, &b);
    //Poke IO register
    io_stat = (uint8_t)(b & 0xFF);
    return substr;
}

// <peripheral_tail> = 'CONTROL: ' <hex>
//                   | 'AB: ' <hex>
//                   | 'BM: ' <hex>
//                   ;
static const char* parse_peripheral_tail(const char* line){
    const char* substr;
    uint32_t b;
    if ( (substr = starts_with(TOKEN_CONTROL, line)) ){
        if (*substr == ' ') ++substr; else return 0;
        substr = parse_hex(substr, &b);
        *perdbg_reg_control() = (uint8_t)b;
    }
    else if ( (substr = starts_with(TOKEN_AB, line)) ){
        if (*substr == ' ') ++substr; else return 0;
        substr = parse_hex(substr, &b);
        *perdbg_reg_ab() = (uint8_t)b;
    }
    else if ( (substr = starts_with(TOKEN_BM, line)) ){
        if (*substr == ' ') ++substr; else return 0;
        substr = parse_hex(substr, &b);
        *perdbg_reg_bm() = (uint8_t)b;
    }
    else{
        return 0;
    }
    return substr;
}

// <psg_tail> = 'TONE:' <hex> ': ' <hex>
//            | 'VOLUME:' <hex> ': ' <hex>
//            ;
static const char* parse_psg_tail(const char* line){
    const char* substr;
    uint32_t register_index;
    uint32_t register_value;

    if ( (substr = starts_with(TOKEN_TONE, line)) ){
        substr = parse_hex(substr, &register_index);
        if (*substr == ':') ++substr; else return 0;
        if (*substr == ' ') ++substr; else return 0;
        substr = parse_hex(substr, &register_value);
        //Poke register
        if (register_index < 4){
            psgdbg_get_tone()[register_index] = (uint16_t)register_value;
        }
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_VOLUME, line)) ){
        substr = parse_hex(substr, &register_index);
        if (*substr == ':') ++substr; else return 0;
        if (*substr == ' ') ++substr; else return 0;
        substr = parse_hex(substr, &register_value);
        //Poke register
        if (register_index < 4){
            psgdbg_get_volume()[register_index] = (int8_t)((uint8_t)register_value);
        }
        return substr;
    }
    else{
        return 0;
    }
}

// <rom_data> = <hex> ':' <hex> ': ' <hex_array>
//            ;
static const char* parse_rom_data(const char* line){
    uint32_t address;
    uint32_t count;
    uint8_t bytes[256];
    const char* substr;

    substr = parse_hex(line, &address);
    if (*substr == ':') ++substr; else return 0;
    substr = parse_hex(substr, &count);
    if (*substr == ':') ++substr; else return 0;
    if (*substr == ' ') ++substr; else return 0;
    substr = parse_byte_array(substr, bytes);

    //Poke ROM bytes
    uint8_t* rom_bytes = romdbg_get_rom();
    for (uint32_t i = 0; i < count; ++i){
        if ((address + i) < ROM_MAX_SIZE){
            rom_bytes[address + i] = bytes[i];
        }
    }
    return substr;
}

// <rom_mapper_sega> = 'SLOT:' <hex> ': ' <hex>
//                   | 'RAM:' ' ' <hex>
//                   ;
static const char* parse_rom_mapper_sega(const char* line){
    uint32_t slot_index;
    uint32_t reg_value;
    const char* substr;
    if ( (substr = starts_with(TOKEN_SLOT, line)) ){
        substr = parse_hex(substr, &slot_index);
        if (*substr == ':') ++substr; else return 0;
        if (*substr == ' ') ++substr; else return 0;
        substr = parse_hex(substr, &reg_value);
        //Poke register
        if (slot_index < 3){
            *romdbg_get_slot(slot_index) = (uint8_t) reg_value;
        }
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_RAM, line)) ){
        if (*substr == ' ') ++substr; else return 0;
        substr = parse_hex(substr, &reg_value);
        *romdbg_get_slot(3) = (uint8_t) reg_value;
        return substr;
    }
    return 0;
}

// <rom_name> = ' ' <string>
//            ;
static const char* parse_rom_name(const char* line){
    char result[256];
    if (*line == ' ') ++line; else return 0;
    size_t path_len = strlen(line);
    path_len = (path_len > 0) ? path_len - 1 : 0;
    strncpy(result, line, path_len);
    result[path_len] = '\0';

    if (rom_load_file(result) == 0){
        return line + path_len;
    }
    else{
        return 0;
    }
}

// <rom_tail> = <rom_data>
//            | <rom_name>
//            | <rom_mapper_sega>
//            ;
static const char* parse_rom_tail(const char* line){
    const char* substr;
    if ( (substr = starts_with(TOKEN_DATA, line)) ){
        return parse_rom_data(substr);
    }
    else if ( (substr = starts_with(TOKEN_NAME, line)) ){
        return substr;
        //return parse_rom_name(substr);
    }
    else if ( (substr = starts_with(TOKEN_MAPPER_SEGA, line)) ){
        return parse_rom_mapper_sega(substr);
    }
    else{
        return 0;
    }
}

// <z80_tail> = BUS:DATA: <hex>
//            | BUS:ADDRESS: <hex>
//            | PIN:RD: <hex>
//            | PIN:RD: <hex>
//            | PIN:WR: <hex>
//            | PIN:IOREQ: <hex>
//            | PIN:MREQ: <hex>
//            | PIN:RFSH: <hex>
//            | PIN:M1: <hex>
//            | PIN:INT: <hex>
//            | PIN:NMI: <hex>
//            | PIN:RESET: <hex>
//            | PIN:WAIT: <hex>
//            | PIN:BUSREQ: <hex>
//            | PIN:BUSACK: <hex>
//            | A: <hex>
//            | F: <hex>
//            | B: <hex>
//            | C: <hex>
//            | D: <hex>
//            | E: <hex>
//            | H: <hex>
//            | L: <hex>
//            | AP: <hex>
//            | FP: <hex>
//            | BP: <hex>
//            | CP: <hex>
//            | DP: <hex>
//            | EP: <hex>
//            | HP: <hex>
//            | LP: <hex>
//            | I: <hex>
//            | R: <hex>
//            | SP: <hex>
//            | PC: <hex>
//            | DATA_LATCH: <hex>
//            | IFF:<hex>: <hex>
//            | OPCODE_INDEX: <hex>
//            | OPCODE: <byte_array>
//            | STAGE:TICKS:<hex>: <hex>
//            | STAGE: <hex>
//            | READ:ADDRESS: <hex>
//            | READ:INDEX: <hex>
//            | READ:IS_IO: <hex>
//            | WRITE:ADDRESS: <hex>
//            | WRITE:INDEX: <hex>
//            | WRITE:IS_IO: <hex>
//            | READ:BUFFER: <byte_array>
//            | WRITE:BUFFER: <byte_array>
//            ;
static const char* parse_z80_tail(const char* line){
    const char* substr;
    uint32_t value;
    struct z80_s* z80_ref = z80dbg_get_z80();
    struct z80_s  z80 = *z80_ref;
    uint8_t bytes[256];

    if ( (substr = starts_with(TOKEN_BUS TOKEN_ADDRESS " ", line)) ){
        substr = parse_hex(substr, &value);
        z80_address = (uint16_t)value;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_BUS TOKEN_DATA " ", line)) ){
        substr = parse_hex(substr, &value);
        z80_data = (uint8_t)value;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_PIN TOKEN_RD " ", line)) ){
        substr = parse_hex(substr, &value);
        z80_n_rd = (uint8_t)value;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_PIN TOKEN_WR " ", line)) ){
        substr = parse_hex(substr, &value);
        z80_n_wr = (uint8_t)value;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_PIN TOKEN_IOREQ " ", line)) ){
        substr = parse_hex(substr, &value);
        z80_n_ioreq = (uint8_t)value;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_PIN TOKEN_MREQ " ", line)) ){
        substr = parse_hex(substr, &value);
        z80_n_mreq = (uint8_t)value;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_PIN TOKEN_RFSH " ", line)) ){
        substr = parse_hex(substr, &value);
        z80_n_rfsh = (uint8_t)value;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_PIN TOKEN_M1 " ", line)) ){
        substr = parse_hex(substr, &value);
        z80_n_m1 = (uint8_t)value;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_PIN TOKEN_INT " ", line)) ){
        substr = parse_hex(substr, &value);
        z80_n_int = (uint8_t)value;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_PIN TOKEN_NMI " ", line)) ){
        substr = parse_hex(substr, &value);
        z80_n_nmi = (uint8_t)value;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_PIN TOKEN_RESET " ", line)) ){
        substr = parse_hex(substr, &value);
        z80_n_reset = (uint8_t)value;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_PIN TOKEN_WAIT " ", line)) ){
        substr = parse_hex(substr, &value);
        z80_n_wait = (uint8_t)value;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_PIN TOKEN_BUSREQ " ", line)) ){
        substr = parse_hex(substr, &value);
        z80_n_busreq = (uint8_t)value;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_PIN TOKEN_BUSACK " ", line)) ){
        substr = parse_hex(substr, &value);
        z80_n_busack = (uint8_t)value;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_A " ", line)) ){
        substr = parse_hex(substr, &value);
        Z80_A = (uint8_t)value; *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_F " ", line)) ){
        substr = parse_hex(substr, &value);
        Z80_F = (uint8_t)value; *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_B " ", line)) ){
        substr = parse_hex(substr, &value);
        Z80_B = (uint8_t)value; *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_C " ", line)) ){
        substr = parse_hex(substr, &value);
        Z80_C = (uint8_t)value; *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_D " ", line)) ){
        substr = parse_hex(substr, &value);
        Z80_D = (uint8_t)value; *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_E " ", line)) ){
        substr = parse_hex(substr, &value);
        Z80_E = (uint8_t)value; *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_H " ", line)) ){
        substr = parse_hex(substr, &value);
        Z80_H = (uint8_t)value; *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_L " ", line)) ){
        substr = parse_hex(substr, &value);
        Z80_L = (uint8_t)value; *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_AP " ", line)) ){
        substr = parse_hex(substr, &value);
        Z80_Ap = (uint8_t)value; *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_FP " ", line)) ){
        substr = parse_hex(substr, &value);
        Z80_Fp = (uint8_t)value; *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_BP " ", line)) ){
        substr = parse_hex(substr, &value);
        Z80_Bp = (uint8_t)value; *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_CP " ", line)) ){
        substr = parse_hex(substr, &value);
        Z80_Cp = (uint8_t)value; *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_DP " ", line)) ){
        substr = parse_hex(substr, &value);
        Z80_Dp = (uint8_t)value; *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_EP " ", line)) ){
        substr = parse_hex(substr, &value);
        Z80_Ep = (uint8_t)value; *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_HP " ", line)) ){
        substr = parse_hex(substr, &value);
        Z80_Hp = (uint8_t)value; *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_LP " ", line)) ){
        substr = parse_hex(substr, &value);
        Z80_Lp = (uint8_t)value; *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_I " ", line)) ){
        substr = parse_hex(substr, &value);
        Z80_I = (uint8_t)value; *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_R " ", line)) ){
        substr = parse_hex(substr, &value);
        Z80_R = (uint8_t)value; *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_SP " ", line)) ){
        substr = parse_hex(substr, &value);
        Z80_SP = (uint16_t)value; *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_PC " ", line)) ){
        substr = parse_hex(substr, &value);
        Z80_PC = (uint16_t)value; *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_IX " ", line)) ){
        substr = parse_hex(substr, &value);
        Z80_IX = (uint16_t)value; *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_IY " ", line)) ){
        substr = parse_hex(substr, &value);
        Z80_IY = (uint16_t)value; *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_DATA_LATCH " ", line)) ){
        substr = parse_hex(substr, &value);
        z80.data_latch = (uint8_t)value; *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_IFF, line)) ){
        uint32_t iff_index;
        substr = parse_hex(substr, &iff_index);
        if (*substr == ':') ++substr; else return 0;
        if (*substr == ' ') ++substr; else return 0;
        substr = parse_hex(substr, &value);
        if (iff_index < 2){
            z80.iff[iff_index] = (uint8_t)value;
        }
        *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_OPCODE_INDEX " ", line)) ){
        substr = parse_hex(substr, &value);
        z80.opcode_index = (uint8_t)value; *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_STAGE " ", line)) ){
        substr = parse_hex(substr, &value);
        z80.stage = (uint8_t)value; *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_STAGE TOKEN_TICKS, line)) ){
        uint32_t stage;
        substr = parse_hex(substr, &stage);
        if (*substr == ':') ++substr; else return 0;
        if (*substr == ' ') ++substr; else return 0;
        substr = parse_hex(substr, &value);
        switch (stage){
        case Z80_STAGE_M1:
            z80.m1_tick_count = (uint16_t)value; *z80_ref = z80;
            return substr;
        case Z80_STAGE_M2:
            z80.m2_tick_count = (uint16_t)value; *z80_ref = z80;
            return substr;
        case Z80_STAGE_M3:
            z80.m3_tick_count = (uint16_t)value; *z80_ref = z80;
            return substr;
        default:
            return 0;
        }
    }
    else if ( (substr = starts_with(TOKEN_OPCODE " ", line)) ){
        substr = parse_byte_array(substr, bytes);
        for (int i = 0; i < 4; ++i){
            z80.opcode[i] = bytes[i];
        }
        *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_READ TOKEN_ADDRESS " ", line)) ){
        substr = parse_hex(substr, &value);
        z80.read_address = (uint16_t)value;
        *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_READ TOKEN_BUFFER " ", line)) ){
        substr = parse_byte_array(substr, bytes);
        z80.read_buffer[0] = bytes[0];
        z80.read_buffer[1] = bytes[1];
        *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_READ TOKEN_INDEX " ", line)) ){
        substr = parse_hex(substr, &value);
        z80.read_index = (uint8_t)value;
        *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_READ TOKEN_IS_IO " ", line)) ){
        substr = parse_hex(substr, &value);
        z80.read_is_io = (uint8_t)value;
        *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_WRITE TOKEN_ADDRESS " ", line)) ){
        substr = parse_hex(substr, &value);
        z80.write_address = (uint16_t)value;
        *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_WRITE TOKEN_BUFFER " ", line)) ){
        substr = parse_byte_array(substr, bytes);
        z80.write_buffer[0] = bytes[0];
        z80.write_buffer[1] = bytes[1];
        *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_WRITE TOKEN_INDEX " ", line)) ){
        substr = parse_hex(substr, &value);
        z80.write_index = (uint8_t)value;
        *z80_ref = z80;
        return substr;
    }
    else if ( (substr = starts_with(TOKEN_WRITE TOKEN_IS_IO " ", line)) ){
        substr = parse_hex(substr, &value);
        z80.write_is_io = (uint8_t)value;
        *z80_ref = z80;
        return substr;
    }

    else{
        return 0;
    }
}

// <line> = 'RAM:' <ram_tail>
//        | 'ROM:' <rom_tail>
//        | 'IO:'  <io_tail>
//        | 'PERIPHERAL:' <peripheral_tail>
//        | 'PSG:' <psg_tail>
//        | 'Z80:' <z80_tail>
//        | 'SAVESTATE:' <savestate_tail>
//        | 'END:' .*
//        | '#' .*
//        ;
static const char* parse_line(const char* line){
    const char* substr;
    if ( (substr = starts_with(TOKEN_RAM, line)) ){
        return parse_ram_tail(substr);
    }
    else if ( (substr = starts_with(TOKEN_ROM, line)) ){
        return parse_rom_tail(substr);
    }
    else if ( (substr = starts_with(TOKEN_IO, line)) ){
        return parse_io_tail(substr);
    }
    else if ( (substr = starts_with(TOKEN_PERIPHERAL, line)) ){
        return parse_peripheral_tail(substr);
    }
    else if ( (substr = starts_with(TOKEN_PSG, line)) ){
        return parse_psg_tail(substr);
    }
    else if ( (substr = starts_with(TOKEN_Z80, line)) ){
        return parse_z80_tail(substr);
    }
    else if ( (substr = starts_with(TOKEN_SAVESTATE, line)) ){
        ; ///@ToDo Check savestate version
    }
    else if ( (substr = starts_with(TOKEN_END, line)) ){
        ; //Do nothing
    }
    else if ( (substr = starts_with(TOKEN_COMMENT, line)) ){
        ; //Do nothing
    }
    else{
        //Warning! unexpected token!
        return 0;
    }
    return line;
}

// <savestate> = <line> '\n' <savestate>
//             | EMPTY
//             ;
int ss_restore(FILE* f){
    int rv = 0;
    static char line[SAVESTATE_LINE_BUFFER + 1];
    memset(line, 0, SAVESTATE_LINE_BUFFER + 1);

    //For each line
    while (feof(f) == 0){
        fgets(line, SAVESTATE_LINE_BUFFER, f);
        if (feof(f) == 0){
            if (parse_line(line) == 0){
                //Error!
                return -1;
                break;
            }
        }
    }
    return rv;
}
