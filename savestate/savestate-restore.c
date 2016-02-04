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

#define TOKEN_RAM        "RAM:"
#define TOKEN_ROM        "ROM:"
#define TOKEN_IO         "IO: "
#define TOKEN_PERIPHERAL "PERIPHERAL:"
#define TOKEN_PSG        "PSG:"
#define TOKEN_Z80        "Z80:"
#define TOKEN_SAVESTATE  "SAVESTATE:"
#define TOKEN_END        "END:"
#define TOKEN_COMMENT    "#"

//Peripheral tokens
#define TOKEN_CONTROL "CONTROL: "
#define TOKEN_AB "AB: "
#define TOKEN_BM "BM: "

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
    return line;
}

// <ram_tail> = <hex> ':' <hex> ': ' <byte_array>
//            ;
static const char* parse_ram_tail(const char* line){
    uint32_t address;
    uint32_t count;
    uint8_t bytes[256];
    const char* substr;

    //Get address
    if (substr = parse_hex(line, &address)){
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
    if (substr = parse_hex(substr, &count)){
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

// <io_tail> = <hex>
//           ;
static const char* parse_io_tail(const char* line){
    uint32_t b;
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
    if (substr = starts_with(TOKEN_CONTROL, line)){
        substr = parse_hex(substr, &b);
        *perdbg_reg_control() = (uint8_t)b;
    }
    else if (substr = starts_with(TOKEN_AB, line)){
        substr = parse_hex(substr, &b);
        *perdbg_reg_ab() = (uint8_t)b;
    }
    else if (substr = starts_with(TOKEN_BM, line)){
        substr = parse_hex(substr, &b);
        *perdbg_reg_bm() = (uint8_t)b;
    }
    else{
        return 0;
    }
    return substr;
}

// <line> = 'RAM:' <ram_tail>
//        | 'ROM:' <rom_tail>
//        | 'IO:'  <io_tail>
//        | 'PERIPHERAL:' <peripheral_tail>
//        | 'PSG:' <psg_tail>
//        | 'Z80:' <z80_tail>
//        | 'SAVESTATE:' <savestate_tail>
//        | '#' .*
//        ;
static const char* parse_line(const char* line){
    const char* substr;
    if (substr = starts_with(TOKEN_RAM, line)){
        return parse_ram_tail(substr);
    }
    else if (substr = starts_with(TOKEN_ROM, line)){
        printf("Unimplemented token: ROM\n");
    }
    else if (substr = starts_with(TOKEN_IO, line)){
        return parse_io_tail(substr);
    }
    else if (substr = starts_with(TOKEN_PERIPHERAL, line)){
        return parse_peripheral_tail(substr);
    }
    else if (substr = starts_with(TOKEN_PSG, line)){
        printf("Unimplemented token: PSG\n");
    }
    else if (substr = starts_with(TOKEN_Z80, line)){
        printf("Unimplemented token: Z80\n");
    }
    else if (substr = starts_with(TOKEN_SAVESTATE, line)){
        ; ///@ToDo Check savestate version
    }
    else if (substr = starts_with(TOKEN_END, line)){
        ; //Do nothing
    }
    else if (substr = starts_with(TOKEN_COMMENT, line)){
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
                break;
            }
        }
    }
    return rv;
}
