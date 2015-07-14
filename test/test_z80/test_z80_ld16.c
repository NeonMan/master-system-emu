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

TEST_GROUP(grp_ld16);

TEST_SETUP(grp_ld16){
    sms_ram = ramdbg_get_mem();
    z80dbg_set_pc_breakpoint_cb(pc_breakpoint_cb);

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
}

TEST_TEAR_DOWN(grp_ld16){
    
}

IGNORE_TEST(grp_ld16, LD_dd_nn){
    TEST_FAIL_MESSAGE("Unimplemented.");
}

IGNORE_TEST(grp_ld16, LD_IXY_nn){
    TEST_FAIL_MESSAGE("Unimplemented.");
}

IGNORE_TEST(grp_ld16, LD_HL_nnp){
    TEST_FAIL_MESSAGE("Unimplemented.");
}

IGNORE_TEST(grp_ld16, LD_dd_nnp){
    TEST_FAIL_MESSAGE("Unimplemented.");
}

IGNORE_TEST(grp_ld16, LD_IXY_nnp){
    TEST_FAIL_MESSAGE("Unimplemented.");
}

TEST(grp_ld16, LD_SP_HL){
    const uint8_t op_ld_sp_hl = 0xF9;
    sms_ram[0] = op_ld_sp_hl;
    //Test LD SP HL
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 1, Z80_BREAK_PC);
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_SP_EQUAL(Z80_HL);
    TEST_ASSERT_HL_EQUAL(0x0708);
}

TEST(grp_ld16, LD_SP_IXY){
    const uint8_t op_ld_sp_ix[2] = { 0xDD, 0xF9 };
    const uint8_t op_ld_sp_iy[2] = { 0xFD, 0xF9 };
    sms_ram[0] = op_ld_sp_ix[0];
    sms_ram[1] = op_ld_sp_ix[1];
    sms_ram[2] = op_ld_sp_iy[0];
    sms_ram[3] = op_ld_sp_iy[1];
    //Test LD SP IX
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 2, Z80_BREAK_PC);
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_SP_EQUAL(Z80_IX);
    TEST_ASSERT_IX_EQUAL(0x090A);
    //Test LD SP IY
    bp_triggered = 0;
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 4, Z80_BREAK_PC);
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_SP_EQUAL(Z80_IY);
    TEST_ASSERT_IY_EQUAL(0x0B0C);
}

TEST_GROUP_RUNNER(grp_ld16){
    RUN_TEST_CASE(grp_ld16, LD_dd_nn);
    RUN_TEST_CASE(grp_ld16, LD_IXY_nn);
    RUN_TEST_CASE(grp_ld16, LD_HL_nnp);
    RUN_TEST_CASE(grp_ld16, LD_dd_nnp);
    RUN_TEST_CASE(grp_ld16, LD_IXY_nnp);
    RUN_TEST_CASE(grp_ld16, LD_SP_HL);
    RUN_TEST_CASE(grp_ld16, LD_SP_IXY);
}

// ----------------------
// --- Helpers & Main ---
// ----------------------
//Helper 'run all' function
static void RunAllTests(void){
    RUN_TEST_GROUP(grp_ld16);
}

//Main
int main(int argc, const char** argv){
    int rv = UnityMain(argc, argv, RunAllTests);
    return rv;
}
