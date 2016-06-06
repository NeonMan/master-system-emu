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
//#include "debug/sms_debug.h"
#include "debug/sms_debug.h"

extern struct z80_s z80; //<-- Access to z80 internals

static alu_result_t alu_op(uint8_t operation, int8_t op1, int8_t op2, uint8_t flags) {
    assert(operation <= Z80_ALUOP_CP);
    alu_result_t rv = { 0, 0 };

    switch (operation) {
    /* --- ADD --- */
    case Z80_ALUOP_ADD:
        rv.result = op1 + op2;
        /*Flags*/
        rv.flags |= Z80_SETFLAG_CARRY(op1, rv.result);    /* Carry                     */
        /*rv.flags |= 0*/                                 /* Negative (cleared)        */
        rv.flags |= Z80_SETFLAG_OVERFLOW(op1, rv.result); /* Overflow                  */
        rv.flags |= Z80_SETFLAG_UNK3(rv.result);          /* Undoc 3 (bit 3 of result) */
        rv.flags |= Z80_SETFLAG_HALF_CARRY(op1, op2);     /* Half carry                */
        rv.flags |= Z80_SETFLAG_UNK5(rv.result);          /* Undoc 5 (bit 5 of result) */
        rv.flags |= Z80_SETFLAG_ZERO(rv.result);          /* Zero                      */
        rv.flags |= Z80_SETFLAG_SIGN(rv.result);          /* Sign                      */
        break;
    /* --- ADC --- */
    case Z80_ALUOP_ADC:
        rv.result = (flags & Z80_FLAG_CARRY) ? (op1 + op2 + 1) : (op1 + op2);
        /*Flags*/
        rv.flags |= Z80_SETFLAG_CARRY(op1, rv.result);    /* Carry                     */
        /*rv.flags |= 0*/                                 /* Negative (cleared)        */
        rv.flags |= Z80_SETFLAG_OVERFLOW(op1, rv.result); /* Overflow                  */
        rv.flags |= Z80_SETFLAG_UNK3(rv.result);          /* Undoc 3 (bit 3 of result) */
        rv.flags |= Z80_SETFLAG_HALF_CARRY(op1, op2);     /* Half carry                */
        rv.flags |= Z80_SETFLAG_UNK5(rv.result);          /* Undoc 5 (bit 5 of result) */
        rv.flags |= Z80_SETFLAG_ZERO(rv.result);          /* Zero                      */
        rv.flags |= Z80_SETFLAG_SIGN(rv.result);          /* Sign                      */
        break;
    /* --- SUB --- */
    case Z80_ALUOP_SUB:
        rv.result = op1 - op2;
        /*Flags*/
        rv.flags |= Z80_SETFLAG_BORROW(op1, rv.result);   /* Carry                     */
        rv.flags |= Z80_FLAG_SUBTRACT;                    /* Negative (set)            */
        rv.flags |= Z80_SETFLAG_OVERFLOW(op1, rv.result); /* Overflow                  */
        rv.flags |= Z80_SETFLAG_UNK3(rv.result);          /* Undoc 3 (bit 3 of result) */
        rv.flags |= Z80_SETFLAG_HALF_BORROW(op1, op2);    /* Half carry                */
        rv.flags |= Z80_SETFLAG_UNK5(rv.result);          /* Undoc 5 (bit 5 of result) */
        rv.flags |= Z80_SETFLAG_ZERO(rv.result);          /* Zero                      */
        rv.flags |= Z80_SETFLAG_SIGN(rv.result);          /* Sign                      */
        break;
    /* --- SBC --- */
    case Z80_ALUOP_SBC:
        rv.result = (flags & Z80_FLAG_CARRY) ? (op1 - op2 + 1) : (op1 - op2);
        /*Flags*/
        rv.flags |= Z80_SETFLAG_BORROW(op1, rv.result);   /* Carry                     */
        rv.flags |= Z80_FLAG_SUBTRACT;                    /* Negative (set)            */
        rv.flags |= Z80_SETFLAG_OVERFLOW(op1, rv.result); /* Overflow                  */
        rv.flags |= Z80_SETFLAG_UNK3(rv.result);          /* Undoc 3 (bit 3 of result) */
        rv.flags |= Z80_SETFLAG_HALF_BORROW(op1, op2);    /* Half carry                */
        rv.flags |= Z80_SETFLAG_UNK5(rv.result);          /* Undoc 5 (bit 5 of result) */
        rv.flags |= Z80_SETFLAG_ZERO(rv.result);          /* Zero                      */
        rv.flags |= Z80_SETFLAG_SIGN(rv.result);          /* Sign                      */
        break;
    /* --- AND --- */
    case Z80_ALUOP_AND:
        rv.result = op1 & op2;
        /*Flags*/
        /*rv.flags |= 0;*/                                /* Carry (cleared)           */
        /*rv.flags |= 0;*/                                /* Negative (cleared)        */
        rv.flags |= Z80_SETFLAG_PARITY(rv.result);        /* Parity                    */
        rv.flags |= Z80_SETFLAG_UNK3(rv.result);          /* Undoc 3 (bit 3 of result) */
        rv.flags |= Z80_FLAG_HC;                          /* Half carry (set)          */
        rv.flags |= Z80_SETFLAG_UNK5(rv.result);          /* Undoc 5 (bit 5 of result) */
        rv.flags |= Z80_SETFLAG_ZERO(rv.result);          /* Zero                      */
        rv.flags |= Z80_SETFLAG_SIGN(rv.result);          /* Sign                      */
        break;
    /* --- XOR --- */
    case Z80_ALUOP_XOR:
        rv.result = op1 ^ op2;
        /*Flags*/
        /*rv.flags |= 0;*/                                /* Carry (cleared)           */
        /*rv.flags |= 0;*/                                /* Negative (cleared)        */
        rv.flags |= Z80_SETFLAG_PARITY(rv.result);        /* Parity                    */
        rv.flags |= Z80_SETFLAG_UNK3(rv.result);          /* Undoc 3 (bit 3 of result) */
        /*rv.flags |= 0;*/                                /* Half carry (cleared)      */
        rv.flags |= Z80_SETFLAG_UNK5(rv.result);          /* Undoc 5 (bit 5 of result) */
        rv.flags |= Z80_SETFLAG_ZERO(rv.result);          /* Zero                      */
        rv.flags |= Z80_SETFLAG_SIGN(rv.result);          /* Sign                      */
        break;
    /* --- OR --- */
    case Z80_ALUOP_OR:
        rv.result = op1 | op2;
        /*Flags*/
        /*rv.flags |= 0;*/                                /* Carry (cleared)           */
        /*rv.flags |= 0;*/                                /* Negative (cleared)        */
        rv.flags |= Z80_SETFLAG_PARITY(rv.result);        /* Parity                    */
        rv.flags |= Z80_SETFLAG_UNK3(rv.result);          /* Undoc 3 (bit 3 of result) */
        /*rv.flags |= 0;*/                                /* Half carry (cleared)      */
        rv.flags |= Z80_SETFLAG_UNK5(rv.result);          /* Undoc 5 (bit 5 of result) */
        rv.flags |= Z80_SETFLAG_ZERO(rv.result);          /* Zero                      */
        rv.flags |= Z80_SETFLAG_SIGN(rv.result);          /* Sign                      */
        break;
    /* --- CP --- */
    case Z80_ALUOP_CP:
        /*Flags*/
        rv.flags |= Z80_SETFLAG_BORROW(op1, op1 - op2);   /* Carry                     */
        rv.flags |= Z80_FLAG_SUBTRACT;                    /* Negative (set)            */
        rv.flags |= Z80_SETFLAG_OVERFLOW(op1, op1 - op2); /* Overflow                  */
        rv.flags |= Z80_SETFLAG_UNK3(op2);                /* Undoc 3 (bit 3 of result) */
        rv.flags |= Z80_SETFLAG_HALF_BORROW(op1, op2);    /* Half carry                */
        rv.flags |= Z80_SETFLAG_UNK5(op2);                /* Undoc 5 (bit 5 of result) */
        rv.flags |= Z80_SETFLAG_ZERO(op1 - op2);          /* Zero                      */
        rv.flags |= Z80_SETFLAG_SIGN(op1 - op2);          /* Sign                      */
        rv.result = op1;
        break;
    }

    return rv;
}

