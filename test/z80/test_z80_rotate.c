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
 RR
 RRA
 RRC
 RRCA
 RL
 RLA
 RLC
 RLCA
*/

#include "test_z80.h"
#include "z80/z80_opcodes.h"

#include "dataset-rl.h"
#include "dataset-rla.h"
#include "dataset-rr.h"
#include "dataset-rra.h"

#include "dataset-rlc.h"
#include "dataset-rlca.h"
#include "dataset-rrc.h"
#include "dataset-rrca.h"

static char test_str[100];
static make_msg(uint8_t a) {
    sprintf(test_str, "[ In A:%02X ]", a);
}

TEST_GROUP(rotate);

TEST_SETUP(rotate){
	
}

TEST_TEAR_DOWN(rotate){
	
}


TEST(rotate, rl) {
    alu_result_t r;

    //Test for initial F=0x00
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_rl[a + (256 * 0)];
        uint8_t  expected_a = (expected_af >> 8) & 0xFF;
        uint8_t  expected_f = expected_af & 0xFF;

        r = op_rotate_shift(Z80_SHIFT_RL, a, 0x00);

        make_msg(a);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
    }

    //Test for initial F=0x00 + Carry
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_rl[a + (256 * 1)];
        uint8_t  expected_a = (expected_af >> 8) & 0xFF;
        uint8_t  expected_f = expected_af & 0xFF;

        r = op_rotate_shift(Z80_SHIFT_RL, a, 0x00 | Z80_FLAG_CARRY);

        make_msg(a);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
    }

    //Test for initial F=0xFF
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_rl[a + (256 * 2)];
        uint8_t  expected_a = (expected_af >> 8) & 0xFF;
        uint8_t  expected_f = expected_af & 0xFF;

        r = op_rotate_shift(Z80_SHIFT_RL, a, 0xFF);

        make_msg(a);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
    }

    //Test for initial F=0xFF - Carry
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_rl[a + (256 * 3)];
        uint8_t  expected_a = (expected_af >> 8) & 0xFF;
        uint8_t  expected_f = expected_af & 0xFF;

        r = op_rotate_shift(Z80_SHIFT_RL, a, 0xFF & Z80_CLRFLAG_CARRY);

        make_msg(a);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
    }
}

TEST(rotate, rr) {
    alu_result_t r;

    //Test for initial F=0x00
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_rr[a + (256 * 0)];
        uint8_t  expected_a = (expected_af >> 8) & 0xFF;
        uint8_t  expected_f = expected_af & 0xFF;

        r = op_rotate_shift(Z80_SHIFT_RR, a, 0x00);

        make_msg(a);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
    }

    //Test for initial F=0x00 + Carry
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_rr[a + (256 * 1)];
        uint8_t  expected_a = (expected_af >> 8) & 0xFF;
        uint8_t  expected_f = expected_af & 0xFF;

        r = op_rotate_shift(Z80_SHIFT_RR, a, 0x00 | Z80_FLAG_CARRY);

        make_msg(a);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
    }

    //Test for initial F=0xFF
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_rr[a + (256 * 2)];
        uint8_t  expected_a = (expected_af >> 8) & 0xFF;
        uint8_t  expected_f = expected_af & 0xFF;

        r = op_rotate_shift(Z80_SHIFT_RR, a, 0xFF);

        make_msg(a);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
    }

    //Test for initial F=0xFF - Carry
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_rr[a + (256 * 3)];
        uint8_t  expected_a = (expected_af >> 8) & 0xFF;
        uint8_t  expected_f = expected_af & 0xFF;

        r = op_rotate_shift(Z80_SHIFT_RR, a, 0xFF & Z80_CLRFLAG_CARRY);

        make_msg(a);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
    }
}

TEST(rotate, rla) {
    //Test for initial F=0x00
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_rla[a + (256 * 0)];
        uint8_t  expected_a = (expected_af >> 8) & 0xFF;
        uint8_t  expected_f = expected_af & 0xFF;

        z80.opcode_index = 1;
        Z80_A = a;
        Z80_F = 0x00;
        RLA();

        make_msg(a);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, Z80_A, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, Z80_F, test_str);
    }

    //Test for initial F=0x00 + Carry
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_rla[a + (256 * 1)];
        uint8_t  expected_a = (expected_af >> 8) & 0xFF;
        uint8_t  expected_f = expected_af & 0xFF;

        z80.opcode_index = 1;
        Z80_A = a;
        Z80_F = 0x00 | Z80_FLAG_CARRY;
        RLA();

        make_msg(a);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, Z80_A, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, Z80_F, test_str);
    }

    //Test for initial F=0xFF
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_rla[a + (256 * 2)];
        uint8_t  expected_a = (expected_af >> 8) & 0xFF;
        uint8_t  expected_f = expected_af & 0xFF;

        z80.opcode_index = 1;
        Z80_A = a;
        Z80_F = 0xFF;
        RLA();

        make_msg(a);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, Z80_A, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, Z80_F, test_str);
    }

    //Test for initial F=0xFF - Carry
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_rla[a + (256 * 3)];
        uint8_t  expected_a = (expected_af >> 8) & 0xFF;
        uint8_t  expected_f = expected_af & 0xFF;

        z80.opcode_index = 1;
        Z80_A = a;
        Z80_F = 0xFF & Z80_CLRFLAG_CARRY;
        RLA();

        make_msg(a);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, Z80_A, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, Z80_F, test_str);
    }
}

