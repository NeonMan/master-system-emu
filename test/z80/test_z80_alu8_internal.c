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
#include "z80/z80_opcodes.h"

TEST_GROUP(alu8_internal);

TEST_SETUP(alu8_internal) {
}

TEST_TEAR_DOWN(alu8_internal) {
}

TEST(alu8_internal, add) {
    alu_result_t r;
    //Adding zeros
    r = alu8_op(Z80_ALUOP_ADD, 0, 0, 0);
    TEST_ASSERT_FALSE(r.flags & Z80_FLAG_SIGN);      //<-- Positive
    TEST_ASSERT_FALSE(r.flags & Z80_FLAG_CARRY);    //<-- No carry
    TEST_ASSERT_FALSE(r.flags & Z80_FLAG_HC);       //<-- No half-carry
    TEST_ASSERT_FALSE(r.flags & Z80_FLAG_PARITY);   //<-- No overflow
    TEST_ASSERT_TRUE(r.flags & Z80_FLAG_ZERO);      //<-- Zero
    TEST_ASSERT_FALSE(r.flags & Z80_FLAG_SUBTRACT); //<-- No carry
}

TEST_GROUP_RUNNER(alu8_internal) {
    RUN_TEST_CASE(alu8_internal, add);
}

// ----------------------
// --- Helpers & Main ---
// ----------------------
//Helper 'run all' function
static void RunAllTests(void) {
    RUN_TEST_GROUP(alu8_internal);
}

//Main
int main(int argc, const char** argv) {
    int rv = UnityMain(argc, argv, RunAllTests);
    return rv;
}


