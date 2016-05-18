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
 PUSH
 POP
 CALL nn
 CALL cc nn
 RET (Except RETN)
 RET cc
 RST
*/

#include "test_z80.h"

/* ----------------------------------- */
/* --- PUSH Group (Push and calls) --- */
/* ----------------------------------- */
TEST_GROUP(stack_push);

/* --- Test cases --- */
TEST_SETUP(stack_push){
    //Setup state
    sms_ram = ramdbg_get_mem();
    io_stat = IO_RAM ^ 0xFF;
    memset(&z80, 0, sizeof(struct z80_s));
    z80.rSP = 0xFFF0;
    z80.rPC = RAM_BASE_ADDRESS;
    bp_triggered = 0;
    tick_limit = 100;
    memset(sms_ram, 0xFF, RAM_SIZE);

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

TEST_TEAR_DOWN(stack_push){
    //Clear breakpoints
    for (int i = 0; i < (256 * 256); ++i){
        z80dbg_set_breakpoint(i, 0);
    }
}

// ------------
// --- PUSH ---
// ------------
TEST(stack_push, PUSH_BC){
    const uint8_t op_push_bc = 0xC5;
    //PUSH BC
    sms_ram[0] = op_push_bc; /*<-- Copy opcode to ram*/
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + sizeof(op_push_bc), Z80_BREAK_PC); /*<-- Set breakpoint*/
    __RUN_TEST_OPCODES;
    //Test results
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_SP_EQUAL(0xFFF0 - 2);
    TEST_ASSERT_STACKTOP_EQUAL16(Z80_BC);
}

TEST(stack_push, PUSH_DE){
    const uint8_t op_push_de = 0xD5;
    const uint8_t op_pop_de = 0xD1;
    //PUSH DE
    sms_ram[0] = op_push_de; /*<-- Copy opcode to ram*/
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + sizeof(op_push_de), Z80_BREAK_PC); /*<-- Set breakpoint*/
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_SP_EQUAL(0xFFF0 - 2);
    TEST_ASSERT_STACKTOP_EQUAL16(Z80_DE);
}

TEST(stack_push, PUSH_HL){
    const uint8_t op_push_hl = 0xE5;
    const uint8_t op_pop_hl = 0xE1;
    //PUSH HL
    sms_ram[0] = op_push_hl; /*<-- Copy opcode to ram*/
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + sizeof(op_push_hl), Z80_BREAK_PC); /*<-- Set breakpoint*/
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_SP_EQUAL(0xFFF0 - 2);
    TEST_ASSERT_STACKTOP_EQUAL16(Z80_HL);
}

TEST(stack_push, PUSH_AF){
    const uint8_t op_push_af = 0xF5;
    const uint8_t op_pop_af = 0xF1;
    //PUSH AF
    sms_ram[0] = op_push_af; /*<-- Copy opcode to ram*/
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + sizeof(op_push_af), Z80_BREAK_PC); /*<-- Set breakpoint*/
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_SP_EQUAL(0xFFF0 - 2);
    TEST_ASSERT_STACKTOP_EQUAL16(Z80_AF);
}

TEST(stack_push, PUSH_IX){
    const uint8_t op_push_ix[2] = { 0xDD, 0xE5 };
    //PUSH IX
    sms_ram[0] = op_push_ix[0]; /**/
    sms_ram[1] = op_push_ix[1]; /*<-- Copy opcode to ram*/
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + sizeof(op_push_ix), Z80_BREAK_PC); /*<-- Set breakpoint*/
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_SP_EQUAL(0xFFF0 - 2);
    TEST_ASSERT_STACKTOP_EQUAL16(Z80_IX);
}

TEST(stack_push, PUSH_IY){
    const uint8_t op_push_iy[2] = { 0xFD, 0xE5 };
    //PUSH IY
    sms_ram[0] = op_push_iy[0]; /**/
    sms_ram[1] = op_push_iy[1]; /*<-- Copy opcode to ram*/
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + sizeof(op_push_iy), Z80_BREAK_PC); /*<-- Set breakpoint*/
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_SP_EQUAL(0xFFF0 - 2);
    TEST_ASSERT_STACKTOP_EQUAL16(Z80_IY);
}