TEST(rotate, rra) {
    //Test for initial F=0x00
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_rra[a + (256 * 0)];
        uint8_t  expected_a = (expected_af >> 8) & 0xFF;
        uint8_t  expected_f = expected_af & 0xFF;

        z80.opcode_index = 1;
        Z80_A = a;
        Z80_F = 0x00;
        RRA();

        make_msg(a);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, Z80_A, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, Z80_F, test_str);
    }

    //Test for initial F=0x00 + Carry
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_rra[a + (256 * 1)];
        uint8_t  expected_a = (expected_af >> 8) & 0xFF;
        uint8_t  expected_f = expected_af & 0xFF;

        z80.opcode_index = 1;
        Z80_A = a;
        Z80_F = 0x00 | Z80_FLAG_CARRY;
        RRA();

        make_msg(a);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, Z80_A, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, Z80_F, test_str);
    }

    //Test for initial F=0xFF
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_rra[a + (256 * 2)];
        uint8_t  expected_a = (expected_af >> 8) & 0xFF;
        uint8_t  expected_f = expected_af & 0xFF;

        z80.opcode_index = 1;
        Z80_A = a;
        Z80_F = 0xFF;
        RRA();

        make_msg(a);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, Z80_A, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, Z80_F, test_str);
    }

    //Test for initial F=0xFF - Carry
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_rra[a + (256 * 3)];
        uint8_t  expected_a = (expected_af >> 8) & 0xFF;
        uint8_t  expected_f = expected_af & 0xFF;

        z80.opcode_index = 1;
        Z80_A = a;
        Z80_F = 0xFF & Z80_CLRFLAG_CARRY;
        RRA();

        make_msg(a);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, Z80_A, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, Z80_F, test_str);
    }
}

TEST(rotate, rrc) {
    alu_result_t r;

    //Test for initial F=0x00
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_rrc[a + (256 * 0)];
        uint8_t  expected_a = (expected_af >> 8) & 0xFF;
        uint8_t  expected_f = expected_af & 0xFF;

        r = op_rotate_shift(Z80_SHIFT_RRC, a, 0x00);

        make_msg(a);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
    }

    //Test for initial F=0xFF
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_rrc[a + (256 * 1)];
        uint8_t  expected_a = (expected_af >> 8) & 0xFF;
        uint8_t  expected_f = expected_af & 0xFF;

        r = op_rotate_shift(Z80_SHIFT_RRC, a, 0xFF);

        make_msg(a);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
    }
}

TEST(rotate, rlc) {
    alu_result_t r;

    //Test for initial F=0x00
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_rlc[a + (256 * 0)];
        uint8_t  expected_a = (expected_af >> 8) & 0xFF;
        uint8_t  expected_f = expected_af & 0xFF;

        r = op_rotate_shift(Z80_SHIFT_RLC, a, 0x00);

        make_msg(a);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
    }

    //Test for initial F=0xFF
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_rlc[a + (256 * 1)];
        uint8_t  expected_a = (expected_af >> 8) & 0xFF;
        uint8_t  expected_f = expected_af & 0xFF;

        r = op_rotate_shift(Z80_SHIFT_RLC, a, 0xFF);

        make_msg(a);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, r.result, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, r.flags, test_str);
    }
}

TEST(rotate, rrca) {
    //Test for initial F=0x00
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_rrca[a + (256 * 0)];
        uint8_t  expected_a = (expected_af >> 8) & 0xFF;
        uint8_t  expected_f = expected_af & 0xFF;

        z80.opcode_index = 1;
        Z80_A = a;
        Z80_F = 0x00;
        RRCA();

        make_msg(a);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, Z80_A, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, Z80_F, test_str);
    }

    //Test for initial F=0xFF
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_rrca[a + (256 * 1)];
        uint8_t  expected_a = (expected_af >> 8) & 0xFF;
        uint8_t  expected_f = expected_af & 0xFF;

        z80.opcode_index = 1;
        Z80_A = a;
        Z80_F = 0x00;
        RRCA();

        make_msg(a);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, Z80_A, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, Z80_F, test_str);
    }
}

TEST(rotate, rlca) {
    //Test for initial F=0x00
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_rlca[a + (256 * 0)];
        uint8_t  expected_a = (expected_af >> 8) & 0xFF;
        uint8_t  expected_f = expected_af & 0xFF;

        z80.opcode_index = 1;
        Z80_A = a;
        Z80_F = 0x00;
        RLCA();

        make_msg(a);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, Z80_A, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, Z80_F, test_str);
    }

    //Test for initial F=0xFF
    for (int a = 0; a < 256; a++) {
        uint16_t expected_af = z80_dataset_rlca[a + (256 * 1)];
        uint8_t  expected_a = (expected_af >> 8) & 0xFF;
        uint8_t  expected_f = expected_af & 0xFF;

        z80.opcode_index = 1;
        Z80_A = a;
        Z80_F = 0x00;
        RLCA();

        make_msg(a);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_a, Z80_A, test_str);
        TEST_ASSERT_EQUAL_HEX8_MESSAGE(expected_f, Z80_F, test_str);
    }
}

TEST_GROUP_RUNNER(rotate){
    RUN_TEST_CASE(rotate, rl);
    RUN_TEST_CASE(rotate, rla);
    RUN_TEST_CASE(rotate, rr);
    RUN_TEST_CASE(rotate, rra);

    RUN_TEST_CASE(rotate, rlc);
    RUN_TEST_CASE(rotate, rlca);
    RUN_TEST_CASE(rotate, rrc);
    RUN_TEST_CASE(rotate, rrca);
}


// ----------------------
// --- Helpers & Main ---
// ----------------------
//Helper 'run all' function
static void RunAllTests(void){
    RUN_TEST_GROUP(rotate);
}

//Main
int main(int argc, const char** argv){
    int rv = UnityMain(argc, argv, RunAllTests);
    return rv;
}
