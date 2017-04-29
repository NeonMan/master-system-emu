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

#include <rom/rom.h>
#include <savestate/savestate.h>

// --- Constants --
#define ROM_FILE_NAME "SAMPLE.ROM"
#define SAVE_FILE_NAME "SAMPLE.SAV"

// --- Variables --
static uint8_t rom_pattern[ROM_MAX_SIZE];

TEST_GROUP(grp_savestate_rom);

// ---------------------------
// -- Full ROM save-restore --
// ---------------------------
TEST_SETUP(grp_savestate_rom){
    //Initialize
    for (int i = 0; i < ROM_MAX_SIZE; ++i){
        rom_pattern[i] = (uint8_t)((i + 3) & 0xFF);
    }
    memcpy(romdbg_get_rom(), rom_pattern, ROM_MAX_SIZE);

    //Save
    FILE* sav_file = fopen(SAVE_FILE_NAME, "wb");
    TEST_ASSERT_NOT_NULL(sav_file);
    ss_save(sav_file, 0);
    fclose(sav_file);

    //Clear module
    memset(romdbg_get_rom(), 0, ROM_MAX_SIZE);

    //Restore
    sav_file = fopen(SAVE_FILE_NAME, "rb");
    TEST_ASSERT_NOT_NULL(sav_file);
    ss_restore(sav_file);
    fclose(sav_file);
}

TEST_TEAR_DOWN(grp_savestate_rom){
    remove(SAVE_FILE_NAME);
}

TEST(grp_savestate_rom, rom){
    TEST_ASSERT_EQUAL_INT8_ARRAY(rom_pattern, romdbg_get_rom(), ROM_MAX_SIZE);
}

TEST_GROUP_RUNNER(grp_savestate_rom){
    RUN_TEST_CASE(grp_savestate_rom, rom);
}

// ------------------------
// --- File ROM restore ---
// ------------------------

TEST_GROUP(grp_savestate_file);

TEST_SETUP(grp_savestate_file){
    //Initialize
    for (int i = 0; i < ROM_MAX_SIZE; ++i){
        rom_pattern[i] = (uint8_t)((i + 3) & 0xFF);
    }
    memcpy(romdbg_get_rom(), rom_pattern, ROM_MAX_SIZE);

    //Make a ROM file
    FILE* rom_file = fopen(ROM_FILE_NAME, "wb");
    TEST_ASSERT_NOT_NULL(rom_file);
    size_t written_blocks = fwrite(rom_pattern, ROM_MAX_SIZE, 1, rom_file);
    TEST_ASSERT_EQUAL(1, written_blocks);
    fclose(rom_file);


    //Save
    FILE* sav_file = fopen(SAVE_FILE_NAME, "wb");
    TEST_ASSERT_NOT_NULL(sav_file);
    ss_save(sav_file, ROM_FILE_NAME);
    fclose(sav_file);

    //Clear module
    memset(romdbg_get_rom(), 0, ROM_MAX_SIZE);

    //Restore
    sav_file = fopen(SAVE_FILE_NAME, "rb");
    TEST_ASSERT_NOT_NULL(sav_file);
    ss_restore(sav_file);
    fclose(sav_file);
}

TEST_TEAR_DOWN(grp_savestate_file){
    remove(ROM_FILE_NAME);
    remove(SAVE_FILE_NAME);
}

IGNORE_TEST(grp_savestate_file, rom){
    //Test wether the filename has been recovered.
    //Can't do yet.
}

TEST_GROUP_RUNNER(grp_savestate_file){
    RUN_TEST_CASE(grp_savestate_file, rom);
}

// ----------------------
// --- Helpers & Main ---
// ----------------------
//Helper 'run all' function
static void RunAllTests(void) {
    RUN_TEST_GROUP(grp_savestate_file);
    RUN_TEST_GROUP(grp_savestate_rom);
}

//Main
int main(int argc, const char** argv) {
    int rv = UnityMain(argc, argv, RunAllTests);
    return rv;
}