static alu_result_t op_inc(uint8_t op, uint8_t flags) {
    alu_result_t rv = { 0,0 };
    rv.result = op++;
    rv.flags |= Z80_SETFLAG_SIGN(rv.result);
    rv.flags |= Z80_SETFLAG_ZERO(rv.result);
    rv.flags |= Z80_SETFLAG_UNK3(rv.result);
    rv.flags |= Z80_SETFLAG_HALF_CARRY(op, rv.result); ///<-- @bug This is probably wrong AF.
    rv.flags |= Z80_SETFLAG_UNK5(rv.result);
    rv.flags |= (op == 0x7f) ? Z80_FLAG_PARITY : 0;
    //N is reset
    rv.flags |= flags & Z80_FLAG_CARRY;
    return rv;
}

static alu_result_t op_dec(uint8_t op, uint8_t flags) {
    alu_result_t rv = { 0,0 };
    rv.result = op++;
    rv.flags |= Z80_SETFLAG_SIGN(rv.result);
    rv.flags |= Z80_SETFLAG_ZERO(rv.result);
    rv.flags |= Z80_SETFLAG_UNK3(rv.result);
    rv.flags |= Z80_SETFLAG_HALF_BORROW(op, rv.result); ///<-- @bug This is probably wrong AF.
    rv.flags |= Z80_SETFLAG_UNK5(rv.result);
    rv.flags |= (op == 0x80) ? Z80_FLAG_PARITY : 0;
    rv.flags |= Z80_FLAG_SUBTRACT;
    rv.flags |= flags & Z80_FLAG_CARRY;
    return rv;
}

