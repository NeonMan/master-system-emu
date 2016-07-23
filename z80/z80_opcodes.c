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

///CCF; Size: 1; Flags: C
int CCF(){
    assert(z80.opcode_index == 1);
    Z80_F = (Z80_F ^ Z80_FLAG_CARRY) & Z80_CLRFLAG_SUBTRACT;
    return Z80_STAGE_RESET;
}

///CPL; Size: 1; Flags: H,N
int CPL(){
    assert(z80.opcode_index == 1);
    Z80_A = ~Z80_A;
    Z80_F = Z80_F | Z80_FLAG_HC | Z80_FLAG_SUBTRACT;
    return Z80_STAGE_RESET;
}

///DAA; Size: 1; Flags: ???
int DAA(){
    assert(z80.opcode_index == 1);
    
    /* C&P from z80 heaven.
    When this instruction is executed, the A register is BCD corrected using the
    contents of the flags. The exact process is the following: if the least
    significant four bits of A contain a non-BCD digit (i. e. it is greater
    than 9) or the H flag is set, then $06 is added to the register. Then the
    four most significant bits are checked. If this more significant digit also
    happens to be greater than 9 or the C flag is set, then $60 is added.
    */
    /**@ToDo This is probably not correcttly  implemented :/ .*/
    uint8_t result = Z80_A;
    uint8_t flags = 0;
    if (((result & 0x0F) > 0x09) || (Z80_F & Z80_FLAG_HC)){
        result += 0x06;
    }
    if ((((result >> 4) & 0x0F) > 0x09) || (Z80_F & Z80_FLAG_CARRY)) {
        result += 0x60;
        flags |= Z80_FLAG_CARRY;
    }
    flags |= Z80_SETFLAG_SIGN(result);
    flags |= Z80_SETFLAG_ZERO(result);
    flags |= Z80_SETFLAG_PARITY(result);
    flags |= (Z80_F & Z80_FLAG_SUBTRACT);
    return Z80_STAGE_RESET;
}

///DI; Size: 1; Flags: None
int DI(){
    assert(z80.opcode_index == 1);
    z80.iff[0] = 0;
    z80.iff[1] = 0;
    return Z80_STAGE_RESET;
}

///EI; Size: 1; Flags: None
int EI(){
    assert(z80.opcode_index == 1);
    z80.iff[0] = 1;
    z80.iff[1] = 1;
    return Z80_STAGE_RESET;
}

///EX AF, AFp; Size: 1; Flags: None
int EX(){
    assert(z80.opcode_index == 1);
    const uint16_t tmp_af = Z80_AF;
    Z80_AF = Z80_AFp;
    Z80_AFp = tmp_af;
    return Z80_STAGE_RESET;
}

///EX DE, HL; Size: 1; Flags: None
int EX_DE_HL(){
    assert(z80.opcode_index == 1);
    const uint16_t old_de = Z80_DE;
    Z80_DE = Z80_HL;
    Z80_HL = old_de;
    return Z80_STAGE_RESET;
}

///EX (SP), HL; Size: 1; Flags: None
int EX_SPp_HL(){
    assert(z80.opcode_index == 1);
	Z80_16BIT_READ(Z80_SP, 0);
	Z80_16BIT_WRITE(Z80_SP, 0, (Z80_HL & 0x00FF), ((Z80_HL >> 8) & 0x00FF));
	Z80_HL = z80.read_buffer[0] + (((uint16_t)z80.read_buffer[1]) << 8);
    return Z80_STAGE_RESET;
}

///EXX; Size: 1; Flags: None
int EXX(){
    assert(z80.opcode_index == 1);
    const uint16_t old_bc = Z80_BC;
    const uint16_t old_de = Z80_DE;
    const uint16_t old_hl = Z80_HL;
    Z80_BC = Z80_BCp;
    Z80_DE = Z80_DEp;
    Z80_HL = Z80_HLp;
    Z80_BCp = old_bc;
    Z80_DEp = old_de;
    Z80_HLp = old_hl;
    return Z80_STAGE_RESET;
}

///HALT; Size: 1; Flags: None
int HALT(){
    
	if (z80.opcode_index != 1) {
		assert(z80.opcode_index == 2);
		/*Emulator hooks*/
		/* op, arg1 and arg2 are on the stack */
		/**@note Hooks are unimplemented.*/
	}
	else {
		/*Regular HALT*/
		assert(0); ///<-- Unimplemented
	}    
    return Z80_STAGE_RESET;
}

///IM(im[y]); Size: 2; Flags: None
int IM(){
    assert(z80.opcode_index == 2);
    Z80_OPCODE_SUBDIV;
    z80.iff[0] = z80_im[y[1]][0];
    z80.iff[1] = z80_im[y[1]][1];
    return Z80_STAGE_RESET;
}

///NOP; Size: 1; Flags: None
int NOP(){
    assert(z80.opcode_index == 1);
    return Z80_STAGE_RESET;
}

///RLA; Size: 2; Flags: HC,A,C
int RLA(){
    assert(z80.opcode_index == 1);
    const uint8_t next_carry = Z80_A & (1 << 7);
    Z80_A = (Z80_A << 1) | (Z80_F & Z80_FLAG_CARRY ? 1 : 0);
    Z80_F = (Z80_A & (Z80_CLRFLAG_HC & Z80_CLRFLAG_SUBTRACT & Z80_CLRFLAG_CARRY))
        | (next_carry ? Z80_FLAG_CARRY : 0);
    return Z80_STAGE_RESET;
}

///RLCA; Size: 1; Flags: H,N,C
int RLCA(){
    assert(z80.opcode_index == 1);
    Z80_A = (Z80_A << 1) | (Z80_A & (1 << 7) ? 1 : 0);
    Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_SUBTRACT & Z80_CLRFLAG_CARRY))
        | ((Z80_A & (1)) ? Z80_FLAG_CARRY : 0);
    return Z80_STAGE_RESET;
}

///RRA; Size: 1; Flags: H,N,C
int RRA(){
    assert(z80.opcode_index == 1);
    const uint8_t next_carry = Z80_A & (1);
    Z80_A = (Z80_A >> 1) | (Z80_F & Z80_FLAG_CARRY ? (1 << 7) : 0);
    Z80_F = (Z80_A & (Z80_CLRFLAG_HC & Z80_CLRFLAG_SUBTRACT & Z80_CLRFLAG_CARRY))
        | (next_carry ? Z80_FLAG_CARRY : 0);
    return Z80_STAGE_RESET;
}

///RRCA; Size: 1; Flags: H,N,C
int RRCA(){
    assert(z80.opcode_index == 1);
    Z80_A = (Z80_A >> 1) | (Z80_A & 1 ? (1 << 7) : 0);
    Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_SUBTRACT & Z80_CLRFLAG_CARRY))
        | ((Z80_A & (1 << 7)) ? Z80_FLAG_CARRY : 0);
    return Z80_STAGE_RESET;
}

///SCF; Size: 1; Flags: C
int SCF(){
    assert(z80.opcode_index == 1);
    Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_SUBTRACT)) | Z80_FLAG_CARRY;
    return Z80_STAGE_RESET;
}

int UNDOC(){
    assert(0); /*<-- Stop execution*/
    return Z80_STAGE_RESET;
}
