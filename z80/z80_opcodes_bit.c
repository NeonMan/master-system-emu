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

#include "z80_opcodes.h"
#include "z80_macros.h"
#include "z80_register_lut.h"
#include <assert.h>

static alu_result_t bit_op(uint8_t in_byte, uint8_t bit_index, uint8_t flags) {
    assert(bit_index < 8);
    alu_result_t rv = { 0, 0 };
    rv.flags |= (in_byte & (1 << bit_index)) ? 0 : Z80_FLAG_ZERO;
    rv.flags |= Z80_FLAG_HC;
    rv.flags |= flags & Z80_FLAG_CARRY;
    rv.result = in_byte;
    return rv;
}

static uint8_t set_op(uint8_t in_byte, uint8_t bit_index) {
    assert(bit_index < 8);
    return (in_byte | (1 << bit_index));
}

static uint8_t res_op(uint8_t in_byte, uint8_t bit_index) {
    assert(bit_index < 8);
    return (in_byte & ((1 << bit_index) ^ 0xFF));
}

static uint8_t tmp_result;

///BIT y, (HL); Size: 2; Flags: ???
int BIT_b_HLp() {
    assert(z80.opcode_index == 2);
    Z80_8BIT_READ(Z80_HL, 0);
    Z80_OPCODE_SUBDIV;
    alu_result_t r = bit_op(z80.read_buffer[0], y[1], Z80_F);
    Z80_F = r.flags;
    return Z80_STAGE_RESET;
}

///BIT y,r[z]; Size: 2; Flags: _S,Z,H,_P,N
int BIT_b_r() {
    assert(z80.opcode_index == 2);
    Z80_OPCODE_SUBDIV;
    alu_result_t r = bit_op(*(z80_r[z[1]]), y[1], Z80_F);
    Z80_F = r.flags;
    return Z80_STAGE_RESET;
}

///RES y, (HL); Size: 2; Flags: None
int RES_b_HLp() {
    assert(z80.opcode_index == 2);
    assert(0);
    return Z80_STAGE_RESET;
}

///RES y,r[z]; Size: 2; Flags: None
int RES_b_r() {
    assert(z80.opcode_index == 2);
    Z80_OPCODE_SUBDIV;
    *(z80_r[z[1]]) = res_op(*(z80_r[z[1]]), y[1]);
    return Z80_STAGE_RESET;
}

///SET y, (HL); Size: 2; Flags: None
int SET_b_HLp() {
    assert(z80.opcode_index == 2);
    assert(0);
    return Z80_STAGE_RESET;
}

///SET y, r; Size: 2; Flags: None
int SET_b_r() {
    assert(z80.opcode_index == 2);
    Z80_OPCODE_SUBDIV;
    *(z80_r[z[1]]) = set_op(*(z80_r[z[1]]), y[1]);
    return Z80_STAGE_RESET;
}

/* Opcodes using IX/IY/(IX+d)/(IY+d)*/

int SET_b_IXYp() {
    assert(z80.opcode_index == 4);
    uint16_t address;
    const int8_t displacement = (int8_t)z80.opcode[2];
    if (z80.opcode[0] == 0xDD) {
        address = Z80_IX + displacement;
    }
    else {
        address = Z80_IY + displacement;
    }
    Z80_8BIT_READ(address, 0);

    if (z80.write_index == 0) {
        Z80_OPCODE_SUBDIV;
        tmp_result = set_op(z80.read_buffer[0], y[3]);
    }
    Z80_8BIT_WRITE(address, 0, tmp_result);
    return Z80_STAGE_RESET;
}

int RES_b_IXYp() {
    assert(z80.opcode_index == 4);
    uint16_t address;
    const int8_t displacement = (int8_t)z80.opcode[2];
    if (z80.opcode[0] == 0xDD) {
        address = Z80_IX + displacement;
    }
    else {
        address = Z80_IY + displacement;
    }
    Z80_8BIT_READ(address, 0);

    if (z80.write_index == 0) {
        Z80_OPCODE_SUBDIV;
        tmp_result = res_op(z80.read_buffer[0], y[3]);
    }
    Z80_8BIT_WRITE(address, 0, tmp_result);
    return Z80_STAGE_RESET;
}

int BIT_b_IXYp() {
    assert(z80.opcode_index == 4);
    uint16_t address;
    const int8_t displacement = (int8_t)z80.opcode[2];
    if (z80.opcode[0] == 0xDD) {
        address = Z80_IX + displacement;
    }
    else {
        address = Z80_IY + displacement;
    }
    Z80_8BIT_READ(address, 0);

    Z80_OPCODE_SUBDIV;
    Z80_F = bit_op(z80.read_buffer[0], y[3], Z80_F).flags;
    return Z80_STAGE_RESET;
}