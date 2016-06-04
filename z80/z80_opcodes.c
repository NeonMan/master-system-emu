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
    assert(0); ///<-- Unimplemented
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
    assert(0); ///<-- Unimplemented
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
    assert(z80.opcode_index == 1);
    assert(0); ///<-- Unimplemented
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

///RLC r; Size: 2; Flags: ???
int RLC_r(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///RLC (HL); Size: 2; Flags: ???
int RLC_HLp() {
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///RLC (IX+d); Size: 4; Flags: All
int RLC_IXYp() {
    assert(z80.opcode_index == 4);
    const int8_t d = (int8_t)z80.opcode[2];
    /*Perform memory read*/
    if (z80.opcode[0] == 0xDD){
        Z80_8BIT_READ(Z80_IX + d, 0);
    }
    else{
        Z80_8BIT_READ(Z80_IY + d, 0);
    }
    /*Perform rotation*/
    const uint8_t carry_out = ((z80.read_buffer[0] >> 7) & 0x01);
    const uint8_t result = ((z80.read_buffer[0] << 1) & 0xFE) | carry_out;
    /*Flags*/
    Z80_F =
        Z80_SETFLAG_SIGN(result)
        | Z80_SETFLAG_ZERO(result)
        | Z80_SETFLAG_PARITY(result)
        | (carry_out ? Z80_FLAG_CARRY : 0)
        ;
    /*Perform write back*/
    if (z80.opcode[0] == 0xDD){
        Z80_8BIT_WRITE(Z80_IX + d, 0, result);
    }
    else{
        Z80_8BIT_WRITE(Z80_IY + d, 0, result);
    }
    return Z80_STAGE_RESET;
}

///RLD; Size: 2; Flags: ???
int RLD(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///RL (HL); Size: 2; Flags: ???
int RL_HLp(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}


///RL r; Size: 2; Flags: ???
int RL_r(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
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

///RRC (HL); Size: 2; Flags: ???
int RRC_HLp(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///RRC r; Size: 2; Flags: ???
int RRC_r(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///RRD; Size: 2; Flags: ???
int RRD(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///RR (HL); Size: 2; Flags: ???
int RR_HLp(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///RR r; Size: 2; Flags: ???
int RR_r(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SCF; Size: 1; Flags: C
int SCF(){
    assert(z80.opcode_index == 1);
    Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_SUBTRACT)) | Z80_FLAG_CARRY;
    return Z80_STAGE_RESET;
}

///SLA (HL); Size: 2; Flags: ???
int SLA_HLp(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SLA r; Size: 2; Flags: ???
int SLA_r(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SLL (HL); Size: 2; Flags: ???
int SLL_HLp(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SLL r; Size: 2; Flags: ???
int SLL_r(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SRA (HL); Size: 2; Flags: ???
int SRA_HLp(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SRA r; Size: 2; Flags: ???
int SRA_r(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SRL (HL); Size: 2; Flags: ???
int SRL_HLp(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SRL r[z]; Size: 2; Flags: ?
int SRL_r(){
    assert(z80.opcode_index == 2);
    Z80_OPCODE_SUBDIV;
    if (z80_r[z[1]]){
        const uint8_t old_r = *(z80_r[z[1]]);
        *(z80_r[z[1]]) = (*(z80_r[z[1]])) >> 1;
        Z80_F = 0;
        Z80_F |= old_r & 1 ? Z80_FLAG_CARRY : 0;
        Z80_F |= Z80_SETFLAG_ZERO(*(z80_r[z[1]]));
        Z80_F |= Z80_SETFLAG_PARITY(*(z80_r[z[1]]));
        return Z80_STAGE_RESET;
    }
    else{ //SRL (HL)
        assert(0); //Unimplemented
    }
    assert(0); //Unimplemented
    return Z80_STAGE_RESET;
}

/* Stubs for IX/IY/(IX+d)/(IY+d)*/

int SLL_IXYp(){
    /*Select index register address*/
    const uint16_t address = (z80.opcode[0] == 0xDD) ? Z80_IX : Z80_IY;
    /*Read*/
    const int8_t d = (int8_t)z80.opcode[2];
    Z80_8BIT_READ(address + d, 0);
    /*Execute opcode and update flags*/
    const uint8_t next_c = z80.read_buffer[0] & 0x80;
    const uint8_t next_b = (z80.read_buffer[0] << 1) | 0x01;
    /*Write back*/
    Z80_8BIT_WRITE(address + d, 0, next_b);
    /*Update flags*/
    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_ZERO(next_b);
    Z80_F |= next_c ? Z80_FLAG_CARRY : 0;
    Z80_F |= Z80_SETFLAG_SIGN(next_b);
    Z80_F |= Z80_SETFLAG_PARITY(next_b);
    return Z80_STAGE_RESET;
}

int RRC_IXYp(){
    /*Select index register address*/
    const uint16_t address = (z80.opcode[0] == 0xDD) ? Z80_IX : Z80_IY;
    /*Read*/
    const int8_t d = (int8_t)z80.opcode[2];
    Z80_8BIT_READ(address + d, 0);
    /*Execute opcode and update flags*/
    const uint8_t next_c = z80.read_buffer[0] & 0x01;
    const uint8_t next_b = (z80.read_buffer[0] >> 1) & 0x7F;
    /*Write back*/
    Z80_8BIT_WRITE(address + d, 0, next_b);
    /*Update flags*/
    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_ZERO(next_b);
    Z80_F |= next_c ? Z80_FLAG_CARRY : 0;
    Z80_F |= Z80_SETFLAG_SIGN(next_b);
    Z80_F |= Z80_SETFLAG_PARITY(next_b);
    return Z80_STAGE_RESET;
}

int RL_IXYp(){
    /*Select index register address*/
    const uint16_t address = (z80.opcode[0] == 0xDD) ? Z80_IX : Z80_IY;
    /*Read*/
    const int8_t d = (int8_t)z80.opcode[2];
    Z80_8BIT_READ(address + d, 0);
    /*Execute opcode and update flags*/
    const uint8_t next_lsb = Z80_C ? 1 : 0;
    const uint8_t next_c = z80.read_buffer[0] & 0x80;
    const uint8_t next_b = (z80.read_buffer[0] << 1) | next_lsb;
    /*Write back*/
    Z80_8BIT_WRITE(address + d, 0, next_b);
    /*Update flags*/
    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_ZERO(next_b);
    Z80_F |= next_c ? Z80_FLAG_CARRY : 0;
    Z80_F |= Z80_SETFLAG_SIGN(next_b);
    Z80_F |= Z80_SETFLAG_PARITY(next_b);
    return Z80_STAGE_RESET;
}

int SLA_IXYp(){
    /*Select index register address*/
    const uint16_t address = (z80.opcode[0] == 0xDD) ? Z80_IX : Z80_IY;
    /*Read*/
    const int8_t d = (int8_t)z80.opcode[2];
    Z80_8BIT_READ(address + d, 0);
    /*Execute opcode and update flags*/
    const uint8_t next_c = z80.read_buffer[0] & 0x80;
    const uint8_t next_b = (z80.read_buffer[0] << 1);
    /*Write back*/
    Z80_8BIT_WRITE(address + d, 0, next_b);
    /*Update flags*/
    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_ZERO(next_b);
    Z80_F |= next_c ? Z80_FLAG_CARRY : 0;
    Z80_F |= Z80_SETFLAG_SIGN(next_b);
    Z80_F |= Z80_SETFLAG_PARITY(next_b);
    return Z80_STAGE_RESET;
}

int RR_IXYp(){
    /*Select index register address*/
    const uint16_t address = (z80.opcode[0] == 0xDD) ? Z80_IX : Z80_IY;
    /*Read*/
    const int8_t d = (int8_t)z80.opcode[2];
    Z80_8BIT_READ(address + d, 0);
    /*Execute opcode and update flags*/
    const uint8_t next_msb = Z80_C ? 0x80 : 0;
    const uint8_t next_c = z80.read_buffer[0] & 0x01;
    const uint8_t next_b = ((z80.read_buffer[0] >> 1) & 0x7F) | next_msb;
    /*Write back*/
    Z80_8BIT_WRITE(address + d, 0, next_b);
    /*Update flags*/
    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_ZERO(next_b);
    Z80_F |= next_c ? Z80_FLAG_CARRY : 0;
    Z80_F |= Z80_SETFLAG_SIGN(next_b);
    Z80_F |= Z80_SETFLAG_PARITY(next_b);
    return Z80_STAGE_RESET;
}

int SRA_IXYp(){
    /*Select index register address*/
    const uint16_t address = (z80.opcode[0] == 0xDD) ? Z80_IX : Z80_IY;
    /*Read*/
    const int8_t d = (int8_t)z80.opcode[2];
    Z80_8BIT_READ(address + d, 0);
    /*Execute opcode and update flags*/
    const uint8_t next_c = z80.read_buffer[0] & 0x01;
    const uint8_t next_b = (z80.read_buffer[0] >> 1) | (z80.read_buffer[0] & 0x80);
    /*Write back*/
    Z80_8BIT_WRITE(address + d, 0, next_b);
    /*Update flags*/
    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_ZERO(next_b);
    Z80_F |= next_c ? Z80_FLAG_CARRY : 0;
    Z80_F |= Z80_SETFLAG_SIGN(next_b);
    Z80_F |= Z80_SETFLAG_PARITY(next_b);
    return Z80_STAGE_RESET;
}

///SRL (IX+d); Size: 4; Flags: All
int SRL_IXYp(){
	assert(z80.opcode_index == 4);
	const int8_t d = (int8_t)z80.opcode[2];
	/*Perform read*/
	if (z80.opcode[0] == 0xDD){
		Z80_8BIT_READ(Z80_IX + d, 0);
	}
	else{
		Z80_8BIT_READ(Z80_IY + d, 0);
	}
	/*Perform shift*/
	const uint8_t carry_out = z80.read_buffer[0] & 0x01;
	const uint8_t result = (z80.read_buffer[0] >> 1) & 0x7F;
	/*Flags*/
	Z80_F = 
		Z80_SETFLAG_ZERO(result)
		| Z80_SETFLAG_PARITY(result)
		| (carry_out ? Z80_FLAG_CARRY : 0)
		;
	/*Write back*/
	if (z80.opcode[0] == 0xDD){
		Z80_8BIT_WRITE(Z80_IX + d, 0, result);
	}
	else{
		Z80_8BIT_WRITE(Z80_IY + d, 0, result);
	}
    return Z80_STAGE_RESET;
}

int UNDOC(){
    assert(0); /*<-- Stop execution*/
    return Z80_STAGE_RESET;
}
