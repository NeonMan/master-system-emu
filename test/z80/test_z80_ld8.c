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

TEST(grp_ld8, ld_r_n){
    //Load opcodes
    sms_ram[0]  = 0x06; sms_ram[1] = 0xAA;  //LD B, AAh
    sms_ram[2]  = 0x0E; sms_ram[3] = 0xAA;  //LD C, AAh
    sms_ram[4]  = 0x16; sms_ram[5] = 0xAA;  //LD D, AAh
    sms_ram[6]  = 0x1E; sms_ram[7] = 0xAA;  //LD E, AAh
    sms_ram[8]  = 0x26; sms_ram[9] = 0xAA;  //LD H, AAh
    sms_ram[10] = 0x2E; sms_ram[11] = 0xAA; //LD L, AAh
    sms_ram[12] = 0x3E; sms_ram[13] = 0xAA; //LD A, AAh

    //Set breakpoints
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (2 * 1), Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (2 * 2), Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (2 * 3), Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (2 * 4), Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (2 * 5), Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (2 * 6), Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (2 * 7), Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (2 * 8), Z80_BREAK_PC);

    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL_HEX8(0xAA, Z80_B);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL_HEX8(0xAA, Z80_C);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL_HEX8(0xAA, Z80_D);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL_HEX8(0xAA, Z80_E);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL_HEX8(0xAA, Z80_H);

    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL_HEX8(0xAA, Z80_L);

    bp_triggered = 0; tick_limit = 0x100;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL_HEX8(0xAA, Z80_A);
}

TEST(grp_ld8, ld_r_hlp){
    //Load opcodes
    sms_ram[0] = 0x46; //LD B, (HL)
    sms_ram[1] = 0x4E; //LD C, (HL)
    sms_ram[2] = 0x56; //LD D, (HL)
    sms_ram[3] = 0x5E; //LD E, (HL)
    sms_ram[4] = 0x66; //LD H, (HL)
    sms_ram[5] = 0x6E; //LD L, (HL)
    sms_ram[6] = 0x7E; //LD A, (HL)
    Z80_HL = 0xFFF0;

    //Set breakpoints
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 1, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 2, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 3, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 4, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 5, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 6, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 7, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 8, Z80_BREAK_PC);

    bp_triggered = 0; tick_limit = 0x100;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(0xAA, Z80_B);

    bp_triggered = 0; tick_limit = 0x100;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(0xAA, Z80_C);

    bp_triggered = 0; tick_limit = 0x100;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(0xAA, Z80_D);

    bp_triggered = 0; tick_limit = 0x100;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(0xAA, Z80_E);

    bp_triggered = 0; tick_limit = 0x100;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(0xAA, Z80_H);

    Z80_HL = 0xFFF0; /*Fix HL value*/
    bp_triggered = 0; tick_limit = 0x100;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(0xAA, Z80_L);

    Z80_HL = 0xFFF0; /*Fix HL value*/
    bp_triggered = 0; tick_limit = 0x100;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(0xAA, Z80_A);
}

