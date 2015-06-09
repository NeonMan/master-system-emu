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
 POP        (ToDo)
 CALL nn
 CALL cc nn (ToDo)
 RETN       (ToDo)
 RST        (ToDo)
*/

#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "unity.h"
#include "unity_fixture.h"

#include "z80/z80.h"
#include "z80/z80_externs.h"
#include "z80/z80_macros.h"
#include "ram/ram.h"

#include "io/io_externs.h"

TEST_GROUP(stack_push);

/* --- Globals --- */
///Spy on the z80 internals
extern struct z80_s  z80;
///SMS RAM pointer
uint8_t* sms_ram;
///Breakpoint triggered
uint8_t bp_triggered;
///Tick limit
int16_t tick_limit;

/* --- Helpers --- */
static void sys_tick(){
    z80_tick();
    if ((!z80_n_mreq) && ((!z80_n_rd) || (!z80_n_wr))){ //Memory-mapped operation
        ram_tick();
    }
}

static void pc_breakpoint_cb(uint16_t pc){
    bp_triggered = 1;
}

static uint8_t get_ram(uint16_t address){
    if (address < RAM_BASE_ADDRESS){
        TEST_FAIL_MESSAGE("RAM address out of range.");
        return 0;
    }
    else{
        return sms_ram[(address - RAM_BASE_ADDRESS) % RAM_SIZE];
    }
}

static void set_ram(uint16_t address, uint8_t b){
    if (address < RAM_BASE_ADDRESS){
        TEST_FAIL_MESSAGE("RAM address out of range.");
    }
    else{
        sms_ram[(address - RAM_BASE_ADDRESS) % RAM_SIZE] = b;
    }
}

/* --- Test cases --- */
TEST_SETUP(stack_push){
    //Setup state
    sms_ram = ramdbg_get_mem();
    io_stat = IO_RAM ^ 0xFF;
    memset(&z80, 0, sizeof(struct z80_s));
    z80.rSP = 0xFFF0;
    z80.rPC = RAM_BASE_ADDRESS;
    bp_triggered = 0;
    tick_limit = 0x0FFF;
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

}

TEST(stack_push, PUSH_BC){
    const uint8_t op_push_bc = 0xC5;
    const uint8_t op_pop_bc = 0xC1;
    //PUSH BC
    sms_ram[0] = op_push_bc; /*<-- Copy opcode to ram*/
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + sizeof(op_push_bc), Z80_BREAK_PC); /*<-- Set breakpoint*/
    while (tick_limit && (!bp_triggered)){
        sys_tick();
        --tick_limit;
    }
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_EQUAL_HEX16(0xFFF0 - 2, Z80_SP);
    TEST_ASSERT_EQUAL_HEX8(0x04, get_ram(Z80_SP + 0));
    TEST_ASSERT_EQUAL_HEX8(0x03, sms_ram[(Z80_SP + 1 - RAM_BASE_ADDRESS) % RAM_SIZE]);
}

TEST(stack_push, PUSH_DE){
    const uint8_t op_push_de = 0xD5;
    const uint8_t op_pop_de = 0xD1;
    //PUSH DE
    sms_ram[0] = op_push_de; /*<-- Copy opcode to ram*/
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + sizeof(op_push_de), Z80_BREAK_PC); /*<-- Set breakpoint*/
    while (tick_limit && (!bp_triggered)){
        sys_tick();
        --tick_limit;
    }
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_EQUAL_HEX16(0xFFF0 - 2, Z80_SP);
    TEST_ASSERT_EQUAL_HEX8(0x06, get_ram(Z80_SP + 0));
    TEST_ASSERT_EQUAL_HEX8(0x05, sms_ram[(Z80_SP + 1 - RAM_BASE_ADDRESS) % RAM_SIZE]);
}

TEST(stack_push, PUSH_HL){
    const uint8_t op_push_hl = 0xE5;
    const uint8_t op_pop_hl = 0xE1;
    //PUSH HL
    sms_ram[0] = op_push_hl; /*<-- Copy opcode to ram*/
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + sizeof(op_push_hl), Z80_BREAK_PC); /*<-- Set breakpoint*/
    while (tick_limit && (!bp_triggered)){
        sys_tick();
        --tick_limit;
    }
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_EQUAL_HEX16(0xFFF0 - 2, Z80_SP);
    TEST_ASSERT_EQUAL_HEX8(0x08, get_ram(Z80_SP + 0));
    TEST_ASSERT_EQUAL_HEX8(0x07, sms_ram[(Z80_SP + 1 - RAM_BASE_ADDRESS) % RAM_SIZE]);
}