///alu(HL); Size: 1; Flags: All
int alu8_HLp() {
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    const uint8_t alu_operation = y[0];
    Z80_8BIT_READ(Z80_HL, 0);
    alu_result_t r = alu_op(alu_operation, Z80_A, z80.read_buffer[0], Z80_F);
    Z80_A = r.result;
    Z80_F = r.flags;
    return Z80_STAGE_RESET;
}

///alu n; Size: 2; Flags: All
int alu8_n() {
    assert(z80.opcode_index == 2);
    Z80_OPCODE_SUBDIV;
    const uint8_t alu_operation = y[0];
    alu_result_t r = alu_op(alu_operation, Z80_A, z80.opcode[1], Z80_F);
    Z80_A = r.result;
    Z80_F = r.flags;
    return Z80_STAGE_RESET;
}

///alu r; Size: 1; Flags: All
int alu8_r() {
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    assert(z[0] != Z80_R_INDEX_HL);
    const uint8_t alu_operation = y[0];
    alu_result_t r = alu_op(alu_operation, Z80_A, *(z80_r[z[0]]), Z80_F);
    Z80_A = r.result;
    Z80_F = r.flags;
    return Z80_STAGE_RESET;
}

int alu8_r_undoc() {
    assert(z80.opcode_index == 2);
    Z80_OPCODE_SUBDIV;
    assert(z[1] != Z80_R_INDEX_HL);
    const uint8_t alu_operation = y[1];
    alu_result_t r;
    if (z80.opcode[0] == 0xDD) {
        r = alu_op(alu_operation, Z80_A, *(z80_r_ix[z[1]]), Z80_F);
    }
    else {
        r = alu_op(alu_operation, Z80_A, *(z80_r_iy[z[1]]), Z80_F);
    }
    Z80_A = r.result;
    Z80_F = r.flags;
    return Z80_STAGE_RESET;
}