TEST(grp_ld8, ld_r_ixyp_zero){
    const char op_ld_r_ix_s[3] = { 0xDD, 0x46, 0x00 }; /*LD B, (IX + 0)*/
    const char op_ld_r_iy_s[3] = { 0xFD, 0x46, 0x00 }; /*LD B, (IY + 0)*/

    //Test opcode with zero shift, all target regs.
    sms_ram[0 ] = op_ld_r_ix_s[0];              //
    sms_ram[1 ] = op_ld_r_ix_s[1] + (0x08 * 0); //
    sms_ram[2 ] = op_ld_r_ix_s[2];              // LD B, (IX + 0)
    sms_ram[3 ] = op_ld_r_iy_s[0];              //
    sms_ram[4 ] = op_ld_r_iy_s[1] + (0x08 * 0); //
    sms_ram[5 ] = op_ld_r_iy_s[2];              // LD B, (IY + 0)

    sms_ram[6 ] = op_ld_r_ix_s[0];              //
    sms_ram[7 ] = op_ld_r_ix_s[1] + (0x08 * 1); //
    sms_ram[8 ] = op_ld_r_ix_s[2];              // LD C, (IX + 0)
    sms_ram[9 ] = op_ld_r_iy_s[0];              //
    sms_ram[10] = op_ld_r_iy_s[1] + (0x08 * 1); //
    sms_ram[11] = op_ld_r_iy_s[2];              // LD C, (IY + 0)

    sms_ram[12] = op_ld_r_ix_s[0];              //
    sms_ram[13] = op_ld_r_ix_s[1] + (0x08 * 2); //
    sms_ram[14] = op_ld_r_ix_s[2];              // LD D, (IX + 0)
    sms_ram[15] = op_ld_r_iy_s[0];              //
    sms_ram[16] = op_ld_r_iy_s[1] + (0x08 * 2); //
    sms_ram[17] = op_ld_r_iy_s[2];              // LD D, (IY + 0)

    sms_ram[18] = op_ld_r_ix_s[0];              //
    sms_ram[19] = op_ld_r_ix_s[1] + (0x08 * 3); //
    sms_ram[20] = op_ld_r_ix_s[2];              // LD E, (IX + 0)
    sms_ram[21] = op_ld_r_iy_s[0];              //
    sms_ram[22] = op_ld_r_iy_s[1] + (0x08 * 3); //
    sms_ram[23] = op_ld_r_iy_s[2];              // LD E, (IY + 0)

    sms_ram[24] = op_ld_r_ix_s[0];              //
    sms_ram[25] = op_ld_r_ix_s[1] + (0x08 * 4); //
    sms_ram[26] = op_ld_r_ix_s[2];              // LD H, (IX + 0)
    sms_ram[27] = op_ld_r_iy_s[0];              //
    sms_ram[28] = op_ld_r_iy_s[1] + (0x08 * 4); //
    sms_ram[29] = op_ld_r_iy_s[2];              // LD H, (IY + 0)

    sms_ram[30] = op_ld_r_ix_s[0];              //
    sms_ram[31] = op_ld_r_ix_s[1] + (0x08 * 5); //
    sms_ram[32] = op_ld_r_ix_s[2];              // LD L, (IX + 0)
    sms_ram[33] = op_ld_r_iy_s[0];              //
    sms_ram[34] = op_ld_r_iy_s[1] + (0x08 * 5); //
    sms_ram[35] = op_ld_r_iy_s[2];              // LD L, (IY + 0)

    sms_ram[36] = op_ld_r_ix_s[0];              //
    sms_ram[37] = op_ld_r_ix_s[1] + (0x08 * 7); //
    sms_ram[38] = op_ld_r_ix_s[2];              // LD A, (IX + 0)
    sms_ram[39] = op_ld_r_iy_s[0];              //
    sms_ram[40] = op_ld_r_iy_s[1] + (0x08 * 7); //
    sms_ram[41] = op_ld_r_iy_s[2];              // LD A, (IY + 0)

    //Set IX and IY
    Z80_IX = 0xFFF0;
    Z80_IY = 0xFFF0;
    //Set breakpoints
    //z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (3 * 1), Z80_BREAK_PC);
    //z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (3 * 2), Z80_BREAK_PC);
    //z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (3 * 3), Z80_BREAK_PC);
    //z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (3 * 4), Z80_BREAK_PC);
    //z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (3 * 5), Z80_BREAK_PC);
    //z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (3 * 6), Z80_BREAK_PC);
    //z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (3 * 7), Z80_BREAK_PC);
    //z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (3 * 8), Z80_BREAK_PC);
    //z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (3 * 9), Z80_BREAK_PC);
    //z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (3 *10), Z80_BREAK_PC);
    //z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (3 *11), Z80_BREAK_PC);
    //z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (3 *12), Z80_BREAK_PC);
    //z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (3 *13), Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (3 *14), Z80_BREAK_PC);

    //Run opcodes
    tick_limit = 0x200;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL_HEX8(0xAA, Z80_B);
    TEST_ASSERT_EQUAL_HEX8(0xAA, Z80_C);
    TEST_ASSERT_EQUAL_HEX8(0xAA, Z80_D);
    TEST_ASSERT_EQUAL_HEX8(0xAA, Z80_E);
    TEST_ASSERT_EQUAL_HEX8(0xAA, Z80_H);
    TEST_ASSERT_EQUAL_HEX8(0xAA, Z80_L);
    TEST_ASSERT_EQUAL_HEX8(0xAA, Z80_A);
}