TEST(stack_push, PUSH_AF){
    const uint8_t op_push_af = 0xF5;
    const uint8_t op_pop_af = 0xF1;
    //PUSH AF
    sms_ram[0] = op_push_af; /*<-- Copy opcode to ram*/
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + sizeof(op_push_af), Z80_BREAK_PC); /*<-- Set breakpoint*/
    while (tick_limit && (!bp_triggered)){
        sys_tick();
        --tick_limit;
    }
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_EQUAL_HEX16(0xFFF0 - 2, Z80_SP);
    TEST_ASSERT_EQUAL_HEX8(0x02, get_ram(Z80_SP + 0));
    TEST_ASSERT_EQUAL_HEX8(0x01, sms_ram[(Z80_SP + 1 - RAM_BASE_ADDRESS) % RAM_SIZE]);
}

TEST(stack_push, PUSH_IX){
    const uint8_t op_push_ix[2] = { 0xDD, 0xE5 };
    //PUSH IX
    sms_ram[0] = op_push_ix[0]; /**/
    sms_ram[1] = op_push_ix[1]; /*<-- Copy opcode to ram*/
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + sizeof(op_push_ix), Z80_BREAK_PC); /*<-- Set breakpoint*/
    while (tick_limit && (!bp_triggered)){
        sys_tick();
        --tick_limit;
    }
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_EQUAL_HEX16(0xFFF0 - 2, Z80_SP);
    TEST_ASSERT_EQUAL_HEX8(0x11, get_ram(Z80_SP + 0));
    TEST_ASSERT_EQUAL_HEX8(0x10, sms_ram[(Z80_SP + 1 - RAM_BASE_ADDRESS) % RAM_SIZE]);
}

TEST(stack_push, PUSH_IY){
    const uint8_t op_push_iy[2] = { 0xFD, 0xE5 };
    //PUSH IY
    sms_ram[0] = op_push_iy[0]; /**/
    sms_ram[1] = op_push_iy[1]; /*<-- Copy opcode to ram*/
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + sizeof(op_push_iy), Z80_BREAK_PC); /*<-- Set breakpoint*/
    while (tick_limit && (!bp_triggered)){
        sys_tick();
        --tick_limit;
    }
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_EQUAL_HEX16(0xFFF0 - 2, Z80_SP);
    TEST_ASSERT_EQUAL_HEX8(0x13, get_ram(Z80_SP + 0));
    TEST_ASSERT_EQUAL_HEX8(0x12, get_ram(Z80_SP + 1));
}

TEST(stack_push, CALL_nn){
    const uint8_t op_call[3] = { 0xCD, 0xF0, 0xC0 };
    //CALL nn
    sms_ram[0] = op_call[0];
    sms_ram[1] = op_call[1];
    sms_ram[2] = op_call[2];
    z80dbg_set_breakpoint(0xC0F0, Z80_BREAK_PC); /*<-- Set breakpoint*/
    while (tick_limit && (!bp_triggered)){
        sys_tick();
        --tick_limit;
    }
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_EQUAL_HEX16(0xC0F0, Z80_PC);
    TEST_ASSERT_EQUAL_HEX16(0xFFF0 - 2, Z80_SP);
    const uint16_t return_address = RAM_BASE_ADDRESS + sizeof(op_call);
    TEST_ASSERT_EQUAL(return_address & 0x00FF, get_ram(Z80_SP));
    TEST_ASSERT_EQUAL((return_address >> 8) & 0x00FF, get_ram(Z80_SP + 1));
}

