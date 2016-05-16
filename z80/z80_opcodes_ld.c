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

extern struct z80_s z80; //<-- Access to z80 internals

///CPD; Size: 2; Flags: ???
int CPD() {
	assert(z80.opcode_index == 2);
	assert(0); ///<-- Unimplemented
	return Z80_STAGE_RESET;
}

///CPI; Size: 2; Flags: ???
int CPI() {
	assert(z80.opcode_index == 2);
	assert(0); ///<-- Unimplemented
	return Z80_STAGE_RESET;
}

///CPDR; Size: 2; Flags: ???
int CPDR() {
	assert(z80.opcode_index == 2);
	assert(0); ///<-- Unimplemented
	return Z80_STAGE_RESET;
}

///CPIR; Size: 2; Flags: ???
int CPIR() {
	assert(z80.opcode_index == 2);
	assert(0); ///<-- Unimplemented
	return Z80_STAGE_RESET;
}

///IN A, (n); Size: 2; Flags: None
int IN_A_np() {
    assert(z80.opcode_index == 2);

    Z80_8BIT_READ((z80.opcode[1] | (((uint16_t)Z80_A) << 8)), 1);
    Z80_A = z80.read_buffer[0];
    return Z80_STAGE_RESET;
}

///IN (C); Size: 2; Flags: ???
int IN_Cp() {
    assert(z80.opcode_index == 2);
    assert(0);
    return Z80_STAGE_RESET;
}