TEST(grp_ld8, ld_r_ixyp_max){
    const char op_ld_r_ix_s[3] = { 0xDD, 0x46, (int8_t) 127 }; /*LD B, (IX + 0)*/
    const char op_ld_r_iy_s[3] = { 0xFD, 0x46, (int8_t) 127 }; /*LD B, (IY + 0)*/

    //Test opcode with zero shift, all target regs.
    sms_ram[0] = op_ld_r_ix_s[0];              //
    sms_ram[1] = op_ld_r_ix_s[1] + (0x08 * 0); //
    sms_ram[2] = op_ld_r_ix_s[2];              // LD B, (IX + 127)
    sms_ram[3] = op_ld_r_iy_s[0];              //
    sms_ram[4] = op_ld_r_iy_s[1] + (0x08 * 1); //
    sms_ram[5] = op_ld_r_iy_s[2];              // LD C, (IY + 127)

    //Set IX and IY
    Z80_IX = 0xFFF0 - 127;
    Z80_IY = 0xFFF0 - 127;

    //Set BPs
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (3 * 2), Z80_BREAK_PC);

    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(0xAA, Z80_B);
    TEST_ASSERT_EQUAL(0xAA, Z80_C);
}

TEST(grp_ld8, ld_r_ixyp_min){
    const char op_ld_r_ix_s[3] = { 0xDD, 0x46, (int8_t)-128 }; /*LD B, (IX + 0)*/
    const char op_ld_r_iy_s[3] = { 0xFD, 0x46, (int8_t)-128 }; /*LD B, (IY + 0)*/

    //Test opcode with zero shift, all target regs.
    sms_ram[0] = op_ld_r_ix_s[0];              //
    sms_ram[1] = op_ld_r_ix_s[1] + (0x08 * 0); //
    sms_ram[2] = op_ld_r_ix_s[2];              // LD B, (IX + -128)
    sms_ram[3] = op_ld_r_iy_s[0];              //
    sms_ram[4] = op_ld_r_iy_s[1] + (0x08 * 1); //
    sms_ram[5] = op_ld_r_iy_s[2];              // LD C, (IY + -128)

    //Set IX and IY
    Z80_IX = (uint16_t)(0xFFF0 + 128);
    Z80_IY = (uint16_t)(0xFFF0 + 128);

    //Set BPs
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (3 * 2), Z80_BREAK_PC);

    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(0xAA, Z80_B);
    TEST_ASSERT_EQUAL(0xAA, Z80_C);
}

TEST(grp_ld8, ld_hlp_r){
    const uint8_t op_ld_hlp_r = 0x70;
    
    //Copy opcodes
    sms_ram[0] = op_ld_hlp_r + 0; //LD (HL), B
    sms_ram[1] = op_ld_hlp_r + 1; //LD (HL), C
    sms_ram[2] = op_ld_hlp_r + 2; //LD (HL), D
    sms_ram[3] = op_ld_hlp_r + 3; //LD (HL), E
    sms_ram[4] = op_ld_hlp_r + 4; //LD (HL), H
    sms_ram[5] = op_ld_hlp_r + 5; //LD (HL), L
    sms_ram[6] = op_ld_hlp_r + 7; //LD (HL), A

    //Set breakpoints
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 1, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 2, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 3, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 4, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 5, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 6, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 7, Z80_BREAK_PC);

    //SET HL
    Z80_HL = 0xFFF0;

    //LD (HL), B
    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_RAM_EQUAL(Z80_B, 0xFFF0);

    //LD (HL), C
    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_RAM_EQUAL(Z80_C, 0xFFF0);

    //LD (HL), D
    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_RAM_EQUAL(Z80_D, 0xFFF0);

    //LD (HL), E
    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_RAM_EQUAL(Z80_E, 0xFFF0);

    //LD (HL), H
    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_RAM_EQUAL(Z80_H, 0xFFF0);

    //LD (HL), L
    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_RAM_EQUAL(Z80_L, 0xFFF0);

    //LD (HL), A
    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_RAM_EQUAL(Z80_A, 0xFFF0);
}

