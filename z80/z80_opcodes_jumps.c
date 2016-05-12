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

///CALL cc[y] nn; Size: 3; Flags: None
int CALL_cc_nn() {
	assert(z80.opcode_index == 3);
	Z80_OPCODE_SUBDIV;
	//Check condition
	if ((Z80_F & z80_cc[y[0]]) == (z80_cc_stat[y[0]])) {
		//Push PC onto the stack
		Z80_16BIT_WRITE(Z80_SP - 2, 0, Z80_PC & 0xFF, ((Z80_PC >> 8) & 0xFF));

		//Update SP
		Z80_SP -= 2;
		//Update PC
		const uint16_t new_pc = *((uint16_t*)(z80.opcode + 1)); ///<-- @bug Endiannes!
		Z80_PC = new_pc;
		return Z80_STAGE_RESET;
	}
	else {
		return Z80_STAGE_RESET;
	}
}

///CALL nn; Size: 3; Flags: None
int CALL_nn() {
	assert(z80.opcode_index == 3);
	//Push PC into stack
	Z80_16BIT_WRITE(Z80_SP - 2, 0, Z80_PC & 0xFF, (Z80_PC >> 8) & 0xFF);
	//Update SP
	Z80_SP -= 2;
	//Update PC
	const uint16_t new_pc = *((uint16_t*)(z80.opcode + 1)); ///<-- @bug Endiannes!
	Z80_PC = new_pc;
	return Z80_STAGE_RESET;
}

///DJNZ e; Size: 2; Flags: None
int DJNZ_d() {
	assert(z80.opcode_index == 2);
	--Z80_B;
	if (Z80_B) {
		Z80_PC += ((int8_t)z80.opcode[1]);
		return Z80_STAGE_RESET;
	}
	else {
		return Z80_STAGE_RESET;
	}
}

///JP nn; Size: 3; Flags: None
int JP_nn() {
	assert(z80.opcode_index == 3);
	///New PC stored in opcode's last bytes. @bug Endianness.
	const uint16_t new_pc = *((uint16_t*)(z80.opcode + 1));
	Z80_PC = new_pc;
	return Z80_STAGE_RESET;
}

///JP cc[y], nn; Size: 3; Flags: None
int JP_cc_nn() {
	assert(z80.opcode_index == 3);
	Z80_OPCODE_SUBDIV;
	//Test condition
	if ((Z80_F & (z80_cc[y[0]])) == (z80_cc_stat[y[0]])) {
		const uint16_t new_pc = *((uint16_t*)(z80.opcode + 1));
		Z80_PC = new_pc; ///<-- @bug Endianness!
		return Z80_STAGE_RESET;
	}
	else {
		return Z80_STAGE_RESET;
	}
}

///JP HL; Size: 1; Flags: None
int JP_HLp() {
	assert(z80.opcode_index == 1);
	assert(0); ///<-- Unimplemented
	return Z80_STAGE_RESET;
}

///JR cc, e; Size: 2; Flags: None
int JR_cc_d() {
	assert(z80.opcode_index == 2);
	//Test required flag
	Z80_OPCODE_SUBDIV;
	if ((Z80_F & z80_cc[y[0] - 4]) == (z80_cc_stat[y[0] - 4])) {
		const int8_t pc_shift = *((int8_t*)&z80.opcode[1]); ///<-- @bug Endianness
		const int32_t next_pc = Z80_PC + pc_shift; //Signed relative jump
		Z80_PC = (next_pc & 0xFFFF);
	}
	return Z80_STAGE_RESET;
}

///JR, e; Size: 2; Flags: None
int JR_d() {
	assert(z80.opcode_index == 2);
	const int8_t pc_shift = *((int8_t*)&z80.opcode[1]); ///<-- @bug Endianness
	const int32_t next_pc = Z80_PC + pc_shift; //Signed relative jump
	Z80_PC = (next_pc & 0xFFFF);
	return Z80_STAGE_RESET;
}

///RET; Size: 1; Flags: None
int RET() {
	assert(z80.opcode_index == 1);
	//Read stack
	Z80_16BIT_READ(Z80_SP, 0);

	Z80_SP += 2;
	Z80_PC = z80.read_buffer[0] | (((uint16_t)(z80.read_buffer[1])) << 8);
	return Z80_STAGE_RESET;
}

///RET cc[y]; Size: 1; Flags: None
int RET_cc() {
	assert(z80.opcode_index == 1);
	Z80_OPCODE_SUBDIV;
	if ((Z80_F & z80_cc[y[0]]) == z80_cc_stat[y[0]]) {
		//POP stack, update PC
		Z80_16BIT_READ(Z80_SP, 0);

		Z80_PC = *((uint16_t*)(z80.read_buffer));
		Z80_SP += 2;
		return Z80_STAGE_RESET;
	}
	else {
		return Z80_STAGE_RESET;
	}
}

///RETI; Size: 2; Flags: None
///@note essentially identical to RET(?)
int RETI() {
	assert(z80.opcode_index == 2);
	//Read stack
	Z80_16BIT_READ(Z80_SP, 0);

	Z80_SP += 2;
	Z80_PC = z80.read_buffer[0] | (((uint16_t)(z80.read_buffer[1])) << 8);
	return Z80_STAGE_RESET;
}

///RETN; Size: 2; Flags: None
int RETN() {
	assert(z80.opcode_index == 2);
	assert(0); ///<-- Unimplemented
	return Z80_STAGE_RESET;
}

///RST y; Size: 1; Flags: None
int RST_y() {
	Z80_OPCODE_SUBDIV;
	assert(z80.opcode_index == 1);
	Z80_16BIT_WRITE(Z80_SP - 2, 0, Z80_PC & 0x00FF, (Z80_PC >> 8) & 0x00FF);
	Z80_SP = Z80_SP - 2;
	Z80_PC = 0x08 * y[0];
	return Z80_STAGE_RESET;
}


/* --- Extended opcodes --- */

int JP_IXYp() {
	assert(0); /*<-- Unimplemented*/
	return Z80_STAGE_RESET;
}