int alu8_IXYp() {
    assert(z80.opcode_index == 3);
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
    const uint8_t alu_operation = y[1];
    alu_result_t r = alu_op(alu_operation, Z80_A, z80.read_buffer[0], Z80_F);
    Z80_A = r.result;
    Z80_F = r.flags;
    return Z80_STAGE_RESET;
}

///DEC(r[y]); Size:1; Flags: S,Z,H,P,N
int DEC_r() {
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    const uint8_t old_r = *z80_r[y[0]];
    --(*(z80_r[y[0]]));
    Z80_F = (Z80_F & (
        Z80_CLRFLAG_SIGN & Z80_CLRFLAG_ZERO & Z80_CLRFLAG_HC
        & Z80_CLRFLAG_PARITY & Z80_CLRFLAG_SUBTRACT)
        )  //Clear S,Z,H,P,N (7,6,4,2,1) ***V0-
        | Z80_SETFLAG_SIGN(*z80_r[y[0]])
        | Z80_SETFLAG_ZERO(*z80_r[y[0]])
        | Z80_SETFLAG_HALF_BORROW(old_r, 1)
        | Z80_SETFLAG_OVERFLOW(*z80_r[y[0]], old_r);
    return Z80_STAGE_RESET;
}

int DEC_rIXY() {
    assert(z80.opcode_index == 2);
    Z80_OPCODE_SUBDIV;
    alu_result_t r;
    if (z80.opcode[0] == 0xDD) {
        assert(z80_r_ix[y[1]]);
        r = op_dec(*(z80_r_ix[y[1]]), Z80_F);
        *(z80_r_ix[y[1]]) = r.result;
    }
    else {
        assert(z80_r_iy[y[1]]);
        r = op_dec(*(z80_r_iy[y[1]]), Z80_F);
        *(z80_r_iy[y[1]]) = r.result;
    }
    Z80_F = r.flags;
    return Z80_STAGE_RESET;
}

///DEC (HL); Size: 1; Flags: ???
int DEC_HLp() {
    assert(z80.opcode_index == 1);
    Z80_8BIT_READ(Z80_HL, 0);
    alu_result_t r = op_dec(z80.read_buffer[0], Z80_F);
    Z80_8BIT_WRITE(Z80_HL, 0, r.result);
    Z80_F = r.flags;
    return Z80_STAGE_RESET;
}

///INC r; Size: 1; Flags: All
int INC_r() {
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;

    uint8_t old_r = *z80_r[y[0]];
    ++(*(z80_r[y[0]]));
    Z80_F = (Z80_F & (
        Z80_CLRFLAG_SIGN & Z80_CLRFLAG_ZERO & Z80_CLRFLAG_HC
        & Z80_CLRFLAG_PARITY & Z80_CLRFLAG_SUBTRACT)
        )  //Clear S,Z,H,P,N (7,6,4,2,1) ***V0-
        | Z80_SETFLAG_SIGN(*z80_r[y[0]])
        | Z80_SETFLAG_ZERO(*z80_r[y[0]])
        | Z80_SETFLAG_HALF_CARRY(old_r, 1)
        | Z80_SETFLAG_OVERFLOW(old_r, *z80_r[y[0]]);
    return Z80_STAGE_RESET;
}

int INC_rIXY() {
    assert(z80.opcode_index == 2);
    Z80_OPCODE_SUBDIV;
    alu_result_t r;
    if (z80.opcode[0] == 0xDD) {
        assert(z80_r_ix[y[1]]);
        r = op_inc(*(z80_r_ix[y[1]]), Z80_F);
        *(z80_r_ix[y[1]]) = r.result;
    }
    else {
        assert(z80_r_iy[y[1]]);
        r = op_inc(*(z80_r_iy[y[1]]), Z80_F);
        *(z80_r_iy[y[1]]) = r.result;
    }
    Z80_F = r.flags;
    return Z80_STAGE_RESET;
}