TEST(grp_ld8, ld_ixyp_r_zero){
    const uint8_t op_ld_ixp_r[3] = { 0xDD, 0x70, 0x00 };
    const uint8_t op_ld_iyp_r[3] = { 0xFD, 0x70, 0x00 };

    //Copy opcodes
    sms_ram[0 ] = op_ld_ixp_r[0];     //
    sms_ram[1 ] = op_ld_ixp_r[1] + 0; //
    sms_ram[2 ] = op_ld_ixp_r[2];     // LD (IX+0), B
    sms_ram[3 ] = op_ld_iyp_r[0];     //
    sms_ram[4 ] = op_ld_iyp_r[1] + 0; //
    sms_ram[5 ] = op_ld_iyp_r[2];     // LD (IY+0), B

    sms_ram[6 ] = op_ld_ixp_r[0];     //
    sms_ram[7 ] = op_ld_ixp_r[1] + 1; //
    sms_ram[8 ] = op_ld_ixp_r[2];     // LD (IX+0), C
    sms_ram[9 ] = op_ld_iyp_r[0];     //
    sms_ram[10] = op_ld_iyp_r[1] + 1; //
    sms_ram[11] = op_ld_iyp_r[2];     // LD (IY+0), C
    
    sms_ram[12] = op_ld_ixp_r[0];     //
    sms_ram[13] = op_ld_ixp_r[1] + 2; //
    sms_ram[14] = op_ld_ixp_r[2];     // LD (IX+0), D
    sms_ram[15] = op_ld_iyp_r[0];     //
    sms_ram[16] = op_ld_iyp_r[1] + 2; //
    sms_ram[17] = op_ld_iyp_r[2];     // LD (IY+0), D
    
    sms_ram[18] = op_ld_ixp_r[0];     //
    sms_ram[19] = op_ld_ixp_r[1] + 3; //
    sms_ram[20] = op_ld_ixp_r[2];     // LD (IX+0), E
    sms_ram[21] = op_ld_iyp_r[0];     //
    sms_ram[22] = op_ld_iyp_r[1] + 3; //
    sms_ram[23] = op_ld_iyp_r[2];     // LD (IY+0), E
    
    sms_ram[24] = op_ld_ixp_r[0];     //
    sms_ram[25] = op_ld_ixp_r[1] + 4; //
    sms_ram[26] = op_ld_ixp_r[2];     // LD (IX+0), H
    sms_ram[27] = op_ld_iyp_r[0];     //
    sms_ram[28] = op_ld_iyp_r[1] + 4; //
    sms_ram[29] = op_ld_iyp_r[2];     // LD (IY+0), H
    
    sms_ram[30] = op_ld_ixp_r[0];     //
    sms_ram[31] = op_ld_ixp_r[1] + 5; //
    sms_ram[32] = op_ld_ixp_r[2];     // LD (IX+0), L
    sms_ram[33] = op_ld_iyp_r[0];     //
    sms_ram[34] = op_ld_iyp_r[1] + 5; //
    sms_ram[35] = op_ld_iyp_r[2];     // LD (IY+0), L
    
    sms_ram[36] = op_ld_ixp_r[0];     //
    sms_ram[37] = op_ld_ixp_r[1] + 7; //
    sms_ram[38] = op_ld_ixp_r[2];     // LD (IX+0), A
    sms_ram[39] = op_ld_iyp_r[0];     //
    sms_ram[40] = op_ld_iyp_r[1] + 7; //
    sms_ram[41] = op_ld_iyp_r[2];     // LD (IY+0), A

    //Set breakpoints
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (3 * 2), Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (3 * 4), Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (3 * 6), Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (3 * 8), Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (3 * 10), Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (3 * 12), Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + (3 * 14), Z80_BREAK_PC);

    //Set IX/IY and limits.
    Z80_IX = 0xFFF0;
    Z80_IY = 0xFFF2;
    tick_limit = 0x200;

    //LD (I? + 0), B
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_RAM_EQUAL(Z80_B, Z80_IX);
    TEST_ASSERT_RAM_EQUAL(Z80_B, Z80_IY);

    //LD (I? + 0), C
    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_RAM_EQUAL(Z80_C, Z80_IX);
    TEST_ASSERT_RAM_EQUAL(Z80_C, Z80_IY);

    //LD (I? + 0), D
    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_RAM_EQUAL(Z80_D, Z80_IX);
    TEST_ASSERT_RAM_EQUAL(Z80_D, Z80_IY);

    //LD (I? + 0), E
    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_RAM_EQUAL(Z80_E, Z80_IX);
    TEST_ASSERT_RAM_EQUAL(Z80_E, Z80_IY);

    //LD (I? + 0), H
    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_RAM_EQUAL(Z80_H, Z80_IX);
    TEST_ASSERT_RAM_EQUAL(Z80_H, Z80_IY);

    //LD (I? + 0), L
    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_RAM_EQUAL(Z80_L, Z80_IX);
    TEST_ASSERT_RAM_EQUAL(Z80_L, Z80_IY);

    //LD (I? + 0), A
    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_RAM_EQUAL(Z80_A, Z80_IX);
    TEST_ASSERT_RAM_EQUAL(Z80_A, Z80_IY);
}

