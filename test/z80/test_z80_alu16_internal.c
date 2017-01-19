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

TEST_GROUP(alu16_internal);

TEST_SETUP(alu16_internal) {
}

TEST_TEAR_DOWN(alu16_internal) {
}

TEST(alu16_internal, add) {
    
}

TEST_GROUP_RUNNER(alu16_internal) {
    RUN_TEST_CASE(alu16_internal, add);
}

// ----------------------
// --- Helpers & Main ---
// ----------------------
//Helper 'run all' function
static void RunAllTests(void) {
    RUN_TEST_GROUP(alu16_internal);
}

//Main
int main(int argc, const char** argv) {
    int rv = UnityMain(argc, argv, RunAllTests);
    return rv;
}


