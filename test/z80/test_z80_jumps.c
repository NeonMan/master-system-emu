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

/*
Opcodes tested here.
DJNZ
*/

#include "test_z80.h"

/* ------------------ */
/* --- Jump group --- */
/* ------------------ */

TEST_GROUP(jumps);

TEST_SETUP(jumps) {
	//Setup state
	sms_ram = ramdbg_get_mem();
	io_stat = IO_RAM ^ 0xFF;
	memset(&z80, 0, sizeof(struct z80_s));
	z80.rSP = 0xFFF0;
	z80.rPC = RAM_BASE_ADDRESS;
	bp_triggered = 0;
	tick_limit = 100;
	memset(sms_ram, 0xFF, RAM_SIZE);

	//Set known values at the stacktop
	_set_ram(Z80_SP + 0, 0xAA);
	_set_ram(Z80_SP + 1, 0xBB);

	//Setup register values
	Z80_A = 0x01;
	Z80_F = 0x02;
	Z80_B = 0x03;
	Z80_C = 0x04;
	Z80_D = 0x05;
	Z80_E = 0x06;
	Z80_H = 0x07;
	Z80_L = 0x08;
	Z80_IX = 0x1011;
	Z80_IY = 0x1213;

	//Setup callbacks
	z80dbg_set_pc_breakpoint_cb(pc_breakpoint_cb);
}

TEST_TEAR_DOWN(jumps) {
	//Clear breakpoints
	for (int i = 0; i < (256 * 256); ++i) {
		z80dbg_set_breakpoint(i, 0);
	}
}

TEST(jumps, DJNZ) {
	const uint8_t op[2] = { 0x10, 0x7F }; //DJNZ +127
	const uint16_t skip_pc = 2;
	const uint16_t jump_pc = 127 + 2;

	
	sms_ram[0] = op[0];
	sms_ram[1] = op[1]; //Relative jump +127
	Z80_B = 1;

	z80dbg_set_breakpoint(RAM_BASE_ADDRESS + skip_pc, Z80_BREAK_PC);
	z80dbg_set_breakpoint(RAM_BASE_ADDRESS + jump_pc, Z80_BREAK_PC);

	//Run, jump must be taken
	__RUN_TEST_OPCODES;
	TEST_ASSERT_TRUE(tick_limit > 0);
	TEST_ASSERT_TRUE(bp_triggered);
	TEST_ASSERT_PC_EQUAL(RAM_BASE_ADDRESS + jump_pc);

	//Copy opcode. Jump must NOT be taken
	sms_ram[(jump_pc)+0] = op[0];
	sms_ram[(jump_pc)+1] = op[1];

	bp_triggered = 0;
	z80dbg_set_breakpoint(RAM_BASE_ADDRESS + jump_pc + skip_pc, Z80_BREAK_PC);
	z80dbg_set_breakpoint(RAM_BASE_ADDRESS + jump_pc + jump_pc, Z80_BREAK_PC);

	//Run, jump must not be taken
	__RUN_TEST_OPCODES;
	TEST_ASSERT_TRUE(tick_limit > 0);
	TEST_ASSERT_TRUE(bp_triggered);
	TEST_ASSERT_PC_EQUAL(RAM_BASE_ADDRESS + jump_pc + skip_pc);
}

TEST_GROUP_RUNNER(jumps) {
	RUN_TEST_CASE(jumps, DJNZ);
}

// ----------------------
// --- Helpers & Main ---
// ----------------------
//Helper 'run all' function
static void RunAllTests(void) {
	RUN_TEST_GROUP(jumps);
}

//Main
int main(int argc, const char** argv) {
	int rv = UnityMain(argc, argv, RunAllTests);
	return rv;
}