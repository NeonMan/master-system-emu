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
 * This unit test checks the execution correctness of opcodes,
 * in essence it will check the end stage of decode/execution.
 *
 * Return value of the decoder call *must* be Z80_STAGE_RESET and
 * any side effects leading to this last call will be ignored or
 * mocked. This test will be unable to test the correctness of
 * neither main memory access nor any other M1/M2/M3 stage actions.
 */

#include "unity.h"
#include "unity_fixture.h"
#include "z80/z80.h"
#include "z80/z80_decoder.h"
#include "z80/z80_macros.h"
#include <string.h>

//Test macros
#define TEST_AF(E,F)  TEST_ASSERT_EQUAL(E.rAF[0], F.rAF[0]); TEST_ASSERT_EQUAL(E.rAF[1], F.rAF[1]);
#define TEST_AFp(E,F) TEST_ASSERT_EQUAL(E.rAF[2], F.rAF[2]); TEST_ASSERT_EQUAL(E.rAF[3], F.rAF[3]);
#define TEST_BC(E,F)  TEST_ASSERT_EQUAL(E.rBC[0], F.rBC[0]); TEST_ASSERT_EQUAL(E.rBC[1], F.rBC[1]);
#define TEST_BCp(E,F) TEST_ASSERT_EQUAL(E.rBC[2], F.rBC[2]); TEST_ASSERT_EQUAL(E.rBC[3], F.rBC[3]);
#define TEST_DE(E,F)  TEST_ASSERT_EQUAL(E.rDE[0], F.rDE[0]); TEST_ASSERT_EQUAL(E.rDE[1], F.rDE[1]);
#define TEST_DEp(E,F) TEST_ASSERT_EQUAL(E.rDE[2], F.rDE[2]); TEST_ASSERT_EQUAL(E.rDE[3], F.rDE[3]);
#define TEST_HL(E,F)  TEST_ASSERT_EQUAL(E.rHL[0], F.rHL[0]); TEST_ASSERT_EQUAL(E.rHL[1], F.rHL[1]);
#define TEST_HLp(E,F) TEST_ASSERT_EQUAL(E.rHL[2], F.rHL[2]); TEST_ASSERT_EQUAL(E.rHL[3], F.rHL[3]);

#define TEST_PC(E,F)  TEST_ASSERT_EQUAL(E.rPC,F.rPC);
#define TEST_SP(E,F)  TEST_ASSERT_EQUAL(E.rSP,F.rSP);
#define TEST_IX(E,F)  TEST_ASSERT_EQUAL(E.rIX,F.rIX);
#define TEST_IY(E,F)  TEST_ASSERT_EQUAL(E.rIY,F.rIY);

#define TEST_REGS(E,F) \
TEST_AF(E,F);\
TEST_BC(E,F);\
TEST_DE(E,F);\
TEST_HL(E,F);\
TEST_AFp(E,F);\
TEST_BCp(E,F);\
TEST_DEp(E,F);\
TEST_HLp(E,F);\
TEST_PC(E,F);\
TEST_SP(E,F);\
TEST_IX(E,F);\
TEST_IY(E,F);

#define TEST_I(E,F) TEST_ASSERT_EQUAL(E.rI,F.rI);
#define TEST_R(E,F) TEST_ASSERT_EQUAL(E.rR,F.rR);

//Declarations
extern struct z80_s z80; //<-- Access to z80 internals
struct z80_s z80_original;
struct z80_s z80_expected;

//Declare each group name
TEST_GROUP(unprefixed);
TEST_GROUP(cb_prefix);
TEST_GROUP(ed_prefix);
TEST_GROUP(dd_fd_prefix);

// --------------------------
// --- Unprefixed opcodes ---
// --------------------------
TEST_SETUP(unprefixed){
    memset(&z80, 0, sizeof(struct z80_s));
    memset(&z80_original, 0, sizeof(struct z80_s));
    memset(&z80_expected, 0, sizeof(struct z80_s));
}

TEST_TEAR_DOWN(unprefixed){
}

// --- Test cases ---

//NOP
TEST(unprefixed, opcode_NOP){
    z80.opcode[0]    = 0x00;
    z80.opcode_index = 1;
    z80_expected = z80;
    TEST_ASSERT_EQUAL(Z80_STAGE_RESET, z80_instruction_decode());
    TEST_REGS(z80_expected, z80);
}

//EX AF, AF'
TEST(unprefixed, opcode_EX_AF_AFp){
    z80.opcode[0]    = Z80_OPCODE_XYZ(0, 1, 0);
    z80.opcode_index = 1;
    Z80_A = 0xAA; Z80_Ap = 0xBB;
    Z80_F = 0x55; Z80_Fp = 0x66;
    TEST_ASSERT_EQUAL(Z80_STAGE_RESET, z80_instruction_decode());
    TEST_ASSERT_EQUAL(0xBB, Z80_A);
    TEST_ASSERT_EQUAL(0xAA, Z80_Ap);
    TEST_ASSERT_EQUAL(0x66, Z80_F);
    TEST_ASSERT_EQUAL(0x55, Z80_Fp);
    TEST_ASSERT_EQUAL(0xBB66, Z80_AF);
    TEST_ASSERT_EQUAL(0xAA55, Z80_AFp);
}

//DJNZ
IGNORE_TEST(unprefixed, opcode_DJNZ_d){
    //DJNZ 16
    z80.opcode[0] = Z80_OPCODE_XYZ(0, 2, 0);
    z80.opcode[1] = 16;
    z80.opcode_index = 2;

}

// --- Test runner ---
TEST_GROUP_RUNNER(unprefixed){
    RUN_TEST_CASE(unprefixed, opcode_NOP);
    RUN_TEST_CASE(unprefixed, opcode_EX_AF_AFp);
    RUN_TEST_CASE(unprefixed, opcode_DJNZ_d);
}

// ----------------------
// --- Helpers & Main ---
// ----------------------
//Helper 'run all' function
static void RunAllTests(void){
    RUN_TEST_GROUP(unprefixed);
}

//Main
int main(int argc, const char** argv){
    int rv = UnityMain(argc, argv, RunAllTests);
    return rv;
}
