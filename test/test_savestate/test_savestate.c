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

#include <z80/z80.h>
#include <z80/z80_macros.h>
#include <psg/psg.h>
#include <ram/ram.h>
#include <rom/rom.h>
#include <savestate/savestate.h>

// --- Constants ---
#define SAVE_FILE_NAME "SAMPLE.SAV"

#define VALUE_ADDRESS 0x0102
#define VALUE_DATA    0x03

#define VALUE_RD      0x04
#define VALUE_WR      0x05
#define VALUE_IOREQ   0x06
#define VALUE_MREQ    0x07
#define VALUE_RFSH    0x08
#define VALUE_M1      0x09

#define VALUE_INT     0x0A
#define VALUE_NMI     0x0B
#define VALUE_RESET   0x0C
#define VALUE_WAIT    0x0D

#define VALUE_BUSREQ  0x0E
#define VALUE_BUSACK  0x0F


// --- Variables ---
static struct z80_s z80_pattern;
static struct z80_s z80_zero;

static uint8_t ram_pattern[RAM_SIZE];

TEST_GROUP(grp_savestate);

// --- Tests ---
TEST_SETUP(grp_savestate) {
    //Initialize with pattern
    for (int i = 0; i < (sizeof(struct z80_s)); ++i) {
        ((uint8_t*)&z80_pattern)[i] = (uint8_t)i;
    }
    for (int i = 0; i < RAM_SIZE; ++i) {
        ram_pattern[i] = (uint8_t)i;
    }
    //Initialize with zeros
    memset(&z80_zero, 0, sizeof(struct z80_s));

    //Initialize modules with pattern
    *z80dbg_get_z80() = z80_pattern;
    memcpy(ramdbg_get_mem(), ram_pattern, RAM_SIZE);
    z80_address = VALUE_ADDRESS;
    z80_data = VALUE_DATA;
    z80_n_rd = VALUE_RD;
    z80_n_wr = VALUE_WR;
    z80_n_ioreq = VALUE_IOREQ;
    z80_n_mreq = VALUE_MREQ;
    z80_n_rfsh = VALUE_RFSH;
    z80_n_m1 = VALUE_M1;
    z80_n_int = VALUE_INT;
    z80_n_nmi = VALUE_NMI;
    z80_n_reset = VALUE_RESET;
    z80_n_wait = VALUE_WAIT;
    z80_n_busreq = VALUE_BUSREQ;
    z80_n_busack = VALUE_BUSACK;

    //Dump file
    FILE* f;
    f = fopen(SAVE_FILE_NAME, "wb");
    TEST_ASSERT_NOT_NULL(f);
    TEST_ASSERT_EQUAL(0, ss_save(f, SAVE_FILE_NAME));
    fclose(f);

    //Clear modules
    *z80dbg_get_z80() = z80_zero;
    memset(ramdbg_get_mem(), 0, RAM_SIZE);
    z80_address = 0;
    z80_data = 0;
    z80_n_rd = 0;
    z80_n_wr = 0;
    z80_n_ioreq = 0;
    z80_n_mreq = 0;
    z80_n_rfsh = 0;
    z80_n_m1 = 0;
    z80_n_int = 0;
    z80_n_nmi = 0;
    z80_n_reset = 0;
    z80_n_wait = 0;
    z80_n_busreq = 0;
    z80_n_busack = 0;

    //Restore
    f = fopen(SAVE_FILE_NAME, "rb");
    TEST_ASSERT_NOT_NULL(f);
    TEST_ASSERT_EQUAL(0, ss_restore(f));
    fclose(f);
}

TEST_TEAR_DOWN(grp_savestate) {
    ///@ToDo delete sample rom and savestate
}

TEST(grp_savestate, ram) {
    TEST_ASSERT_EQUAL_HEX8_ARRAY(ram_pattern, ramdbg_get_mem(), RAM_SIZE);
}

