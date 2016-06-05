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

extern struct z80_s z80; //<-- Access to z80 internals

///ADC HL, rp; Size: 2; Flags: ???
int ADC_HL_rp() {
    assert(z80.opcode_index == 2);
    Z80_OPCODE_SUBDIV;
    const uint16_t result = (Z80_F & Z80_FLAG_CARRY) ? (Z80_HL + *(z80_rp[p[1]])) : ((Z80_HL + *(z80_rp[p[1]])) + 1);
    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_SIGN_16(result);
    Z80_F |= Z80_SETFLAG_ZERO_16(result);
    Z80_F |= Z80_SETFLAG_HALF_CARRY_16(Z80_HL, result);
    Z80_F |= Z80_SETFLAG_PARITY_16(result);
    Z80_F |= Z80_SETFLAG_CARRY_16(Z80_HL, result);
    Z80_HL = result;
    return Z80_STAGE_RESET;
}

///ADD HL, rp[p] ; Size: 1; Flags N,C
int ADD_HL_rp() {
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    const uint16_t old_hl = Z80_HL;
    Z80_HL = Z80_HL + *(z80_rp[p[0]]);
    //Clear N/Carry flag (bits 1,0)
    Z80_F = (Z80_F & (Z80_CLRFLAG_CARRY & Z80_CLRFLAG_SUBTRACT))
        //Set carry flag (bit 0)
        | Z80_SETFLAG_CARRY_16(old_hl, Z80_HL);
    return Z80_STAGE_RESET;
}

///DEC(rp[p]); Size: 1; Flags: None
int DEC_rp() {
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    --(*(z80_rp[p[0]]));
    return Z80_STAGE_RESET;
}


///INC(rp[p]); Size: 1; Flags: None
int INC_rp() {
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    ++(*(z80_rp[p[0]]));
    return Z80_STAGE_RESET;
}

///SBC HL,rp[p]; Size: 2; Flags: ?
int SBC_HL_rp() {
    assert(z80.opcode_index == 2);
    Z80_OPCODE_SUBDIV;
    const uint16_t old_hl = Z80_HL;
    Z80_HL = (Z80_F & Z80_FLAG_CARRY) ? Z80_HL - *(z80_rp[p[1]]) : Z80_HL - *(z80_rp[p[1]]) - 1;
    Z80_F = Z80_FLAG_SUBTRACT
        | Z80_SETFLAG_BORROW_16(old_hl, Z80_HL)
        | Z80_SETFLAG_SIGN_16(Z80_HL)
        | Z80_SETFLAG_ZERO_16(Z80_HL)
        | Z80_SETFLAG_HALF_BORROW_16(old_hl, *(z80_rp[p[1]]))
        | Z80_SETFLAG_OVERFLOW_16(old_hl, Z80_HL);
    return Z80_STAGE_RESET;
}

/* Stubs for IX/IY/(IX+d)/(IY+d)*/

int ADD_IXY_rp() {
    assert(z80.opcode_index == 2);
    Z80_OPCODE_SUBDIV;
    uint16_t result;
    if (z80.opcode[0] == 0xDD) {
        result = Z80_IX + (*(z80_rp[p[1]]));
        Z80_F = Z80_F & (Z80_FLAG_SIGN | Z80_FLAG_ZERO | Z80_FLAG_PARITY);
        Z80_F |= Z80_SETFLAG_HALF_CARRY_16(Z80_IX, result);
        Z80_F |= Z80_SETFLAG_CARRY_16(Z80_IX, result);
    }
    else {
        result = Z80_IY + (*(z80_rp[p[1]]));
        Z80_F = Z80_F & (Z80_FLAG_SIGN | Z80_FLAG_ZERO | Z80_FLAG_PARITY);
        Z80_F |= Z80_SETFLAG_HALF_CARRY_16(Z80_IY, result);
        Z80_F |= Z80_SETFLAG_CARRY_16(Z80_IY, result);
    }
    Z80_HL = result;
    return Z80_STAGE_RESET;
}

///INC IX; Size: 2; Flags: None
int INC_IXY() {
    assert(z80.opcode_index == 2);
    if (z80.opcode[0] == 0xDD) {
        ++Z80_IX;
    }
    else {
        ++Z80_IY;
    }
    return Z80_STAGE_RESET;
}

///DEC IX; Size: 2; Flags: None
int DEC_IXY() {
    assert(z80.opcode_index == 2);
    if (z80.opcode[0] == 0xDD) {
        --Z80_IX;
    }
    else {
        --Z80_IY;
    }
    return Z80_STAGE_RESET;
}