TEST(grp_ld8, ld_ixyp_r_min){
    const uint8_t op_ld_ixp_r[3] = { 0xDD, 0x70, (int8_t) -128 };
    const uint8_t op_ld_iyp_r[3] = { 0xFD, 0x70, (int8_t) -128 };

    //Copy opcodes
    sms_ram[0] = op_ld_ixp_r[0]; //
    sms_ram[1] = op_ld_ixp_r[1]; //
    sms_ram[2] = op_ld_ixp_r[2]; // LD (IX-128), B
    sms_ram[3] = op_ld_iyp_r[0]; //
    sms_ram[4] = op_ld_iyp_r[1]; //
    sms_ram[5] = op_ld_iyp_r[2]; // LD (IY-128), B

    //Set breakpoint
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 6, Z80_BREAK_PC);

    //Set IX/IY
    Z80_IX = (uint16_t)(0xFFF0 + 128);
    Z80_IY = (uint16_t)(0xFFF2 + 128);

    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_RAM_EQUAL(Z80_B, 0xFFF0);
    TEST_ASSERT_RAM_EQUAL(Z80_B, 0xFFF2);
}

TEST(grp_ld8, ld_ixyp_r_max){
    const uint8_t op_ld_ixp_r[3] = { 0xDD, 0x70, (int8_t)127 };
    const uint8_t op_ld_iyp_r[3] = { 0xFD, 0x70, (int8_t)127 };

    //Copy opcodes
    sms_ram[0] = op_ld_ixp_r[0]; //
    sms_ram[1] = op_ld_ixp_r[1]; //
    sms_ram[2] = op_ld_ixp_r[2]; // LD (IX+127), B
    sms_ram[3] = op_ld_iyp_r[0]; //
    sms_ram[4] = op_ld_iyp_r[1]; //
    sms_ram[5] = op_ld_iyp_r[2]; // LD (IY+127), B

    //Set breakpoint
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 6, Z80_BREAK_PC);

    //Set IX/IY
    Z80_IX = (uint16_t)(0xFFF0 - 127);
    Z80_IY = (uint16_t)(0xFFF2 - 127);

    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_RAM_EQUAL(Z80_B, 0xFFF0);
    TEST_ASSERT_RAM_EQUAL(Z80_B, 0xFFF2);
}

TEST(grp_ld8, ld_hlp_n){
    const uint8_t op_ld_hlp_n[2] = { 0x36, 0xAA };

    //Copy opcode
    sms_ram[0] = op_ld_hlp_n[0];
    sms_ram[1] = op_ld_hlp_n[1];

    //Setup
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 2, Z80_BREAK_PC);
    Z80_HL = 0xFFF0;

    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_RAM_EQUAL(0xAA, 0xFFF0);
}

