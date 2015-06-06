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

TEST_GROUP(decoder);

TEST_SETUP(decoder){

}

TEST_TEAR_DOWN(decoder){

}

TEST(decoder, dummy){
    TEST_FAIL_MESSAGE("Tests not implemented.");
}

TEST_GROUP_RUNNER(decoder){
    RUN_TEST_CASE(decoder, dummy);
}

// ----------------------
// --- Helpers & Main ---
// ----------------------
//Helper 'run all' function
static void RunAllTests(void){
    RUN_TEST_GROUP(decoder);
}

//Main
int main(int argc, const char** argv){
    int rv = UnityMain(argc, argv, RunAllTests);
    return rv;
}