// --------------------------
// --- Unconditional call ---
// --------------------------
TEST(stack_push, CALL_nn){
    const uint8_t op_call[3] = { 0xCD, 0xF0, 0xC0 };
    //CALL nn
    sms_ram[0] = op_call[0];
    sms_ram[1] = op_call[1];
    sms_ram[2] = op_call[2];
    z80dbg_set_breakpoint(0xC0F0, Z80_BREAK_PC); /*<-- Set breakpoint*/
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    const uint16_t return_address = RAM_BASE_ADDRESS + sizeof(op_call);
    TEST_ASSERT_PC_EQUAL(0xC0F0);
    TEST_ASSERT_SP_EQUAL(0xFFF0 - 2);
    TEST_ASSERT_STACKTOP_EQUAL16(return_address);
}

// -------------------------
// --- Conditional calls ---
// -------------------------
static void cc_test(const uint8_t* opcode, uint8_t flag){
    //Conditional CALL
    //Write opcodes to memory.
    //First jump, must be taken.
    sms_ram[0] = opcode[0]; sms_ram[1] = opcode[1];
    sms_ram[2] = opcode[2];
    //Second jump, must not be taken.
    sms_ram[0xF0] = opcode[0]; sms_ram[0xF1] = opcode[1];
    sms_ram[0xF2] = opcode[2] + 1;
    //Write set breakpoints, one at (BASE + 15) and other to (BASE + 15 + 3)
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 0xF0 + 0, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 0xF0 + 3, Z80_BREAK_PC);
    //Set flag (to take the jump)
    Z80_F = flag;
    //Execute opcode til breakpoint or tick limit
    __RUN_TEST_OPCODES;
    //Test results
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_PC_EQUAL(0xC0F0);
    TEST_ASSERT_SP_EQUAL(0xFFF0 - 2); /*<-- Stack increased its size by 2 */
    const uint16_t return_address = RAM_BASE_ADDRESS + 3;
    TEST_ASSERT_STACKTOP_EQUAL16(return_address); /*<-- Address pushed to stack */
    //Clear breakpoint
    bp_triggered = 0;
    //Swap flags
    Z80_F = Z80_F ^ 0xFF;
    //Execute second opcode
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_PC_EQUAL(0xC0F0 + 3);
    TEST_ASSERT_SP_EQUAL(0xFFF0 - 2); /*<-- SP is unchanged */
}

TEST(stack_push, CALL_NZ_nn){
    const uint8_t op[3] = { 0xC4, 0xF0, 0xC0 };
    cc_test(op, Z80_FLAG_ZERO ^ 0xFF);
}

TEST(stack_push, CALL_Z_nn){
    const uint8_t op[3] = { 0xCC, 0xF0, 0xC0 };
    cc_test(op, Z80_FLAG_ZERO);
}

TEST(stack_push, CALL_NC_nn){
    const uint8_t op[3] = { 0xD4, 0xF0, 0xC0 };
    cc_test(op, Z80_FLAG_CARRY ^ 0xFF);
}

TEST(stack_push, CALL_C_nn){
    const uint8_t op[3] = { 0xDC, 0xF0, 0xC0 };
    cc_test(op, Z80_FLAG_CARRY);
}

TEST(stack_push, CALL_PO_nn){
    const uint8_t op[3] = { 0xE4, 0xF0, 0xC0 };
    cc_test(op, Z80_FLAG_PARITY ^ 0xFF);
}

TEST(stack_push, CALL_PE_nn){
    const uint8_t op[3] = { 0xEC, 0xF0, 0xC0 };
    cc_test(op, Z80_FLAG_PARITY);
}

TEST(stack_push, CALL_P_nn){
    const uint8_t op[3] = { 0xF4, 0xF0, 0xC0 };
    cc_test(op, Z80_FLAG_SIGN ^ 0xFF);
}

TEST(stack_push, CALL_M_nn){
    const uint8_t op[3] = { 0xFC, 0xF0, 0xC0 };
    cc_test(op, Z80_FLAG_SIGN);
}

static void rst_test(uint8_t rst_index){
    //Opcode
    uint8_t op_rst = 0xC7;
    //Add reset vector index
    op_rst = op_rst | (rst_index << 3);
    //Check limits
    TEST_ASSERT_TRUE(rst_index < 8);
    //Calculate reset vector
    uint16_t reset_vector = rst_index * 0x08;

    sms_ram[0] = op_rst;
    z80dbg_set_breakpoint(reset_vector, Z80_BREAK_PC);
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_STACKTOP_EQUAL16(RAM_BASE_ADDRESS + 1);
    TEST_ASSERT_PC_EQUAL(reset_vector);
}

TEST(stack_push, RST0){
    rst_test(0);
}

TEST(stack_push, RST1){
    rst_test(1);
}

TEST(stack_push, RST2){
    rst_test(2);
}

TEST(stack_push, RST3){
    rst_test(3);
}