TEST(grp_ld8, ld_ixyp_n_zero){
    const uint8_t op_ld_ixp_n[4] = { 0xDD, 0x36, 0x00, 0xAA };
    const uint8_t op_ld_iyp_n[4] = { 0xFD, 0x36, 0x00, 0xAA };

    //Copy opcode
    sms_ram[0] = op_ld_ixp_n[0];
    sms_ram[1] = op_ld_ixp_n[1];
    sms_ram[2] = op_ld_ixp_n[2];
    sms_ram[3] = op_ld_ixp_n[3];

    sms_ram[4] = op_ld_iyp_n[0];
    sms_ram[5] = op_ld_iyp_n[1];
    sms_ram[6] = op_ld_iyp_n[2];
    sms_ram[7] = op_ld_iyp_n[3];

    //Setup
    Z80_IX = 0xFFF0;
    Z80_IY = 0xFFF2;
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 8, Z80_BREAK_PC);

    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_RAM_EQUAL(0xAA, Z80_IX);
    TEST_ASSERT_RAM_EQUAL(0xAA, Z80_IY);
}

TEST(grp_ld8, ld_ixyp_n_min){
    const uint8_t op_ld_ixp_n[4] = { 0xDD, 0x36, (int8_t)-128, 0xAA };
    const uint8_t op_ld_iyp_n[4] = { 0xFD, 0x36, (int8_t)-128, 0xAA };

    //Copy opcode
    sms_ram[0] = op_ld_ixp_n[0];
    sms_ram[1] = op_ld_ixp_n[1];
    sms_ram[2] = op_ld_ixp_n[2];
    sms_ram[3] = op_ld_ixp_n[3];

    sms_ram[4] = op_ld_iyp_n[0];
    sms_ram[5] = op_ld_iyp_n[1];
    sms_ram[6] = op_ld_iyp_n[2];
    sms_ram[7] = op_ld_iyp_n[3];

    //Setup
    Z80_IX = (uint16_t)(0xFFF0 + 128);
    Z80_IY = (uint16_t)(0xFFF2 + 128);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 8, Z80_BREAK_PC);

    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_RAM_EQUAL(0xAA, Z80_IX - 128);
    TEST_ASSERT_RAM_EQUAL(0xAA, Z80_IY - 128);
}

TEST(grp_ld8, ld_ixyp_n_max){
    const uint8_t op_ld_ixp_n[4] = { 0xDD, 0x36, (int8_t)127, 0xAA };
    const uint8_t op_ld_iyp_n[4] = { 0xFD, 0x36, (int8_t)127, 0xAA };

    //Copy opcode
    sms_ram[0] = op_ld_ixp_n[0];
    sms_ram[1] = op_ld_ixp_n[1];
    sms_ram[2] = op_ld_ixp_n[2];
    sms_ram[3] = op_ld_ixp_n[3];

    sms_ram[4] = op_ld_iyp_n[0];
    sms_ram[5] = op_ld_iyp_n[1];
    sms_ram[6] = op_ld_iyp_n[2];
    sms_ram[7] = op_ld_iyp_n[3];

    //Setup
    Z80_IX = (uint16_t)(0xFFF0 - 127);
    Z80_IY = (uint16_t)(0xFFF2 - 127);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 8, Z80_BREAK_PC);

    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_RAM_EQUAL(0xAA, Z80_IX + 127);
    TEST_ASSERT_RAM_EQUAL(0xAA, Z80_IY + 127);
}

TEST(grp_ld8, ld_a_bcp){
    const uint8_t op_ld_a_bcp = 0x0A;

    sms_ram[0] = op_ld_a_bcp;
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 1, Z80_BREAK_PC);

    Z80_BC = 0xFFF0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_A_EQUAL(0xAA);
}

