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
#include "debug/sms_debug.h"
#include "z80_internals.h"

///Implementation of rotations and shifts. All flags affected.
alu_result_t op_rotate_shift(uint8_t operation, uint8_t op, uint8_t flags) {
    alu_result_t rv = { 0, 0 };
    uint8_t tmp_bit;
    uint8_t tmp_carry;
    assert(operation <= Z80_SHIFT_SRL);
    switch (operation) {
    case Z80_SHIFT_RLC:
        tmp_bit = (op & (1 << 7)) ? 1 : 0;
        rv.result = (op << 1) | tmp_bit;
        rv.flags |= Z80_SETFLAG_SIGN(rv.result);
        rv.flags |= Z80_SETFLAG_ZERO(rv.result);
        rv.flags |= Z80_SETFLAG_PARITY(rv.result);
        rv.flags |= (tmp_bit) ? Z80_FLAG_CARRY : 0;
        rv.flags |= Z80_SETFLAG_UNK3(rv.result);
        rv.flags |= Z80_SETFLAG_UNK5(rv.result);
        break;
    case Z80_SHIFT_RRC:
        tmp_bit = (op & 1) ? (1<<7) : 0;
        rv.result = (op >> 1) | tmp_bit;
        rv.flags |= Z80_SETFLAG_SIGN(rv.result);
        rv.flags |= Z80_SETFLAG_ZERO(rv.result);
        rv.flags |= Z80_SETFLAG_PARITY(rv.result);
        rv.flags |= (tmp_bit) ? Z80_FLAG_CARRY : 0;
        rv.flags |= Z80_SETFLAG_UNK3(rv.result);
        rv.flags |= Z80_SETFLAG_UNK5(rv.result);
        break;
    case Z80_SHIFT_RL:
        tmp_bit = (flags & Z80_FLAG_CARRY) ? 1 : 0;
        tmp_carry = op & (1 << 7);
        rv.result = (op << 1) | tmp_bit;
        rv.flags |= Z80_SETFLAG_SIGN(rv.result);
        rv.flags |= Z80_SETFLAG_ZERO(rv.result);
        rv.flags |= Z80_SETFLAG_PARITY(rv.result);
        rv.flags |= (tmp_carry) ? Z80_FLAG_CARRY : 0;
        rv.flags |= Z80_SETFLAG_UNK3(rv.result);
        rv.flags |= Z80_SETFLAG_UNK5(rv.result);
        break;
    case Z80_SHIFT_RR:
        tmp_bit = (flags & Z80_FLAG_CARRY) ? (1<<7) : 0;
        tmp_carry = op & 1;
        rv.result = (op >> 1) | tmp_bit;
        rv.flags |= Z80_SETFLAG_SIGN(rv.result);
        rv.flags |= Z80_SETFLAG_ZERO(rv.result);
        rv.flags |= Z80_SETFLAG_PARITY(rv.result);
        rv.flags |= (tmp_carry) ? Z80_FLAG_CARRY : 0;
        rv.flags |= Z80_SETFLAG_UNK3(rv.result);
        rv.flags |= Z80_SETFLAG_UNK5(rv.result);
        break;
    case Z80_SHIFT_SLA:
        rv.result = op << 1;
        rv.flags |= Z80_SETFLAG_SIGN(rv.result);
        rv.flags |= Z80_SETFLAG_ZERO(rv.result);
        rv.flags |= Z80_SETFLAG_PARITY(rv.result);
        rv.flags |= (op & (1 << 7)) ? Z80_FLAG_CARRY : 0;
        break;
    case Z80_SHIFT_SRA:
        rv.result = (op >> 1) & 0x7f;
        rv.result = (op & (1 << 7)) ? (rv.result | (1 << 7)) : rv.result;
        rv.flags |= Z80_SETFLAG_SIGN(rv.result);
        rv.flags |= Z80_SETFLAG_ZERO(rv.result);
        rv.flags |= Z80_SETFLAG_PARITY(rv.result);
        rv.flags |= (op & 1) ? Z80_FLAG_CARRY : 0;
        break;
    case Z80_SHIFT_SLL:
        rv.result = (op << 1) | 1;
        rv.flags |= Z80_SETFLAG_SIGN(rv.result);
        rv.flags |= Z80_SETFLAG_ZERO(rv.result);
        rv.flags |= Z80_SETFLAG_PARITY(rv.result);
        rv.flags |= (op & (1 << 7)) ? Z80_FLAG_CARRY : 0;
        break;
    case Z80_SHIFT_SRL:
        rv.result = (op >> 1) & 0x7f;
        rv.flags |= Z80_SETFLAG_SIGN(rv.result);
        rv.flags |= Z80_SETFLAG_ZERO(rv.result);
        rv.flags |= Z80_SETFLAG_PARITY(rv.result);
        rv.flags |= (op & 1) ? Z80_FLAG_CARRY : 0;
        break;
    }
    return rv;
}