TEST(stack_push, CALL_NZ_nn){
    //CALL NZ, nn
    const uint8_t op_call_nz_nn[3] = { 0xC4, 0xF0, 0xC0 };
    //Write opcodes to memory.
    //First jump, must be taken.
    sms_ram[0] = op_call_nz_nn[0]; sms_ram[1] = op_call_nz_nn[1];
    sms_ram[2] = op_call_nz_nn[2];
    //First jump, must not be taken.
    sms_ram[0xF0] = op_call_nz_nn[0]; sms_ram[0xF1] = op_call_nz_nn[1];
    sms_ram[0xF2] = op_call_nz_nn[2];
    //Write set breakpoints, one at (BASE + 15) and other to (BASE + 15 + 3)
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 0xF0 + 0, Z80_BREAK_PC);
    z80dbg_set_breakpoint(RAM_BASE_ADDRESS + 0xF0 + 3, Z80_BREAK_PC);
    //Set flag (to take the jump)
    Z80_F = Z80_FLAG_ZERO ^ 0xFF;
    //Execute opcode til breakpoint or tick limit
    while (tick_limit && (!bp_triggered)){
        sys_tick();
        --tick_limit;
    }
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_EQUAL_HEX16(0xC0F0, Z80_PC);
    TEST_ASSERT_EQUAL_HEX16(0xFFF0 - 2, Z80_SP); /*<-- Address pushed to stack */
    const uint16_t return_address = RAM_BASE_ADDRESS + sizeof(op_call_nz_nn);
    TEST_ASSERT_EQUAL_HEX8((return_address >> 8) & 0xFF, get_ram(Z80_SP + 1));
    TEST_ASSERT_EQUAL_HEX8(return_address        & 0xFF, get_ram(Z80_SP + 0));
    //Clear breakpoint
    bp_triggered = 0;
    //Swap flags
    Z80_F = Z80_F ^ 0xFF;
    //Execute second opcode
    while (tick_limit && (!bp_triggered)){
        sys_tick();
        --tick_limit;
    }
    TEST_ASSERT_TRUE(tick_limit > 0);
    TEST_ASSERT_TRUE(bp_triggered);
    TEST_ASSERT_EQUAL_HEX16(0xC0F0 + 3, Z80_PC);
    TEST_ASSERT_EQUAL_HEX16(0xFFF0 - 2, Z80_SP); /*<-- SP is unchanged */
    /*Neither is the stack top*/
    TEST_ASSERT_EQUAL_HEX8((return_address >> 8) & 0xFF, get_ram(Z80_SP + 1));
    TEST_ASSERT_EQUAL_HEX8(return_address & 0xFF, get_ram(Z80_SP + 0));
}

IGNORE_TEST(stack_push, CALL_Z_nn){
    const uint8_t op_call_z_nn[3] = { 0xCC, 0xF0, 0xC0 };
    TEST_FAIL_MESSAGE("Unimplemented");
}

IGNORE_TEST(stack_push, CALL_NC_nn){
    const uint8_t op_call_nc_nn[3] = { 0xD4, 0xF0, 0xC0 };
    TEST_FAIL_MESSAGE("unimplemented");
}

IGNORE_TEST(stack_push, CALL_C_nn){
    const uint8_t op_call_c_nn[3] = { 0xDC, 0xF0, 0xC0 };
    TEST_FAIL_MESSAGE("unimplemented");
}

IGNORE_TEST(stack_push, CALL_PO_nn){
    const uint8_t op_call_po_nn[3] = { 0xE4, 0xF0, 0xC0 };
    TEST_FAIL_MESSAGE("unimplemented");
}

IGNORE_TEST(stack_push, CALL_PE_nn){
    const uint8_t op_call_pe_nn[3] = { 0xEC, 0xF0, 0xC0 };
    TEST_FAIL_MESSAGE("unimplemented");
}

IGNORE_TEST(stack_push, CALL_P_nn){
    const uint8_t op_call_p_nn[3] = { 0xF4, 0xF0, 0xC0 };
    TEST_FAIL_MESSAGE("unimplemented");
}

IGNORE_TEST(stack_push, CALL_M_nn){
    const uint8_t op_call_m_nn[3] = { 0xFC, 0xF0, 0xC0 };
    TEST_FAIL_MESSAGE("unimplemented");
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
}

// ----------------------
// --- Helpers & Main ---
// ----------------------
//Helper 'run all' function
static void RunAllTests(void){
    RUN_TEST_GROUP(stack_push);
}

//Main
int main(int argc, const char** argv){
    int rv = UnityMain(argc, argv, RunAllTests);
    return rv;
}