TEST(stack_push, RST4){
    rst_test(4);
}

TEST(stack_push, RST5){
    rst_test(5);
}

TEST(stack_push, RST6){
    rst_test(6);
}

TEST(stack_push, RST7){
    rst_test(7);
}

TEST_GROUP_RUNNER(stack_push){
    RUN_TEST_CASE(stack_push, PUSH_BC);
    RUN_TEST_CASE(stack_push, PUSH_DE);
    RUN_TEST_CASE(stack_push, PUSH_HL);
    RUN_TEST_CASE(stack_push, PUSH_AF);
    RUN_TEST_CASE(stack_push, PUSH_IX);
    RUN_TEST_CASE(stack_push, PUSH_IY);
    RUN_TEST_CASE(stack_push, CALL_nn);
    RUN_TEST_CASE(stack_push, CALL_NZ_nn);
    RUN_TEST_CASE(stack_push, CALL_Z_nn);
    RUN_TEST_CASE(stack_push, CALL_NC_nn);
    RUN_TEST_CASE(stack_push, CALL_C_nn);
    RUN_TEST_CASE(stack_push, CALL_PO_nn);
    RUN_TEST_CASE(stack_push, CALL_PE_nn);
    RUN_TEST_CASE(stack_push, CALL_P_nn);
    RUN_TEST_CASE(stack_push, CALL_M_nn);
    RUN_TEST_CASE(stack_push, RST0);
    RUN_TEST_CASE(stack_push, RST1);
    RUN_TEST_CASE(stack_push, RST2);
    RUN_TEST_CASE(stack_push, RST3);
    RUN_TEST_CASE(stack_push, RST4);
    RUN_TEST_CASE(stack_push, RST5);
    RUN_TEST_CASE(stack_push, RST6);
    RUN_TEST_CASE(stack_push, RST7);
}

/* ----------------------------------- */
/* --- POP Group (POP and returns) --- */
/* ----------------------------------- */
TEST_GROUP(stack_pop);

TEST_SETUP(stack_pop){
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

TEST_TEAR_DOWN(stack_pop){
    for (int i = 0; i < (256 * 256); ++i){
        z80dbg_set_breakpoint(i, 0);
    }
}

TEST(stack_pop, POP_BC){
    const uint8_t op_pop_bc = 0xC1;
    //POP BC
    sms_ram[0] = op_pop_bc;
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 1, Z80_BREAK_PC); /*<-- Set breakpoint*/
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_SP_EQUAL(0xFFF0 + 2);
    TEST_ASSERT_BC_EQUAL(0xBBAA);
}

TEST(stack_pop, POP_DE){
    const uint8_t op_pop_de = 0xD1;
    //POP DE
    sms_ram[0] = op_pop_de;
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 1, Z80_BREAK_PC); /*<-- Set breakpoint*/
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_SP_EQUAL(0xFFF0 + 2);
    TEST_ASSERT_DE_EQUAL(0xBBAA);
}

TEST(stack_pop, POP_HL){
    const uint8_t op_pop_hl = 0xE1;
    //POP HL
    sms_ram[0] = op_pop_hl;
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 0x0001, Z80_BREAK_PC); /*<-- Set breakpoint*/
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_SP_EQUAL(0xFFF0 + 2);
    TEST_ASSERT_HL_EQUAL(0xBBAA);
}

TEST(stack_pop, POP_AF){
    const uint8_t op_pop_af = 0xF1;
    //POP AF
    sms_ram[0] = op_pop_af;
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 0x0001, Z80_BREAK_PC); /*<-- Set breakpoint*/
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_SP_EQUAL(0xFFF0 + 2);
    TEST_ASSERT_AF_EQUAL(0xBBAA);
}

TEST(stack_pop, POP_IX){
    const uint8_t op_pop_ix[2] = { 0xDD, 0xE1 };
    //POP IX
    sms_ram[0] = op_pop_ix[0];
    sms_ram[1] = op_pop_ix[1];
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 0x0002, Z80_BREAK_PC); /*<-- Set breakpoint*/
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_SP_EQUAL(0xFFF0 + 2);
    TEST_ASSERT_IX_EQUAL(0xBBAA);
}

TEST(stack_pop, POP_IY){
    const uint8_t op_pop_iy[2] = { 0xFD, 0xE1 };
    //POP IX
    sms_ram[0] = op_pop_iy[0];
    sms_ram[1] = op_pop_iy[1];
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 0x0002, Z80_BREAK_PC); /*<-- Set breakpoint*/
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_SP_EQUAL(0xFFF0 + 2);
    TEST_ASSERT_IY_EQUAL(0xBBAA);
}

