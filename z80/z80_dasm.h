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
* @file z80_dasm.h
* @brief Provides functions to decode z80 opcodes into human-readable text.
*
*/
#ifndef __Z80_DASM
#define __Z80_DASM

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Contains all the info regarding an opcode at a given address.
struct z80d_opcode_s{
    char     opcode_str[32]; ///<-- Mnemonic representing the opcode
    uint8_t  opcode[4];      ///<-- The opcode bytes
    uint8_t  size;           ///<-- Opcode size
    uint16_t address;        ///<-- Opcode address

    ///Pointer to an opcode at the same address (if bank switching is used)
    struct z80d_opcode_s *next;
};
typedef struct z80d_opcode_s z80d_opcode;

///Binary tree of z80d_opcode
struct z80d_opcode_tree_s{
    struct z80d_opcode_s      *node;
    struct z80d_opcode_tree_s *left;
    struct z80d_opcode_tree_s *right;
};
typedef struct z80d_opcode_tree_s z80d_optree;

///List header of z80d_opcode
struct z80d_opcode_list_s{
    struct z80d_opcode_s* head;
};
typedef struct z80d_opcode_tree_s z80d_oplist;

/**
* Decodes an opcode, writing a sring representing the opcode.
* 
* @param opcode A 4-byte array containing the opcode.
* @param size   maximum size of the result string.
* @param result pointer to the string where the result will be written.
* @returns the opcode size in bytes.
*/
int z80d_decode(const uint8_t* opcode, unsigned int size, char* result);

/**
 * Decodes an opcode, return an z80d_opcode struct.
 * 
 * @param opcode A 4-byte array containing the opcode.
 * @param pc_addr The address where the opcode was found (PC).
 * @returns A z80_opcode struct containg all the opcode info.
 */
z80d_opcode z80d_decode_op(const uint8_t* opcode, uint16_t pc_addr);

#ifdef __cplusplus
}
#endif

#endif