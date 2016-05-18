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

#include "z80_decoder.h"
#include "z80_macros.h"
#include "z80_register_lut.h"
#include "z80_opcodes.h"
#include "z80_decoder_tables.h"
#include <assert.h>

/*
Most information taken from z80.info

Decoding Z80 instructions:
http://www.z80.info/decoding.htm
*/

//Forward declaration of decoder functions
int z80_instruction_decode();

///Removes one prefix byte from opcode
void z80_instruction_unprefix(){
    z80.opcode[0] = z80.opcode[1];
    z80.opcode[1] = z80.opcode[2];
    z80.opcode[2] = z80.opcode[3];
    --z80.opcode_index;
}

/**
* @brief decode/execute a Z80 opcode.
*
* Decode a z80 opcode, if the opcode is completly read, execute it.
* If a memory read/write is needed, signal it by returning the apropriate
* Z80_STAGE value.
*
* @return Signal wether we need a fetch/read/write or reset.
*/
int z80_instruction_decode(){
    assert(z80.opcode_index > 0);
    assert(z80.opcode_index < 5);

    /* Check for 'bad' prefixes */
    if (z80.opcode_index >= 2){
        /* DDDD, DDED, DDFD */
        if (z80.opcode[0] == 0xDD){
            if ((z80.opcode[1] == 0xDD) || (z80.opcode[1] == 0xED) || (z80.opcode[1] == 0xFD)){
                z80_instruction_unprefix();
            }
        }

        /* FDFD, FDED, FDDD */
        else if (z80.opcode[0] == 0xFD){
            if ((z80.opcode[1] == 0xFD) || (z80.opcode[1] == 0xED) || (z80.opcode[1] == 0xDD)){
                z80_instruction_unprefix();
            }
        }
    }

    /* Decode */
    switch (z80.opcode[0]){
    case 0xCB:
        if (z80.opcode_index == 1){
            return Z80_STAGE_M1;
        }
        assert(op_cb[z80.opcode[1]].size);
        assert(op_cb[z80.opcode[1]].f);
        if (op_cb[z80.opcode[1]].size == z80.opcode_index){
            return op_cb[z80.opcode[1]].f();
        }
        else{
            return Z80_STAGE_M1;
        }
        break;
    case 0xED:
        if (z80.opcode_index == 1){
            return Z80_STAGE_M1;
        }
        assert(op_ed[z80.opcode[1]].size);
        assert(op_ed[z80.opcode[1]].f);
        if (op_ed[z80.opcode[1]].size == z80.opcode_index){
            return op_ed[z80.opcode[1]].f();
        }
        else{
            return Z80_STAGE_M1;
        }
        break;
    case 0xDD:
        if (z80.opcode_index == 1){
            return Z80_STAGE_M1;
        }
        if (z80.opcode[1] == 0xCB){
            /*DDCB opcodes*/
            if (z80.opcode_index != 4){
                return Z80_STAGE_M1;
            }
            assert(op_ddcb[z80.opcode[3]].f);
            assert(op_ddcb[z80.opcode[3]].size);
            return op_ddcb[z80.opcode[3]].f();
        }
        else{
            assert(op_dd[z80.opcode[1]].f);
            assert(op_dd[z80.opcode[1]].size);
            if (op_dd[z80.opcode[1]].size == z80.opcode_index){
                return op_dd[z80.opcode[1]].f();
            }
            else{
                return Z80_STAGE_M1;
            }
        }
        break;
    case 0xFD:
        if (z80.opcode_index == 1){
            return Z80_STAGE_M1;
        }
        if (z80.opcode[1] == 0xCB){
            /*FDCB opcodes*/
            if (z80.opcode_index != 4){
                return Z80_STAGE_M1;
            }
            assert(op_fdcb[z80.opcode[3]].f);
            assert(op_fdcb[z80.opcode[3]].size);
            return op_fdcb[z80.opcode[3]].f();
        }
        else{
            assert(op_fd[z80.opcode[1]].f);
            assert(op_fd[z80.opcode[1]].size);
            if (op_fd[z80.opcode[1]].size == z80.opcode_index){
                return op_fd[z80.opcode[1]].f();
            }
            else{
                return Z80_STAGE_M1;
            }
        }
        break;
    default:
        /* Unprefixed opcodes */
        assert(op_unpref[z80.opcode[0]].size);
        assert(op_unpref[z80.opcode[0]].f);
        if (op_unpref[z80.opcode[0]].size == z80.opcode_index){
            return op_unpref[z80.opcode[0]].f();
        }
        else{
            return Z80_STAGE_M1;
        }
        break;
    }

    assert(0);
    return Z80_STAGE_RESET;
}