TEST(grp_ld8, ld_a_dep){
    const uint8_t op_ld_a_dep = 0x1A;

    sms_ram[0] = op_ld_a_dep;
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 1, Z80_BREAK_PC);

    Z80_DE = 0xFFF0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_A_EQUAL(0xAA);
}

TEST(grp_ld8, ld_a_nnp){
    const uint8_t op_ld_a_nnp[3] = { 0x3A, 0xF0, 0xFF };

    sms_ram[0] = op_ld_a_nnp[0];
    sms_ram[1] = op_ld_a_nnp[1];
    sms_ram[2] = op_ld_a_nnp[2];
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 3, Z80_BREAK_PC);

    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_A_EQUAL(0xAA);
}

TEST(grp_ld8, ld_bcp_a){
    const uint8_t op_ld_bcp_a = 0x02;
    sms_ram[0] = op_ld_bcp_a;
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 1, Z80_BREAK_PC);
    Z80_BC = 0xFFF0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_RAM_EQUAL(Z80_A, 0xFFF0);
}

TEST(grp_ld8, ld_dep_a){
    const uint8_t op_ld_dep_a = 0x12;
    sms_ram[0] = op_ld_dep_a;
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 1, Z80_BREAK_PC);
    Z80_DE = 0xFFF0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_RAM_EQUAL(Z80_A, 0xFFF0);
}

TEST(grp_ld8, ld_nnp_a){
    const uint8_t op_ld_nnp_a[3] = { 0x32, 0xF0, 0xFF };
    sms_ram[0] = op_ld_nnp_a[0];
    sms_ram[1] = op_ld_nnp_a[1];
    sms_ram[2] = op_ld_nnp_a[2];
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 3, Z80_BREAK_PC);
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_RAM_EQUAL(Z80_A, 0xFFF0);
}

TEST(grp_ld8, ld_i_a){
    const uint8_t op_ld_i_a[2] = { 0xED, 0x47 };
    sms_ram[0] = op_ld_i_a[0];
    sms_ram[1] = op_ld_i_a[1];
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 2, Z80_BREAK_PC);
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_A, Z80_I);
}

IGNORE_TEST(grp_ld8, ld_r_a){
    const uint8_t op_ld_r_a[2] = { 0xED, 0x4F };
    sms_ram[0] = op_ld_r_a[0];
    sms_ram[1] = op_ld_r_a[1];
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 2, Z80_BREAK_PC);
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered); TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_EQUAL(Z80_A, Z80_R);
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
    RUN_TEST_CASE(grp_ld8, ld_r_ixyp_zero);
    RUN_TEST_CASE(grp_ld8, ld_r_ixyp_max);
    RUN_TEST_CASE(grp_ld8, ld_r_ixyp_min);
    RUN_TEST_CASE(grp_ld8, ld_hlp_r);
    RUN_TEST_CASE(grp_ld8, ld_ixyp_r_zero);
    RUN_TEST_CASE(grp_ld8, ld_ixyp_r_max);
    RUN_TEST_CASE(grp_ld8, ld_ixyp_r_min);
    RUN_TEST_CASE(grp_ld8, ld_hlp_n);
    RUN_TEST_CASE(grp_ld8, ld_ixyp_n_zero);
    RUN_TEST_CASE(grp_ld8, ld_ixyp_n_min);
    RUN_TEST_CASE(grp_ld8, ld_ixyp_n_max);
    RUN_TEST_CASE(grp_ld8, ld_a_bcp);
    RUN_TEST_CASE(grp_ld8, ld_a_dep);
    RUN_TEST_CASE(grp_ld8, ld_a_nnp);
    RUN_TEST_CASE(grp_ld8, ld_bcp_a);
    RUN_TEST_CASE(grp_ld8, ld_dep_a);
    RUN_TEST_CASE(grp_ld8, ld_nnp_a);
    RUN_TEST_CASE(grp_ld8, ld_i_a);
    RUN_TEST_CASE(grp_ld8, ld_r_a);
    RUN_TEST_CASE(grp_ld8, ld_a_i); /*Beware of flags!*/
    RUN_TEST_CASE(grp_ld8, ld_a_r); /*Beware of flags!*/
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
