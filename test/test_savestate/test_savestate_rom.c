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
#include "unity/unity.h"
#include "unity/unity_fixture.h"
#include <stdio.h>

TEST_GROUP(grp_savestate_rom);

TEST_SETUP(grp_savestate_rom){
}

TEST_TEAR_DOWN(grp_savestate_rom){
}

TEST_GROUP_RUNNER(grp_savestate_rom){

}

// ----------------------
// --- Helpers & Main ---
// ----------------------
//Helper 'run all' function
static void RunAllTests(void) {
    RUN_TEST_GROUP(grp_savestate_rom);
}

//Main
int main(int argc, const char** argv) {
    int rv = UnityMain(argc, argv, RunAllTests);
    return rv;
}