///IN r, (C); Size: 2; Flags: ???
int IN_r_Cp() {
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///IND; Size: 2; Flags: ???
int IND() {
	assert(z80.opcode_index == 2);
	assert(0); ///<-- Unimplemented
	return Z80_STAGE_RESET;
}

///INDR; Size: 2; Flags: ???
int INDR() {
	assert(z80.opcode_index == 2);
	assert(0); ///<-- Unimplemented
	return Z80_STAGE_RESET;
}

///INI; Size: 2; Flags: ???
int INI() {
	assert(z80.opcode_index == 2);
	assert(0); ///<-- Unimplemented
	return Z80_STAGE_RESET;
}

///INIR; Size: 2; Flags: ???
int INIR() {
	assert(z80.opcode_index == 2);
	assert(0); ///<-- Unimplemented
	return Z80_STAGE_RESET;
}

///LD A, (BC); Size: 1; Flags: None
int LD_A_BCp() {
	assert(z80.opcode_index == 1);
	Z80_8BIT_READ(Z80_BC, 0);
	Z80_A = z80.read_buffer[0];
	return Z80_STAGE_RESET;
}

///LD A,(DE); Size: 1; Flags: None
int LD_A_DEp() {
	assert(z80.opcode_index == 1);
	Z80_8BIT_READ(Z80_DE, 0);
	Z80_A = z80.read_buffer[0];
	return Z80_STAGE_RESET;
}

///LD A, I; Size: 2; Flags: ???
int LD_A_I() {
	assert(z80.opcode_index == 2);
	assert(0); ///<-- Unimplemented
	return Z80_STAGE_RESET;
}

///LD A, (nn); Size: 3; Flags: None
int LD_A_nnp() {
	assert(z80.opcode_index == 3);
	Z80_8BIT_READ(z80.opcode[1] | (((uint16_t)z80.opcode[2]) << 8), 0);
	Z80_A = z80.read_buffer[0];
	return Z80_STAGE_RESET;
}

///LD A, R; Size: 2; Flags: ???
int LD_A_R() {
	assert(z80.opcode_index == 2);
	assert(0); ///<-- Unimplemented
	return Z80_STAGE_RESET;
}

///LD (BC), A; Size:1; Flags: None
int LD_BCp_A() {
	assert(z80.opcode_index == 1);
	Z80_8BIT_WRITE(Z80_BC, 0, Z80_A);
	//Otherwise, reset pipeline
	return Z80_STAGE_RESET;
}

///LD HL, (nn); Size: 3; Flags: None
int LD_HL_nnp() {
	assert(z80.opcode_index == 3);
	Z80_16BIT_READ(z80.opcode[1] | (((uint16_t)z80.opcode[2]) << 8), 0);

	Z80_H = z80.read_buffer[1];
	Z80_L = z80.read_buffer[0];
	return Z80_STAGE_RESET;
}

///LD (HL), n; Size: 2; Flags: None
int LD_HLp_n() {
	assert(z80.opcode_index == 2);
	Z80_8BIT_WRITE(Z80_HL, 0, z80.opcode[1]);
	return Z80_STAGE_RESET;
}

///LD (HL), r; Size: 1; Flags: None
int LD_HLp_r() {
	assert(z80.opcode_index == 1);
	Z80_OPCODE_SUBDIV;
	Z80_8BIT_WRITE(Z80_HL, 0, *(z80_r[z[0]]))
		return Z80_STAGE_RESET;
}

///LD I, A; Size: 2; Flags: None
int LD_I_A() {
	assert(z80.opcode_index == 2);
	Z80_I = Z80_A;
	return Z80_STAGE_RESET;
}

///LD (DE), A; Size: 1; Flags: None
int LD_DEp_A() {
	assert(z80.opcode_index == 1);
	Z80_8BIT_WRITE(Z80_DE, 0, Z80_A);
	return Z80_STAGE_RESET;
}

///LD (nn), rp; Size:4; Flags: None
int LD_nnp_rp() {
	Z80_OPCODE_SUBDIV;
	assert(z80.opcode_index = 4);

	const uint8_t b0 = (*(z80_rp[p[1]])) & 0x00FF;
	const uint8_t b1 = ((*(z80_rp[p[1]])) >> 8) & 0x00FF;
	Z80_16BIT_WRITE((z80.opcode[2] | (((uint16_t)z80.opcode[3]) << 8)), 0, b0, b1);
	return Z80_STAGE_RESET;
}

///LD R, A; Size: 2; Flags: None
int LD_R_A() {
	assert(z80.opcode_index == 2);
	Z80_R = Z80_A;
	return Z80_STAGE_RESET;
}

///LD r, (HL); Size: 1; Flags: None
int LD_r_HLp() {
	assert(z80.opcode_index == 1);
	Z80_OPCODE_SUBDIV;

	Z80_8BIT_READ(Z80_HL, 0);
	*(z80_r[y[0]]) = z80.read_buffer[0];
	return Z80_STAGE_RESET;
}

///LD r, n; Size: 2; Flags: None
int LD_r_n() {
	assert(z80.opcode_index == 2);
	Z80_OPCODE_SUBDIV;
	*(z80_r[y[0]]) = z80.opcode[1];
	return Z80_STAGE_RESET;
}

///LD r[y],r[z]; Size: 1; Flags: None
int LD_r_r() {
	assert(z80.opcode_index == 1);
	Z80_OPCODE_SUBDIV;
	assert(y[0] != 0x06); //
	assert(z[0] != 0x06); // (HL) is treated as a special case
	*(z80_r[y[0]]) = *(z80_r[z[0]]);
	return Z80_STAGE_RESET;
}

///LD rp[n], nn; Size: 3; Flags: None
int LD_rp_nn() {
	assert(z80.opcode_index == 3);
	//nn is stored in the opcode's bytes 1,2.
	Z80_OPCODE_SUBDIV;
	const uint16_t immediate = *((uint16_t*)(z80.opcode + 1)); ///<-- @bug Endianness!
	*z80_rp[p[0]] = immediate; ///<-- @bug Endianness!
	return Z80_STAGE_RESET;
}

///LD rp, (nn); Size: 4; Flags: None
int LD_rp_nnp() {
	Z80_OPCODE_SUBDIV;
	assert(z80.opcode_index == 4);
	Z80_16BIT_READ(z80.opcode[2] | (((uint16_t)z80.opcode[3]) << 8), 0);
	*(z80_rp[p[1]]) = z80.read_buffer[0] | (((uint16_t)z80.read_buffer[1]) << 8);
	return Z80_STAGE_RESET;
}

///LD SP, HL; Size: 1; Flags: None
int LD_SP_HL() {
	assert(z80.opcode_index == 1);
	Z80_SP = Z80_HL;
	return Z80_STAGE_RESET;
}

///LD (nn), A; Size: 3; Flags: None
int LD_nnp_A() {
	assert(z80.opcode_index == 3);
	Z80_8BIT_WRITE((z80.opcode[1] | (((uint16_t)z80.opcode[2]) << 8)), 0, Z80_A);
	return Z80_STAGE_RESET;
}

///LD (nn), HL; size: 3; Flags: None
int LD_nnp_HL() {
	assert(z80.opcode_index == 3);
	Z80_16BIT_WRITE(z80.opcode[1] | (((uint16_t)z80.opcode[2]) << 8), 0, Z80_L, Z80_H);
	return Z80_STAGE_RESET;
}

///LDD; Size: 2; Flags: ???
int LDD() {
	assert(z80.opcode_index == 2);
	//Copy a byte.
	Z80_8BIT_READ(Z80_HL, 0);
	Z80_8BIT_WRITE(Z80_DE, 0, z80.read_buffer[0]);
	//Update registers
	--(Z80_DE);
	--(Z80_HL);
	--(Z80_BC);
	//Update flags
	Z80_F = Z80_F & Z80_CLRFLAG_HC & Z80_CLRFLAG_PARITY & Z80_CLRFLAG_SUBTRACT & Z80_CLRFLAG_UNK3 & Z80_CLRFLAG_UNK5;
	Z80_F = Z80_F | ((Z80_BC == 0) ? 0 : Z80_FLAG_PARITY);
	//Undoc flags
	Z80_F = Z80_F | (((z80.read_buffer[0] + Z80_A) & (1 << 3)) ? Z80_FLAG_UNK3 : 0);
	Z80_F = Z80_F | (((z80.read_buffer[0] + Z80_A) & (1 << 1)) ? Z80_FLAG_UNK5 : 0);
	//Shall we continue copying?
	if (Z80_BC) {
		//Execute this opcode again
		Z80_PC -= 2;
	}
	return Z80_STAGE_RESET;
}

///LDI; Size: 2; Flags: H,V,N
int LDI() {
	assert(z80.opcode_index == 2);
	//Copy a byte.
	Z80_8BIT_READ(Z80_HL, 0);
	Z80_8BIT_WRITE(Z80_DE, 0, z80.read_buffer[0]);
	//Update registers
	++(Z80_DE);
	++(Z80_HL);
	--(Z80_BC);
	//Update flags
	Z80_F &= Z80_CLRFLAG_HC & Z80_CLRFLAG_PARITY & Z80_CLRFLAG_SUBTRACT;
	Z80_F |= ((Z80_BC == 0) ? 0 : Z80_FLAG_PARITY);
	//Undoc flags
	Z80_F = Z80_F | (((z80.read_buffer[0] + Z80_A) & (1 << 3)) ? Z80_FLAG_UNK3 : 0);
	Z80_F = Z80_F | (((z80.read_buffer[0] + Z80_A) & (1 << 1)) ? Z80_FLAG_UNK5 : 0);
	//Shall we continue copying?
	if (Z80_BC) {
		//Execute this opcode again
		Z80_PC -= 2;
	}
	return Z80_STAGE_RESET;
}

///LDDR; Size: 2; Flags: ???
int LDDR() {
	assert(z80.opcode_index == 2);
	int rv = LDD();
	if (rv == Z80_STAGE_RESET) {
		return Z80_STAGE_REFRESH;
	}
	else {
		return rv;
	}
}

///LDIR; Size: 2; Flags: H,P,N (cleared)
int LDIR() {
	assert(z80.opcode_index == 2);
	int rv = LDI();
	if (rv == Z80_STAGE_RESET) {
		return Z80_STAGE_REFRESH;
	}
	else {
		return rv;
	}
}

///OTDR; Size: 2; Flags: ???
int OTDR() {
	assert(z80.opcode_index == 2);
	assert(0); ///<-- Unimplemented
	return Z80_STAGE_RESET;
}

///OTIR; Size: 2; Flags: Z,N
int OTIR() {
	assert(z80.opcode_index == 2);
	//(C)<-(HL), B<-B  1, HL<-HL + 1; B? repeat : end
	//Perform read
	Z80_8BIT_READ(Z80_HL, 0);
	//Perform write
	Z80_8BIT_WRITE(Z80_C | (((uint16_t)Z80_A) << 8), 1, z80.read_buffer[0]);
	//Update state and flags
	++Z80_HL;
	--Z80_B;
	Z80_F = Z80_F & (Z80_CLRFLAG_ZERO & Z80_CLRFLAG_SUBTRACT); //Z,N
	if (Z80_B) { //Repeat instruction
		Z80_PC = Z80_PC - 2;
		return Z80_STAGE_RESET;
	}
	else {
		return Z80_STAGE_RESET;
	}
}

///OUT (n), A; Size: 2; Flags: None
int OUT_np_A() {
	assert(z80.opcode_index == 2);
	const uint16_t port_addr = z80.opcode[1] + (((uint16_t)Z80_A) << 8);
	Z80_8BIT_WRITE(port_addr, 1, Z80_A);
	return Z80_STAGE_RESET;
}

///OUT (C), 0; Size: 2; Flags: None
int OUT_Cp_0() {
	assert(z80.opcode_index == 2);
	assert(0); ///<-- Unimplemented
	return Z80_STAGE_RESET;
}

///OUT (C), r; Size: 2; Flags: None
int OUT_Cp_r() {
	assert(z80.opcode_index == 2);
	assert(0); ///<-- Unimplemented
	return Z80_STAGE_RESET;
}

///OUTD; Size: 2; Flags: ???
int OUTD() {
	assert(z80.opcode_index == 2);
	assert(0); ///<-- Unimplemented
	return Z80_STAGE_RESET;
}

///OUTI; Size: 2; Flags: ???
int OUTI() {
	assert(z80.opcode_index == 2);
	assert(0); ///<-- Unimplemented
	return Z80_STAGE_RESET;
}

///POP rp2[p]; Size: 1; Flags: None
int POP_rp2() {
	assert(z80.opcode_index == 1);
	Z80_OPCODE_SUBDIV;
	//Read stack
	Z80_16BIT_READ(Z80_SP, 0);

	Z80_SP += 2;
	*(z80_rp2[p[0]]) = *((uint16_t*)z80.read_buffer); ///<-- @bug Endianness!
	return Z80_STAGE_RESET;
}

///PUSH rp2[p]; Size: 1; Flags: None
int PUSH_rp2() {
	assert(z80.opcode_index == 1);
	Z80_OPCODE_SUBDIV;
	Z80_16BIT_WRITE(Z80_SP - 2, 0, (*(z80_rp2[p[0]])) & 0xFF, ((*(z80_rp2[p[0]])) >> 8) & 0xFF);
	Z80_SP -= 2;
	return Z80_STAGE_RESET;
}


/* ---------------------------------------------- */
/* --- IX/IY/(IX+d)/(IY+d) version of opcodes --- */
/* ---------------------------------------------- */

int LD_IXY_nn() {
	Z80_OPCODE_SUBDIV;
	assert(z80.opcode_index == 4);
	assert(p[1] == 2);
	if (z80.opcode[0] == 0xDD) {
		Z80_IX = (((uint16_t)z80.opcode[3]) << 8) | z80.opcode[2];
	}
	else {
		assert(z80.opcode[0] == 0xFD);
		Z80_IY = (((uint16_t)z80.opcode[3]) << 8) | z80.opcode[2];
	}
	return Z80_STAGE_RESET;
}

int LD_IXYH_n() {
	Z80_OPCODE_SUBDIV;
	assert(z80.opcode_index == 3);
	assert(y[1] == 4);
	if (z80.opcode[0] == 0xDD) {
		Z80_IXH = z80.opcode[2];
	}
	else {
		assert(z80.opcode[0] == 0xFD);
		Z80_IYH = z80.opcode[2];
	}
	return Z80_STAGE_RESET;
}

int LD_IXYL_n() {
	Z80_OPCODE_SUBDIV;
	assert(z80.opcode_index == 3);
	assert(y[1] == 5);
	if (z80.opcode[0] == 0xDD) {
		Z80_IXL = z80.opcode[2];
	}
	else {
		assert(z80.opcode[0] == 0xFD);
		Z80_IYL = z80.opcode[2];
	}
	return Z80_STAGE_RESET;
}

int LD_nnp_IXY() {
	assert(z80.opcode_index == 4);
	if (z80.opcode[0] == 0xDD) {
		Z80_16BIT_WRITE(z80.opcode[2] | (((uint16_t)z80.opcode[3]) << 8), 0, Z80_IXL, Z80_IXH);
	}
	else {
		assert(z80.opcode[0] == 0xFD);
		Z80_16BIT_WRITE(z80.opcode[2] | (((uint16_t)z80.opcode[3]) << 8), 0, Z80_IYL, Z80_IYH);
	}
	return Z80_STAGE_RESET;
}

int LD_IXY_nnp() {
	assert(z80.opcode_index == 4);
	Z80_16BIT_READ(z80.opcode[2] | (((uint16_t)z80.opcode[3]) << 8), 0);
	assert(z80.read_index == 2);
	if (z80.opcode[0] == 0xDD) {
		Z80_IX = z80.read_buffer[0] | (((uint16_t)z80.read_buffer[1]) << 8);
	}
	else {
		assert(z80.opcode[0] == 0xFD);
		Z80_IY = z80.read_buffer[0] | (((uint16_t)z80.read_buffer[1]) << 8);
	}
	return Z80_STAGE_RESET;
}

int LD_IXYp_n() {
	assert(z80.opcode_index == 4);
	if (z80.opcode[0] == 0xDD) {
		Z80_8BIT_WRITE(Z80_IX + ((int8_t)z80.opcode[2]), 0, z80.opcode[3]);
	}
	else {
		assert(z80.opcode[0] == 0xFD);
		Z80_8BIT_WRITE(Z80_IY + ((int8_t)z80.opcode[2]), 0, z80.opcode[3]);
	}
	return Z80_STAGE_RESET;
}

int LD_r_IXYp() {
	Z80_OPCODE_SUBDIV;
	assert(z80.opcode_index == 3);
	if (z80.opcode[0] == 0xDD) {
		Z80_8BIT_READ(Z80_IX + ((int8_t)z80.opcode[2]), 0);
	}
	else {
		assert(z80.opcode[0] == 0xFD);
		Z80_8BIT_READ(Z80_IY + ((int8_t)z80.opcode[2]), 0);
	}
	*(z80_r[y[1]]) = z80.read_buffer[0];
	return Z80_STAGE_RESET;
}

int LD_IXYp_r() {
	Z80_OPCODE_SUBDIV;
	assert(z80.opcode_index == 3);
	assert(z[1] != 6);
	if (z80.opcode[0] == 0xDD) {
		Z80_8BIT_WRITE(Z80_IX + ((int8_t)z80.opcode[2]), 0, *(z80_r[z[1]]));
	}
	else {
		assert(z80.opcode[0] == 0xFD);
		Z80_8BIT_WRITE(Z80_IY + ((int8_t)z80.opcode[2]), 0, *(z80_r[z[1]]));
	}
	return Z80_STAGE_RESET;
}

int LD_SP_IXY() {
	assert(z80.opcode_index == 2);
	if (z80.opcode[0] == 0xDD) {
		Z80_SP = Z80_IX;
	}
	else {
		assert(z80.opcode[0] == 0xFD);
		Z80_SP = Z80_IY;
	}
	return Z80_STAGE_RESET;
}

///POP IX/POP IY; Size: 2; Flags: None
int POP_IXY() {
	Z80_OPCODE_SUBDIV;
	assert(z80.opcode_index == 2);
	assert(p[1] == Z80_RP_INDEX_HL);

	Z80_16BIT_READ(Z80_SP, 0);
	if (z80.opcode[0] == 0xDD) {
		Z80_IXL = z80.read_buffer[0];
		Z80_IXH = z80.read_buffer[1];
	}
	else {
		Z80_IYL = z80.read_buffer[0];
		Z80_IYH = z80.read_buffer[1];
	}
	Z80_SP = Z80_SP + 2;
	return Z80_STAGE_RESET;
}

int EX_SPp_IXY() {
	assert(0); /*<-- Unimplemented*/
	return Z80_STAGE_RESET;
}

int PUSH_IXY() {
	assert(z80.opcode_index == 2);
	if (z80.opcode[0] == 0xDD) {
		Z80_16BIT_WRITE(Z80_SP - 2, 0, Z80_IXL, Z80_IXH);
	}
	else {
		Z80_16BIT_WRITE(Z80_SP - 2, 0, Z80_IYL, Z80_IYH);
	}
	Z80_SP = Z80_SP - 2;
	return Z80_STAGE_RESET;
}

