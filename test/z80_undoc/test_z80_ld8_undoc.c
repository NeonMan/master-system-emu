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

TEST_GROUP(grp_ld8_undoc);

TEST_SETUP(grp_ld8_undoc){
    sms_ram = ramdbg_get_mem();
    z80dbg_set_pc_breakpoint_cb(pc_breakpoint_cb);
    for (int i = 0; i < (256 * 256); ++i){
        z80dbg_set_breakpoint(i, 0);
    }

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

TEST_TEAR_DOWN(grp_ld8_undoc){

}

TEST(grp_ld8_undoc, ld_ixyh_n){
    const uint8_t op_ld_ixh_n[3] = { 0xDD, 0x26, 0xAA };
    const uint8_t op_ld_iyh_n[3] = { 0xFD, 0x26, 0xBB };

    sms_ram[0] = op_ld_ixh_n[0];
    sms_ram[1] = op_ld_ixh_n[1];
    sms_ram[2] = op_ld_ixh_n[2];

    sms_ram[3] = op_ld_iyh_n[0];
    sms_ram[4] = op_ld_iyh_n[1];
    sms_ram[5] = op_ld_iyh_n[2];

    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 6, Z80_BREAK_PC);

    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_IXH_EQUAL(0xAA);
    TEST_ASSERT_IYH_EQUAL(0xBB);
}

IGNORE_TEST(grp_ld8_undoc, noni_ld_r_n){
    TEST_FAIL_MESSAGE("Unimplemented");
}

TEST(grp_ld8_undoc, ld_ixyl_n){
    const uint8_t op_ld_ixl_n[3] = { 0xDD, 0x2E, 0xAA };
    const uint8_t op_ld_iyl_n[3] = { 0xFD, 0x2E, 0xBB };

    sms_ram[0] = op_ld_ixl_n[0];
    sms_ram[1] = op_ld_ixl_n[1];
    sms_ram[2] = op_ld_ixl_n[2];

    sms_ram[3] = op_ld_iyl_n[0];
    sms_ram[4] = op_ld_iyl_n[1];
    sms_ram[5] = op_ld_iyl_n[2];

    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 6, Z80_BREAK_PC);

    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_IXH_EQUAL(0xAA);
    TEST_ASSERT_IYH_EQUAL(0xBB);
}

TEST_GROUP_RUNNER(grp_ld8_undoc){
    RUN_TEST_CASE(grp_ld8_undoc, ld_ixyh_n);
    RUN_TEST_CASE(grp_ld8_undoc, ld_ixyh_n);
    RUN_TEST_CASE(grp_ld8_undoc, noni_ld_r_n);
}

// ----------------------
// --- Helpers & Main ---
// ----------------------
//Helper 'run all' function
static void RunAllTests(void){
    RUN_TEST_GROUP(grp_ld8_undoc);
}

//Main
int main(int argc, const char** argv){
    int rv = UnityMain(argc, argv, RunAllTests);
    return rv;
}