///INC (HL); Size: 1; Flags: All 
int INC_HLp() {
    assert(z80.opcode_index == 1);
    //Memory read
    Z80_8BIT_READ(Z80_HL, 0);
    //Mem write
    Z80_8BIT_WRITE(Z80_HL, 0, z80.read_buffer[0] + 1);

    const uint8_t old_r = z80.read_buffer[0];
    Z80_F = (Z80_F & (
        Z80_CLRFLAG_SIGN & Z80_CLRFLAG_ZERO & Z80_CLRFLAG_HC
        & Z80_CLRFLAG_PARITY & Z80_CLRFLAG_SUBTRACT)
        )  //Clear S,Z,H,P,N (7,6,4,2,1) ***V0-
        | Z80_SETFLAG_SIGN(old_r + 1)
        | Z80_SETFLAG_ZERO(old_r + 1)
        | Z80_SETFLAG_HALF_CARRY(old_r, 1)
        | Z80_SETFLAG_OVERFLOW(old_r, old_r + 1);
    return Z80_STAGE_RESET;
}


///NEG; Size: 2; Flags: All
int NEG() {
    assert(z80.opcode_index == 2);
    Z80_A = Z80_A ^ 0xFF;
    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_SIGN(Z80_A);
    Z80_F |= Z80_SETFLAG_ZERO(Z80_A);
    Z80_F |= Z80_SETFLAG_HALF_BORROW(Z80_A ^ 0xFF, Z80_A);
    Z80_F |= (Z80_A == 0x7F) ? Z80_FLAG_PARITY : 0;
    Z80_F |= Z80_FLAG_SUBTRACT;
    Z80_F |= (Z80_A == 0xFF) ? 0 : Z80_FLAG_CARRY;
    Z80_F |= Z80_SETFLAG_UNK3(Z80_A);
    Z80_F |= Z80_SETFLAG_UNK5(Z80_A);
    return Z80_STAGE_RESET;
}

/* Stubs for IX/IY/(IX+d)/(IY+d)*/

///INC (IX + d); Size: 3; Flags: All
int INC_IXYp() {
    assert(z80.opcode_index == 3);
    alu_result_t r;
    const int8_t d = (int8_t) z80.opcode[2];
    if (z80.opcode[0] == 0xDD) {
        Z80_8BIT_READ(Z80_IX + d, 0);
        r = op_inc(z80.read_buffer[0], Z80_F);
        Z80_8BIT_WRITE(Z80_IX + d, 0, r.result);
    }
    else {
        Z80_8BIT_READ(Z80_IY + d, 0);
        r = op_inc(z80.read_buffer[0], Z80_F);
        Z80_8BIT_WRITE(Z80_IY + d, 0, r.result);
    }
    Z80_F = r.flags;
    return Z80_STAGE_RESET;
}

///DEC (IX + d); Size: 3; Flags: All
int DEC_IXYp() {
    assert(z80.opcode_index == 3);
    alu_result_t r;
    const int8_t d = (int8_t)z80.opcode[2];
    if (z80.opcode[0] == 0xDD) {
        Z80_8BIT_READ(Z80_IX + d, 0);
        r = op_dec(z80.read_buffer[0], Z80_F);
        Z80_8BIT_WRITE(Z80_IX + d, 0, r.result);
    }
    else {
        Z80_8BIT_READ(Z80_IY + d, 0);
        r = op_dec(z80.read_buffer[0], Z80_F);
        Z80_8BIT_WRITE(Z80_IY + d, 0, r.result);
    }
    Z80_F = r.flags;
    return Z80_STAGE_RESET;
}
