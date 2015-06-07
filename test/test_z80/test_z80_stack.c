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
 CALL
 RETN
 RST
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
    Z80_IX = 0x10;
    Z80_IY = 0x11;
    
    //Setup callbacks
    z80dbg_set_pc_breakpoint_cb(pc_breakpoint_cb);
}

TEST_TEAR_DOWN(stack_push){

}

TEST(stack_push, PUSH_BC){
    const uint8_t op_push_bc = 0xC5;
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

TEST_GROUP_RUNNER(stack_push){
    RUN_TEST_CASE(stack_push, PUSH_BC);
    RUN_TEST_CASE(stack_push, PUSH_DE);
    RUN_TEST_CASE(stack_push, PUSH_HL);
    RUN_TEST_CASE(stack_push, PUSH_AF);
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
