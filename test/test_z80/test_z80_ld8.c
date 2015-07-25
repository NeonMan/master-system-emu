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

TEST_GROUP(grp_ld8);

TEST_SETUP(grp_ld8){
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

    _set_ram(0xFFF0, 0xAA);
    _set_ram(0xFFF1, 0xBB);
    _set_ram(0xFFF2, 0xCC);
    _set_ram(0xFFF3, 0xDD);
    _set_ram(0xFFF4, 0xEE);
    _set_ram(0xFFF5, 0xFF);
    _set_ram(0xFFF6, 0x11);
    _set_ram(0xFFF7, 0x22);
}

TEST_TEAR_DOWN(grp_ld8){

}

// --- Test cases ---
TEST(grp_ld8, ld_B_r){
    //Copy opcodes
    sms_ram[0] = 0x40; //LD B, B
    sms_ram[1] = 0x41; //LD B, C
    sms_ram[2] = 0x42; //LD B, D
    sms_ram[3] = 0x43; //LD B, E
    sms_ram[4] = 0x44; //LD B, H
    sms_ram[5] = 0x45; //LD B, L
    sms_ram[6] = 0x47; //LD B, A

    //Set breakpoints
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 1, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 2, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 3, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 4, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 5, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 6, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 7, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 8, Z80_BREAK_PC);

    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(0x03,  Z80_B);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_C, Z80_B);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_D, Z80_B);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_E, Z80_B);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_H, Z80_B);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_L, Z80_B);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_A, Z80_B);
}

TEST(grp_ld8, ld_C_r){
    //Copy opcodes
    sms_ram[0] = 0x48; //LD C, B
    sms_ram[1] = 0x49; //LD C, C
    sms_ram[2] = 0x4A; //LD C, D
    sms_ram[3] = 0x4B; //LD C, E
    sms_ram[4] = 0x4C; //LD C, H
    sms_ram[5] = 0x4D; //LD C, L
    sms_ram[6] = 0x4F; //LD C, A

    //Set breakpoints
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 1, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 2, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 3, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 4, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 5, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 6, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 7, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 8, Z80_BREAK_PC);

    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_B, Z80_C);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_B, Z80_C); /*Register keeps the previous value*/

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_D, Z80_C);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_E, Z80_C);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_H, Z80_C);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_L, Z80_C);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_A, Z80_C);
}

TEST(grp_ld8, ld_D_r){
    //Copy opcodes
    sms_ram[0] = 0x50; //LD D, B
    sms_ram[1] = 0x51; //LD D, C
    sms_ram[2] = 0x52; //LD D, D
    sms_ram[3] = 0x53; //LD D, E
    sms_ram[4] = 0x54; //LD D, H
    sms_ram[5] = 0x55; //LD D, L
    sms_ram[6] = 0x57; //LD D, A

    //Set breakpoints
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 1, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 2, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 3, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 4, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 5, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 6, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 7, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 8, Z80_BREAK_PC);

    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_B, Z80_D);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_C, Z80_D);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_C, Z80_D); /*Register keeps the previous value*/

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_E, Z80_D);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_H, Z80_D);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_L, Z80_D);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_A, Z80_D);
}

TEST(grp_ld8, ld_E_r){
    //Copy opcodes
    sms_ram[0] = 0x58; //LD E, B
    sms_ram[1] = 0x59; //LD E, C
    sms_ram[2] = 0x5A; //LD E, D
    sms_ram[3] = 0x5B; //LD E, E
    sms_ram[4] = 0x5C; //LD E, H
    sms_ram[5] = 0x5D; //LD E, L
    sms_ram[6] = 0x5F; //LD E, A

    //Set breakpoints
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 1, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 2, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 3, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 4, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 5, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 6, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 7, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 8, Z80_BREAK_PC);

    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_B, Z80_E);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_C, Z80_E);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_D, Z80_E);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_D, Z80_E); /*Register keeps the previous value*/

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_H, Z80_E);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_L, Z80_E);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_A, Z80_E);
}

TEST(grp_ld8, ld_H_r){
    //Copy opcodes
    sms_ram[0] = 0x60; //LD H, B
    sms_ram[1] = 0x61; //LD H, C
    sms_ram[2] = 0x62; //LD H, D
    sms_ram[3] = 0x63; //LD H, E
    sms_ram[4] = 0x64; //LD H, H
    sms_ram[5] = 0x65; //LD H, L
    sms_ram[6] = 0x67; //LD H, A

    //Set breakpoints
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 1, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 2, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 3, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 4, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 5, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 6, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 7, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 8, Z80_BREAK_PC);

    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_B, Z80_H);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_C, Z80_H);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_D, Z80_H);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_E, Z80_H);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_E, Z80_H); /*Register keeps the previous value*/

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_L, Z80_H);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_A, Z80_H);
}

TEST(grp_ld8, ld_L_r){
    //Copy opcodes
    sms_ram[0] = 0x68; //LD L, B
    sms_ram[1] = 0x69; //LD L, C
    sms_ram[2] = 0x6A; //LD L, D
    sms_ram[3] = 0x6B; //LD L, E
    sms_ram[4] = 0x6C; //LD L, H
    sms_ram[5] = 0x6D; //LD L, L
    sms_ram[6] = 0x6F; //LD L, A

    //Set breakpoints
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 1, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 2, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 3, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 4, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 5, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 6, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 7, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 8, Z80_BREAK_PC);

    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_B, Z80_L);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_C, Z80_L);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_D, Z80_L);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_E, Z80_L);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_H, Z80_L);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_H, Z80_L); /*Register keeps the previous value*/

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_A, Z80_L);
}

