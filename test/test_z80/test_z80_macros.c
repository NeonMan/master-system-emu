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

#include <string.h>

#include "unity.h"
#include "unity_fixture.h"

#include "z80/z80.h"
#include "z80/z80_macros.h"

TEST_GROUP(z80_macros);

/* --- Mocks --- */
struct z80_s z80;

/* --- Test cases --- */
TEST_SETUP(z80_macros){
    memset(&z80, 0, sizeof(struct z80_s));
}

TEST_TEAR_DOWN(z80_macros){

}

/*Test sub-byte divisons*/
TEST(z80_macros, subbyte){
    const unsigned char x = (3 << 6); /*0b11000000*/
    const unsigned char y = (7 << 3); /*0b00111000*/
    const unsigned char z = (7 << 0); /*0b00000111*/
    const unsigned char p = (3 << 4); /*0b00110000*/
    const unsigned char q = (1 << 3); /*0b00001000*/

    TEST_ASSERT_EQUAL(x, Z80_OPCODE_X(3));
    TEST_ASSERT_EQUAL(y, Z80_OPCODE_Y(7));
    TEST_ASSERT_EQUAL(z, Z80_OPCODE_Z(7));

    TEST_ASSERT_EQUAL(x + z, Z80_OPCODE_XZ(3, 7));
    TEST_ASSERT_EQUAL(y + z, Z80_OPCODE_YZ(7, 7));

    TEST_ASSERT_EQUAL(x + y + z, Z80_OPCODE_XYZ(3, 7, 7));
    TEST_ASSERT_EQUAL(x + p + q + z, Z80_OPCODE_XPQZ(3, 3, 1, 7));

    TEST_ASSERT_EQUAL(x, Z80_OPCODE_X_MASK);
    TEST_ASSERT_EQUAL(y, Z80_OPCODE_Y_MASK);
    TEST_ASSERT_EQUAL(z, Z80_OPCODE_Z_MASK);
    TEST_ASSERT_EQUAL(p, Z80_OPCODE_P_MASK);
    TEST_ASSERT_EQUAL(q, Z80_OPCODE_Q_MASK);
}

TEST(z80_macros, register_access){
    Z80_I = 0x01;
    Z80_R = 0x02;
    TEST_ASSERT_EQUAL_HEX8(0x01, Z80_I);
    TEST_ASSERT_EQUAL_HEX8(0x02, Z80_R);
    Z80_A = 0x03;
    Z80_F = 0x04;
    Z80_Ap = 0x05;
    Z80_Fp = 0x06;
    TEST_ASSERT_EQUAL_HEX16(0x0304, Z80_AF);
    TEST_ASSERT_EQUAL_HEX16(0x0506, Z80_AFp);
    Z80_B = 0x07;
    Z80_C = 0x08;
    Z80_Bp = 0x09;
    Z80_Cp = 0x0A;
    TEST_ASSERT_EQUAL_HEX16(0x0708, Z80_BC);
    TEST_ASSERT_EQUAL_HEX16(0x090A, Z80_BCp);
    Z80_D = 0x0B;
    Z80_E = 0x0C;
    Z80_Dp = 0x0D;
    Z80_Ep = 0x0E;
    TEST_ASSERT_EQUAL_HEX16(0x0B0C, Z80_DE);
    TEST_ASSERT_EQUAL_HEX16(0x0D0E, Z80_DEp);
    Z80_H = 0x0F;
    Z80_L = 0x10;
    Z80_Hp = 0x11;
    Z80_Lp = 0x12;
    TEST_ASSERT_EQUAL_HEX16(0x0F10, Z80_HL);
    TEST_ASSERT_EQUAL_HEX16(0x1112, Z80_HLp);
    Z80_W = 0x13;
    Z80_Z = 0x14;
    Z80_Wp = 0x15;
    Z80_Zp = 0x16;
    TEST_ASSERT_EQUAL_HEX16(0x1314, Z80_WZ);
    TEST_ASSERT_EQUAL_HEX16(0x1516, Z80_WZp);
    Z80_IXH = 0x17;
    Z80_IXL = 0x18;
    Z80_IYH = 0x19;
    Z80_IYL = 0x1A;
    TEST_ASSERT_EQUAL_HEX16(0x1718, Z80_IX);
    TEST_ASSERT_EQUAL_HEX16(0x191A, Z80_IY);
}

TEST(z80_macros, pins){
    z80_address = 0xAA55;
    TEST_ASSERT_EQUAL_HEX8(0xAA, Z80_ADDRH);
    TEST_ASSERT_EQUAL_HEX8(0x55, Z80_ADDRL);
}

TEST(z80_macros, flags){
    /*Carry*/
    /*Add/Subtract*/
    /*Parity/Overflow*/
    /*Half-carry*/

    /*Zero*/
    TEST_ASSERT_EQUAL(0, Z80_SETFLAG_ZERO(0xFF));
    TEST_ASSERT_EQUAL(Z80_FLAG_ZERO, Z80_SETFLAG_ZERO(0x00));
    TEST_ASSERT_EQUAL(0, Z80_SETFLAG_ZERO(0x01));
    /*Sign*/
    TEST_ASSERT_EQUAL(Z80_FLAG_SIGN, Z80_SETFLAG_SIGN((uint8_t)((int8_t)-128)));
    TEST_ASSERT_EQUAL(Z80_FLAG_SIGN, Z80_SETFLAG_SIGN((uint8_t)((int8_t)-1)));
    TEST_ASSERT_EQUAL(0, Z80_SETFLAG_SIGN((uint8_t)((int8_t)127)));
    TEST_ASSERT_EQUAL(0, Z80_SETFLAG_SIGN((uint8_t)((int8_t)0)));
    /*Add/Subtract*/
    TEST_ASSERT_EQUAL(0, Z80_SETFLAG_SUBTRACT(0));
    TEST_ASSERT_EQUAL(Z80_FLAG_SUBTRACT, Z80_SETFLAG_SUBTRACT(1));
    /*Carry/Borrow*/
    TEST_ASSERT_EQUAL(Z80_FLAG_CARRY, Z80_SETFLAG_CARRY(0xFF, 0x00));
    TEST_ASSERT_EQUAL(0, Z80_SETFLAG_CARRY(0xF0, 0xFF));

    TEST_FAIL_MESSAGE("Test is not complete.");
}

IGNORE_TEST(z80_macros, register_lut){

}

TEST_GROUP_RUNNER(z80_macros){
    RUN_TEST_CASE(z80_macros, subbyte);
    RUN_TEST_CASE(z80_macros, register_access);
    RUN_TEST_CASE(z80_macros, pins);
    RUN_TEST_CASE(z80_macros, flags);
    RUN_TEST_CASE(z80_macros, register_lut);
}

// ----------------------
// --- Helpers & Main ---
// ----------------------
//Helper 'run all' function
static void RunAllTests(void){
    RUN_TEST_GROUP(z80_macros);
}

//Main
int main(int argc, const char** argv){
    int rv = UnityMain(argc, argv, RunAllTests);
    return rv;
}
