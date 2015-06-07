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

#include "unity.h"
#include "unity_fixture.h"

#include "z80/z80.h"
#include "z80/z80_externs.h"
#include "z80/z80_macros.h"
#include "ram/ram.h"

#include "io/io_externs.h"
#include "io/fake_io.h"

TEST_GROUP(stack);

/* --- Globals --- */
///Spy on the z80 internals
extern struct z80_s  z80;
///SMS RAM pointer
uint8_t* sms_ram;

/* --- Test cases --- */
TEST_SETUP(stack){
    sms_ram = ramdbg_get_mem();
    io_stat = IO_RAM ^ 0xFF;
    memset(&z80, 0, sizeof(struct z80_s));
    z80.rSP = 0xFFFF;
    z80.rPC = RAM_BASE_ADDRESS;
}

TEST_TEAR_DOWN(stack){

}

TEST(stack, dummy){
    TEST_FAIL_MESSAGE("Unimplemented.");
}

TEST_GROUP_RUNNER(stack){
    RUN_TEST_CASE(stack, dummy);
}

// ----------------------
// --- Helpers & Main ---
// ----------------------
//Helper 'run all' function
static void RunAllTests(void){
    RUN_TEST_GROUP(stack);
}

//Main
int main(int argc, const char** argv){
    z80_tick();
    int rv = UnityMain(argc, argv, RunAllTests);
    return rv;
}
