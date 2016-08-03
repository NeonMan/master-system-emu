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

#include "test_z80.h"

TEST_GROUP(z80_block);

TEST_SETUP(z80_block){
    sms_ram = ramdbg_get_mem();
    z80dbg_set_pc_breakpoint_cb(pc_breakpoint_cb);
	//Clear breakpoints
	z80dbg_clear_breakpoints();

    memset(sms_ram, 0x00, RAM_SIZE);
    memset(&z80, 0x00, sizeof(z80));
    bp_triggered = 0;
    tick_limit = 100;
    io_stat = 0;

    Z80_AF = 0x0102; Z80_AFp = 0xF1F2;
    Z80_BC = 0x0304; Z80_BCp = 0xF3F4;
    Z80_DE = 0x0506; Z80_DEp = 0xF5F6;
    Z80_HL = 0x0708; Z80_HLp = 0xF7F8;
    Z80_IX = 0x090A;
    Z80_IY = 0x0B0C;

    Z80_SP = 0xFFF0;
    Z80_PC = RAM_BASE_ADDRESS;

    _set_ram(0xFFF0, 0xAA);
    _set_ram(0xFFF1, 0xBB);
    _set_ram(0xFFF2, 0xCC);
    _set_ram(0xFFF3, 0xDD);
    _set_ram(0xFFF4, 0xEE);
    _set_ram(0xFFF5, 0xFF);
    _set_ram(0xFFF6, 0x11);
    _set_ram(0xFFF7, 0x22);
}

TEST_TEAR_DOWN(z80_block){

}

IGNORE_TEST(z80_block, op_ldi){
    const uint8_t op_ldi[2] = { 0xED, 0xA0 };
    TEST_FAIL_MESSAGE("Unimplemented");
}

TEST_GROUP_RUNNER(z80_block){
    RUN_TEST_CASE(z80_block, op_ldi);
}

// ----------------------
// --- Helpers & Main ---
// ----------------------
//Helper 'run all' function
static void RunAllTests(void){
    RUN_TEST_GROUP(z80_block);
}

//Main
int main(int argc, const char** argv){
    int rv = UnityMain(argc, argv, RunAllTests);
    return rv;
}