TEST(stack_pop, RET){
    const uint8_t op_ret = 0xC9;
    //RET
    sms_ram[0] = op_ret;
    z80dbg_set_breakpoint(0xBBAA, Z80_BREAK_PC); /*<-- Set breakpoint*/
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_SP_EQUAL(0xFFF0 + 2);
    TEST_ASSERT_PC_EQUAL(0xBBAA);
}

TEST(stack_pop, RETI){
    const uint8_t op_reti[2] = { 0xED, 0x4D };
    //RETI
    sms_ram[0] = op_reti[0];
    sms_ram[1] = op_reti[1];
    z80dbg_set_breakpoint(0xBBAA, Z80_BREAK_PC); /*<-- Set breakpoint*/
    __RUN_TEST_OPCODES;
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_SP_EQUAL(0xFFF0 + 2);
    TEST_ASSERT_PC_EQUAL(0xBBAA);
}

static void retn_test(uint8_t condition){
    uint8_t op_ret_cc = 0xC0;
    TEST_ASSERT_TRUE(condition < 8);
    op_ret_cc = op_ret_cc | (condition << 3);

    //Copy two opcodes, back-to-back
    sms_ram[0] = op_ret_cc;
    sms_ram[1] = op_ret_cc;

    //First RET must not be taken...
    //If expected value is Zero, swap all bits
    Z80_F = z80_cc_stat[condition] ? z80_cc[condition] ^ 0xFF : z80_cc[condition];
    //...breakpoint just after first opcode
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 1, Z80_BREAK_PC);
    //Run opcode
    __RUN_TEST_OPCODES;
    //Test jump has not been taken
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_SP_EQUAL(0xFFF0);
    TEST_ASSERT_PC_EQUAL(RAM_BASE_ADDRESS + 1);

    //Second RET must be taken
    //Swap flags
    Z80_F = Z80_F ^ 0xFF;
    //...breakpoint at stack-stored address
    z80dbg_set_breakpoint(0xBBAA, Z80_BREAK_PC);
    bp_triggered = 0; //<-- Clear trigger flag
    //Run opcodes
    __RUN_TEST_OPCODES;
    //Test jump has been taken
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_SP_EQUAL(0xFFF0 + 2);
    TEST_ASSERT_PC_EQUAL(0xBBAA);
}

TEST(stack_pop, RET_NZ){
    retn_test(0);
}

TEST(stack_pop, RET_Z){
    retn_test(1);
}

TEST(stack_pop, RET_NC){
    retn_test(2);
}

TEST(stack_pop, RET_C){
    retn_test(3);
}

TEST(stack_pop, RET_PO){
    retn_test(4);
}

TEST(stack_pop, RET_PE){
    retn_test(5);
}

TEST(stack_pop, RET_P){
    retn_test(6);
}

TEST(stack_pop, RET_M){
    retn_test(7);
}

IGNORE_TEST(stack_pop, RETN){
    TEST_FAIL_MESSAGE("Unimplemented opcode/test.");
}

TEST_GROUP_RUNNER(stack_pop){
    RUN_TEST_CASE(stack_pop, POP_BC);
    RUN_TEST_CASE(stack_pop, POP_DE);
    RUN_TEST_CASE(stack_pop, POP_HL);
    RUN_TEST_CASE(stack_pop, POP_AF);
    RUN_TEST_CASE(stack_pop, POP_IX);
    RUN_TEST_CASE(stack_pop, POP_IY);
    RUN_TEST_CASE(stack_pop, RET);
    RUN_TEST_CASE(stack_pop, RETN);
    RUN_TEST_CASE(stack_pop, RETI);
    RUN_TEST_CASE(stack_pop, RET_NZ);
    RUN_TEST_CASE(stack_pop, RET_Z);
    RUN_TEST_CASE(stack_pop, RET_NC);
    RUN_TEST_CASE(stack_pop, RET_C);
    RUN_TEST_CASE(stack_pop, RET_PO);
    RUN_TEST_CASE(stack_pop, RET_PE);
    RUN_TEST_CASE(stack_pop, RET_P);
    RUN_TEST_CASE(stack_pop, RET_M);
}



// ----------------------
// --- Helpers & Main ---
// ----------------------
//Helper 'run all' function
static void RunAllTests(void){
    RUN_TEST_GROUP(stack_push);
    RUN_TEST_GROUP(stack_pop);
}

//Main
int main(int argc, const char** argv){
    int rv = UnityMain(argc, argv, RunAllTests);
    return rv;
}