///rot (IX+d); Size: 4; Flags: All
int shift_rot_IXYp() {
    assert(z80.opcode_index == 4);
    Z80_OPCODE_SUBDIV;
    int8_t d = (int8_t) z80.opcode[2];
    if (z80.opcode[0] == 0xDD) {
        Z80_8BIT_READ(Z80_IX + d, 0);
        alu_result_t r = op_rotate_shift(y[3], z80.read_buffer[0], Z80_F);
        Z80_8BIT_WRITE(Z80_IX + d, 0, r.result);
        Z80_F = r.flags;
    }
    else {
        Z80_8BIT_READ(Z80_IY + d, 0);
        alu_result_t r = op_rotate_shift(y[3], z80.read_buffer[0], Z80_F);
        Z80_8BIT_WRITE(Z80_IY + d, 0, r.result);
        Z80_F = r.flags;
    }
    return Z80_STAGE_RESET;
}

///rot (HL); Size: 2; Flags: All
int shift_rot_HLp() {
    assert(z80.opcode_index == 2);
    Z80_OPCODE_SUBDIV;
    Z80_8BIT_READ(Z80_HL, 0);
    alu_result_t r = op_rotate_shift(y[3], z80.read_buffer[0], Z80_F);
    Z80_8BIT_WRITE(Z80_HL, 0, r.result);
    Z80_F = r.flags;
    return Z80_STAGE_RESET;
}

///rot r; Size: 2; Flags: All
int shift_rot_r() {
    assert(z80.opcode_index == 2);
    Z80_OPCODE_SUBDIV;
    alu_result_t r = op_rotate_shift(y[1], *(z80_r[z[1]]), Z80_F);
    *(z80_r[z[1]]) = r.result;
    Z80_F = r.flags;
    return Z80_STAGE_RESET;
}

///RRD; Size: 2; Flags: S,Z,H,P,N
int RRD() {
    assert(z80.opcode_index == 2);
    uint8_t result;
    result = Z80_A & (3 << 6);         //Keep first half nibble (hn)
    result |= (Z80_A << 4) & (3 << 4); //second hn <- fourth hn
    result |= (Z80_A >> 2) & (3 << 2); //third hn <- second hn
    result |= (Z80_A >> 2) & (3);      //fourth hn <- third hn

    Z80_F = Z80_F & Z80_FLAG_CARRY;
    Z80_F |= Z80_SETFLAG_SIGN(result);
    Z80_F |= Z80_SETFLAG_ZERO(result);
    Z80_F |= Z80_FLAG_HC;
    Z80_F |= Z80_SETFLAG_PARITY(result);
    return Z80_STAGE_RESET;
}

///RLD; Size: 2; Flags: S,Z,H,P,N
int RLD() {
    assert(z80.opcode_index == 2);
    uint8_t result;
    result = Z80_A & (3 << 6);         //Keep first half nibble.
    result |= (Z80_A << 2) & (3 << 4); //Second half nibble <- third half nibble
    result |= (Z80_A << 2) & (3 << 2); //Third half nibble <- Fourth half nibble
    result |= (Z80_A >> 4) & (3);      //Fourth half nibble <- Second half nibble

    Z80_F = Z80_F & Z80_FLAG_CARRY;
    Z80_F |= Z80_SETFLAG_SIGN(result);
    Z80_F |= Z80_SETFLAG_ZERO(result);
    Z80_F |= Z80_FLAG_HC;
    Z80_F |= Z80_SETFLAG_PARITY(result);
    return Z80_STAGE_RESET;
}