TEST(grp_savestate, z80_pins) {
    TEST_ASSERT_EQUAL(VALUE_ADDRESS, z80_address);
    TEST_ASSERT_EQUAL(VALUE_DATA, z80_data);
    TEST_ASSERT_EQUAL(VALUE_RD, z80_n_rd);
    TEST_ASSERT_EQUAL(VALUE_WR, z80_n_wr);
    TEST_ASSERT_EQUAL(VALUE_IOREQ, z80_n_ioreq);
    TEST_ASSERT_EQUAL(VALUE_MREQ, z80_n_mreq);
    TEST_ASSERT_EQUAL(VALUE_RFSH, z80_n_rfsh);
    TEST_ASSERT_EQUAL(VALUE_M1, z80_n_m1);
    TEST_ASSERT_EQUAL(VALUE_INT, z80_n_int);
    TEST_ASSERT_EQUAL(VALUE_NMI, z80_n_nmi);
    TEST_ASSERT_EQUAL(VALUE_RESET, z80_n_reset);
    TEST_ASSERT_EQUAL(VALUE_WAIT, z80_n_wait);
    TEST_ASSERT_EQUAL(VALUE_BUSREQ, z80_n_busreq);
    TEST_ASSERT_EQUAL(VALUE_BUSACK, z80_n_busack);
}

TEST(grp_savestate, z80) {
    const struct z80_s z80_ = *z80dbg_get_z80();
    TEST_ASSERT_EQUAL(z80_pattern.data_latch, z80_.data_latch);
    TEST_ASSERT_EQUAL(z80_pattern.m1_tick_count, z80_.m1_tick_count);
    TEST_ASSERT_EQUAL(z80_pattern.m2_tick_count, z80_.m2_tick_count);
    TEST_ASSERT_EQUAL(z80_pattern.m3_tick_count, z80_.m3_tick_count);
    TEST_ASSERT_EQUAL(z80_pattern.opcode_index, z80_.opcode_index);
    TEST_ASSERT_EQUAL(z80_pattern.read_address, z80_.read_address);
    TEST_ASSERT_EQUAL(z80_pattern.read_index, z80_.read_index);
    TEST_ASSERT_EQUAL(z80_pattern.read_is_io, z80_.read_is_io);
    TEST_ASSERT_EQUAL(z80_pattern.stage, z80_.stage);
    TEST_ASSERT_EQUAL(z80_pattern.write_address, z80_.write_address);
    TEST_ASSERT_EQUAL(z80_pattern.write_index, z80_.write_index);
    TEST_ASSERT_EQUAL(z80_pattern.write_is_io, z80_.write_is_io);
    TEST_ASSERT_EQUAL(z80_pattern.rI, z80_.rI);
    TEST_ASSERT_EQUAL(z80_pattern.rR, z80_.rR);
    TEST_ASSERT_EQUAL(z80_pattern.rIX, z80_.rIX);
    TEST_ASSERT_EQUAL(z80_pattern.rIY, z80_.rIY);
    TEST_ASSERT_EQUAL(z80_pattern.rPC, z80_.rPC);
    TEST_ASSERT_EQUAL(z80_pattern.rSP, z80_.rSP);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(z80_pattern.iff, z80_.iff, 2);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(z80_pattern.opcode, z80_.opcode, 4);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(z80_pattern.rAF, z80_.rAF, 4);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(z80_pattern.rBC, z80_.rBC, 4);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(z80_pattern.rDE, z80_.rDE, 4);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(z80_pattern.rHL, z80_.rHL, 4);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(z80_pattern.read_buffer, z80_.read_buffer, 2);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(z80_pattern.write_buffer, z80_.write_buffer, 2);
}

TEST_GROUP_RUNNER(grp_savestate) {
    RUN_TEST_CASE(grp_savestate, ram);
    RUN_TEST_CASE(grp_savestate, z80);
    RUN_TEST_CASE(grp_savestate, z80_pins);
}

// ----------------------
// --- Helpers & Main ---
// ----------------------
//Helper 'run all' function
static void RunAllTests(void) {
    RUN_TEST_GROUP(grp_savestate);
}

//Main
int main(int argc, const char** argv) {
    int rv = UnityMain(argc, argv, RunAllTests);
    return rv;
}