TEST(grp_ld8, ld_A_r){
    //Copy opcodes
    sms_ram[0] = 0x78; //LD L, B
    sms_ram[1] = 0x79; //LD L, C
    sms_ram[2] = 0x7A; //LD L, D
    sms_ram[3] = 0x7B; //LD L, E
    sms_ram[4] = 0x7C; //LD L, H
    sms_ram[5] = 0x7D; //LD L, L
    sms_ram[6] = 0x7F; //LD L, A

    //Set breakpoints
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 1, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 2, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 3, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 4, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 5, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 6, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 7, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 8, Z80_BREAK_PC);

    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_B, Z80_A);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_C, Z80_A);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_D, Z80_A);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_E, Z80_A);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_H, Z80_A);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_L, Z80_A);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_L, Z80_A); /*Register keeps the previous value*/
}


IGNORE_TEST(grp_ld8, ld_r_n){
    TEST_FAIL_MESSAGE("Unimplemented");
}

IGNORE_TEST(grp_ld8, ld_r_hlp){
    TEST_FAIL_MESSAGE("Unimplemented");
}

IGNORE_TEST(grp_ld8, ld_r_ixyp){
    TEST_FAIL_MESSAGE("Unimplemented");
}

IGNORE_TEST(grp_ld8, ld_hlp_r){
    TEST_FAIL_MESSAGE("Unimplemented");
}

IGNORE_TEST(grp_ld8, ld_ixyp_r){
    TEST_FAIL_MESSAGE("Unimplemented");
}

IGNORE_TEST(grp_ld8, ld_hlp_n){
    TEST_FAIL_MESSAGE("Unimplemented");
}

IGNORE_TEST(grp_ld8, ld_ixyp_n){
    TEST_FAIL_MESSAGE("Unimplemented");
}

IGNORE_TEST(grp_ld8, ld_a_bcp){
    TEST_FAIL_MESSAGE("Unimplemented");
}

IGNORE_TEST(grp_ld8, ld_a_dep){
    TEST_FAIL_MESSAGE("Unimplemented");
}

IGNORE_TEST(grp_ld8, ld_a_nnp){
    TEST_FAIL_MESSAGE("Unimplemented");
}

IGNORE_TEST(grp_ld8, ld_bcp_a){
    TEST_FAIL_MESSAGE("Unimplemented");
}

IGNORE_TEST(grp_ld8, ld_dep_a){
    TEST_FAIL_MESSAGE("Unimplemented");
}

IGNORE_TEST(grp_ld8, ld_nnp_a){
    TEST_FAIL_MESSAGE("Unimplemented");
}

IGNORE_TEST(grp_ld8, ld_i_a){
    TEST_FAIL_MESSAGE("Unimplemented");
}

IGNORE_TEST(grp_ld8, ld_r_a){
    TEST_FAIL_MESSAGE("Unimplemented");
}

/*Beware of flags!*/
IGNORE_TEST(grp_ld8, ld_a_i){
    TEST_FAIL_MESSAGE("Unimplemented");
}

/*Beware of flags!*/
IGNORE_TEST(grp_ld8, ld_a_r){
    TEST_FAIL_MESSAGE("Unimplemented");
}

TEST_GROUP_RUNNER(grp_ld8){
    RUN_TEST_CASE(grp_ld8, ld_B_r);
    RUN_TEST_CASE(grp_ld8, ld_C_r);
    RUN_TEST_CASE(grp_ld8, ld_D_r);
    RUN_TEST_CASE(grp_ld8, ld_E_r);
    RUN_TEST_CASE(grp_ld8, ld_H_r);
    RUN_TEST_CASE(grp_ld8, ld_L_r);
    RUN_TEST_CASE(grp_ld8, ld_A_r);

    RUN_TEST_CASE(grp_ld8, ld_r_n);
    RUN_TEST_CASE(grp_ld8, ld_r_hlp);
    RUN_TEST_CASE(grp_ld8, ld_r_ixyp);
    RUN_TEST_CASE(grp_ld8, ld_hlp_r);
    RUN_TEST_CASE(grp_ld8, ld_ixyp_r);
    RUN_TEST_CASE(grp_ld8, ld_hlp_n);
    RUN_TEST_CASE(grp_ld8, ld_ixyp_n);
    RUN_TEST_CASE(grp_ld8, ld_a_bcp);
    RUN_TEST_CASE(grp_ld8, ld_a_dep);
    RUN_TEST_CASE(grp_ld8, ld_a_nnp);
    RUN_TEST_CASE(grp_ld8, ld_bcp_a);
    RUN_TEST_CASE(grp_ld8, ld_dep_a);
    RUN_TEST_CASE(grp_ld8, ld_nnp_a);
    RUN_TEST_CASE(grp_ld8, ld_a_i); /*Beware of flags!*/
    RUN_TEST_CASE(grp_ld8, ld_a_r); /*Beware of flags!*/
    RUN_TEST_CASE(grp_ld8, ld_i_a);
    RUN_TEST_CASE(grp_ld8, ld_r_a);
}

// ----------------------
// --- Helpers & Main ---
// ----------------------
//Helper 'run all' function
static void RunAllTests(void){
    RUN_TEST_GROUP(grp_ld8);
}

//Main
int main(int argc, const char** argv){
    int rv = UnityMain(argc, argv, RunAllTests);
    return rv;
}
