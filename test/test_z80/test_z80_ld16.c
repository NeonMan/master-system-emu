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

    _set_ram(0xFFF0, 0xAA);
    _set_ram(0xFFF1, 0xBB);
    _set_ram(0xFFF2, 0xCC);
    _set_ram(0xFFF3, 0xDD);
    _set_ram(0xFFF4, 0xEE);
    _set_ram(0xFFF5, 0xFF);
    _set_ram(0xFFF6, 0x11);
    _set_ram(0xFFF7, 0x22);
}

TEST_TEAR_DOWN(grp_ld16){
    
}

TEST(grp_ld16, LD_dd_nn){
    const char op_ld_bc_nn[3] = { 0x01, 0x55, 0xAA };
    const char op_ld_de_nn[3] = { 0x11, 0x44, 0xBB };
    const char op_ld_hl_nn[3] = { 0x21, 0x33, 0xCC };
    const char op_ld_sp_nn[3] = { 0x31, 0x22, 0xDD };
    //Copy opcodes, back-to-back
    sms_ram[0] = op_ld_bc_nn[0]; sms_ram[1]  = op_ld_bc_nn[1]; sms_ram[2]  = op_ld_bc_nn[2];
    sms_ram[3] = op_ld_de_nn[0]; sms_ram[4]  = op_ld_de_nn[1]; sms_ram[5]  = op_ld_de_nn[2];
    sms_ram[6] = op_ld_hl_nn[0]; sms_ram[7]  = op_ld_hl_nn[1]; sms_ram[8]  = op_ld_hl_nn[2];
    sms_ram[9] = op_ld_sp_nn[0]; sms_ram[10] = op_ld_sp_nn[1]; sms_ram[11] = op_ld_sp_nn[2];
    //Set breakpoints
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS +  3, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS +  6, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS +  9, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 12, Z80_BREAK_PC);
    //Test LD BC nn
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_BC_EQUAL(0xAA55);
    //Test LD DE nn
    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_DE_EQUAL(0xBB44);
    //Test LD HL nn
    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_HL_EQUAL(0xCC33);
    //Test LD SP nn
    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_SP_EQUAL(0xDD22);
}

TEST(grp_ld16, LD_IXY_nn){
    const char op_ld_ix_nn[4] = { 0xDD, 0x21, 0x22, 0xDD };
    const char op_ld_iy_nn[4] = { 0xFD, 0x21, 0x11, 0xEE };
    //Copy opcodes, back-to-back
    sms_ram[0] = op_ld_ix_nn[0];
    sms_ram[1] = op_ld_ix_nn[1];
    sms_ram[2] = op_ld_ix_nn[2];
    sms_ram[3] = op_ld_ix_nn[3];

    sms_ram[4] = op_ld_iy_nn[0];
    sms_ram[5] = op_ld_iy_nn[1];
    sms_ram[6] = op_ld_iy_nn[2];
    sms_ram[7] = op_ld_iy_nn[3];
    //Set breakpoints
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 4, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 8, Z80_BREAK_PC);
    //LD IX nn
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_IX_EQUAL(0xDD22);
    //LD IY nn
    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_IY_EQUAL(0xEE11);
}

TEST(grp_ld16, LD_HL_nnp){
    const uint8_t op_ld_hl_nnp[3] = { 0x2A, 0xF0, 0xFF };
    //Copy opcode
    sms_ram[0] = op_ld_hl_nnp[0];
    sms_ram[1] = op_ld_hl_nnp[1];
    sms_ram[2] = op_ld_hl_nnp[2];
    //Set BP
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 3, Z80_BREAK_PC);
    //LD HL (nn)
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_HL_EQUAL(0xBBAA);
}

TEST(grp_ld16, LD_dd_nnp){
    const uint8_t op_ld_bc_nnp[4] = { 0xED, 0x4B, 0xF0, 0xFF }; /*Reads: 0xBBAA*/
    const uint8_t op_ld_de_nnp[4] = { 0xED, 0x5B, 0xF2, 0xFF }; /*Reads: 0xDDCC*/
    const uint8_t op_ld_hl_nnp[4] = { 0xED, 0x6B, 0xF4, 0xFF }; /*Reads: 0xFFEE*/
    const uint8_t op_ld_sp_nnp[4] = { 0xED, 0x7B, 0xF6, 0xFF }; /*Reads: 0x2211*/
    //copy opcodes
    memcpy(sms_ram, op_ld_bc_nnp, 4);
    memcpy(sms_ram  +4, op_ld_de_nnp, 4);
    memcpy(sms_ram  +8, op_ld_hl_nnp, 4);
    memcpy(sms_ram +12, op_ld_sp_nnp, 4);
    //Set BPs
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 4, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 8, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS +12, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS +16, Z80_BREAK_PC);
    //LD BC (nn)
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_BC_EQUAL(0xBBAA);
    //LD DE (nn)
    bp_triggered = 0; tick_limit = 100;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_DE_EQUAL(0xDDCC);
    //LD HL (nn)
    bp_triggered = 0; tick_limit = 100;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_HL_EQUAL(0xFFEE);
    //LD SP (nn)
    bp_triggered = 0; tick_limit = 100;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_SP_EQUAL(0x2211);
}

TEST(grp_ld16, LD_IXY_nnp){
    const uint8_t op_ld_ix_nnp[4] = { 0xDD, 0x2A, 0xF0, 0xFF };
    const uint8_t op_ld_iy_nnp[4] = { 0xFD, 0x2A, 0xF2, 0xFF };
    //Copy opcode
    sms_ram[0] = op_ld_ix_nnp[0]; sms_ram[4] = op_ld_iy_nnp[0];
    sms_ram[1] = op_ld_ix_nnp[1]; sms_ram[5] = op_ld_iy_nnp[1];
    sms_ram[2] = op_ld_ix_nnp[2]; sms_ram[6] = op_ld_iy_nnp[2];
    sms_ram[3] = op_ld_ix_nnp[3]; sms_ram[7] = op_ld_iy_nnp[3];
    //Set BPs
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 4, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 8, Z80_BREAK_PC);
    //LD IX (nn)
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_IX_EQUAL(0xBBAA);
    //LD IY (nn)
    bp_triggered = 0;
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_IY_EQUAL(0xDDCC);
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

TEST(grp_ld16, LD_nnp_HL){
    TEST_FAIL_MESSAGE("Unimplemented test!");
}

TEST(grp_ld16, LD_nnp_dd){
    TEST_FAIL_MESSAGE("Unimplemented test!");
}

TEST(grp_ld16, LD_nnp_IXY){
    TEST_FAIL_MESSAGE("Unimplemented test!");
}

TEST_GROUP_RUNNER(grp_ld16){
    RUN_TEST_CASE(grp_ld16, LD_dd_nn);
    RUN_TEST_CASE(grp_ld16, LD_IXY_nn);
    RUN_TEST_CASE(grp_ld16, LD_HL_nnp);
    RUN_TEST_CASE(grp_ld16, LD_dd_nnp);
    RUN_TEST_CASE(grp_ld16, LD_IXY_nnp);
    RUN_TEST_CASE(grp_ld16, LD_SP_HL);
    RUN_TEST_CASE(grp_ld16, LD_SP_IXY);
    RUN_TEST_CASE(grp_ld16, LD_nnp_HL);
    RUN_TEST_CASE(grp_ld16, LD_nnp_dd);
    RUN_TEST_CASE(grp_ld16, LD_nnp_IXY);
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
