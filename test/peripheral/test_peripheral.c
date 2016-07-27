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
#include <stdio.h>
#include <z80/z80_externs.h>
#include <io/fake_io.h>
#include <peripheral/peripheral.h>

#define VALUE_AB 0xAA
#define VALUE_BM 0x55

static char message[256];

TEST_GROUP(grp_peripheral_write);
TEST_GROUP(grp_peripheral_read);

TEST_SETUP(grp_peripheral_write){
    *perdbg_reg_ab() = 0;
    *perdbg_reg_bm() = 0;
    *perdbg_reg_control() = 0;
    io_stat = 0x00;
    z80_data = 0;
    z80_n_wr = 0;
    z80_n_rd = 1;
    z80_n_mreq = 1;
    z80_n_ioreq = 0;
    sprintf(message, "");
}

TEST_TEAR_DOWN(grp_peripheral_write){

}

TEST_SETUP(grp_peripheral_read){
    *perdbg_reg_ab() = 0;
    *perdbg_reg_bm() = 0;
    *perdbg_reg_control() = 0;
    io_stat = 0x00;
    z80_data = 0;
    z80_n_wr = 1;
    z80_n_rd = 0;
    z80_n_mreq = 1;
    z80_n_ioreq = 0;
    sprintf(message, "");

    *perdbg_reg_ab() = VALUE_AB;
    *perdbg_reg_bm() = VALUE_BM;
}

TEST_TEAR_DOWN(grp_peripheral_read){

}

// -------------------------------------------------------------------------
// --- BM port. Port 0xDD (Mirrors on all even address from 0xC1 to 0xFF  --
// -------------------------------------------------------------------------
TEST(grp_peripheral_read, get_bm_valid){
    for (int i = 0xC1; i <= 0xFF; i = i + 2){
        sprintf(message, "Testing port 0x%02X", (uint8_t)i);
        z80_address = (uint16_t)i;
        z80_data = 0;
        per_tick();
        TEST_ASSERT_EQUAL_MESSAGE(VALUE_BM, z80_data, message);
    }
}

TEST(grp_peripheral_read, get_bm_invalid){
    for (int i = 0xC0; i <= 0xFF; i = i + 2){
        sprintf(message, "Testing port 0x%02X", (uint8_t)i);
        z80_address = (uint16_t)i;
        per_tick();
        TEST_ASSERT_NOT_EQUAL_MESSAGE(VALUE_BM, z80_data, message);
    }
    for (int i = 0; i < 0xC0; ++i){
        sprintf(message, "Testing port 0x%02X", (uint8_t)i);
        z80_address = (uint16_t)i;
        per_tick();
        TEST_ASSERT_NOT_EQUAL_MESSAGE(VALUE_BM, z80_data, message);
    }
}

TEST(grp_peripheral_read, get_bm_disabled){
    io_stat = io_stat | IO_PERIPHERIAL;
    for (int i = 0; i <= 0xFF; ++i){
        sprintf(message, "Testing port 0x%02X", (uint8_t)i);
        z80_address = (uint16_t)i;
        z80_data = 0;
        per_tick();
        TEST_ASSERT_NOT_EQUAL_MESSAGE(VALUE_BM, z80_data, message);
    }
}

TEST(grp_peripheral_read, get_bm_mreq){
    z80_n_mreq = 0;
    z80_n_ioreq = 1;
    for (int i = 0; i <= 0xFF; ++i){
        sprintf(message, "Testing port 0x%02X", (uint8_t)i);
        z80_address = (uint16_t)i;
        z80_data = 0;
        per_tick();
        TEST_ASSERT_NOT_EQUAL_MESSAGE(VALUE_BM, z80_data, message);
    }
}

// -------------------------------------------------------------------------
// --- AB port. Port 0xDC (Mirrors on all even address from 0xC0 to 0xFD  --
// -------------------------------------------------------------------------
TEST(grp_peripheral_read, get_ab_valid){
    for (int i = 0xC0; i <= 0xFF; i = i + 2){
        sprintf(message, "Testing port 0x%02X", (uint8_t)i);
        z80_address = (uint16_t)i;
        z80_data = 0;
        per_tick();
        TEST_ASSERT_EQUAL_MESSAGE(VALUE_AB, z80_data, message);
    }
}

TEST(grp_peripheral_read, get_ab_invalid){
    for (int i = 0xC1; i <= 0xFF; i = i + 2){
        sprintf(message, "Testing port 0x%02X", (uint8_t)i);
        z80_address = (uint16_t)i;
        per_tick();
        TEST_ASSERT_NOT_EQUAL_MESSAGE(VALUE_AB, z80_data, message);
    }
    for (int i = 0; i < 0xC0; ++i){
        sprintf(message, "Testing port 0x%02X", (uint8_t)i);
        z80_address = (uint16_t)i;
        per_tick();
        TEST_ASSERT_NOT_EQUAL_MESSAGE(VALUE_AB, z80_data, message);
    }
}

TEST(grp_peripheral_read, get_ab_disabled){
    io_stat = io_stat | IO_PERIPHERIAL;
    for (int i = 0; i <= 0xFF; ++i){
        sprintf(message, "Testing port 0x%02X", (uint8_t)i);
        z80_address = (uint16_t)i;
        z80_data = 0;
        per_tick();
        TEST_ASSERT_NOT_EQUAL_MESSAGE(VALUE_AB, z80_data, message);
    }
}

TEST(grp_peripheral_read, get_ab_mreq){
    z80_n_mreq = 0;
    z80_n_ioreq = 1;
    for (int i = 0; i <= 0xFF; ++i){
        sprintf(message, "Testing port 0x%02X", (uint8_t)i);
        z80_address = (uint16_t)i;
        z80_data = 0;
        per_tick();
        TEST_ASSERT_NOT_EQUAL_MESSAGE(VALUE_AB, z80_data, message);
    }
}

// ---------------------------------------------------------------------------------
// --- Control port. Port: 0x3F (Mirrors on all odd addresses from 0x01 to 0x3F) ---
// ---------------------------------------------------------------------------------
TEST(grp_peripheral_write, set_control_valid){
    //Check that all valid writes
    for (int i = 1; i <= 0x3f; i = i + 2){
        sprintf(message, "Testing port 0x%02X", (uint8_t)i);
        z80_data = (uint8_t)((i % 255) + 1);
        z80_address = (uint16_t)i;
        per_tick();
        TEST_ASSERT_EQUAL_MESSAGE((i + 1) & 0xFF, *perdbg_reg_control(), message);
    }
}

TEST(grp_peripheral_write, set_control_invalid){
    //Check that all not valid writes
    for (int i = 0; i <= 0x3f; i = i + 2){
        sprintf(message, "Testing port 0x%02X", (uint8_t)i);
        z80_data = (uint8_t)((i % 255) + 1);
        z80_address = (uint16_t)i;
        per_tick();
        TEST_ASSERT_EQUAL_MESSAGE(0, *perdbg_reg_control(), message);
    }
    for (int i = 0x40; i <= 0xFF; ++i){
        sprintf(message, "Testing port 0x%02X", (uint8_t)i);
        z80_data = (uint8_t)((i % 255) + 1);
        z80_address = (uint16_t)i;
        per_tick();
        TEST_ASSERT_EQUAL_MESSAGE(0, *perdbg_reg_control(), message);
    }
}

TEST(grp_peripheral_write, set_control_disabled){
    io_stat = io_stat | (IO_PERIPHERIAL);
    for (int i = 0; i <= 0xFF; ++i){
        sprintf(message, "Testing port 0x%02X", (uint8_t)i);
        z80_data = (uint8_t)((i % 255) + 1);
        z80_address = (uint16_t)i;
        per_tick();
        TEST_ASSERT_EQUAL_MESSAGE(0, *perdbg_reg_control(), message);
    }
}

TEST(grp_peripheral_write, set_control_mreq){
    z80_n_mreq = 0;
    z80_n_ioreq = 1;
    for (int i = 0; i <= 0xFF; ++i){
        sprintf(message, "Testing port 0x%02X", (uint8_t)i);
        z80_data = (uint8_t)((i % 255) + 1);
        z80_address = (uint16_t)i;
        per_tick();
        TEST_ASSERT_EQUAL_MESSAGE(0, *perdbg_reg_control(), message);
    }
}


TEST_GROUP_RUNNER(grp_peripheral_write){
    RUN_TEST_CASE(grp_peripheral_write, set_control_valid);
    RUN_TEST_CASE(grp_peripheral_write, set_control_invalid);
    RUN_TEST_CASE(grp_peripheral_write, set_control_disabled);
    RUN_TEST_CASE(grp_peripheral_write, set_control_mreq);
}

TEST_GROUP_RUNNER(grp_peripheral_read){
    RUN_TEST_CASE(grp_peripheral_read, get_ab_valid);
    RUN_TEST_CASE(grp_peripheral_read, get_ab_invalid);
    RUN_TEST_CASE(grp_peripheral_read, get_ab_disabled);
    RUN_TEST_CASE(grp_peripheral_read, get_ab_mreq);

    RUN_TEST_CASE(grp_peripheral_read, get_bm_valid);
    RUN_TEST_CASE(grp_peripheral_read, get_bm_invalid);
    RUN_TEST_CASE(grp_peripheral_read, get_bm_disabled);
    RUN_TEST_CASE(grp_peripheral_read, get_bm_mreq);
}

// ----------------------
// --- Helpers & Main ---
// ----------------------
//Helper 'run all' function
static void RunAllTests(void){
    RUN_TEST_GROUP(grp_peripheral_write);
    RUN_TEST_GROUP(grp_peripheral_read);
}

//Main
int main(int argc, const char** argv){
    int rv = UnityMain(argc, argv, RunAllTests);
    return rv;
}
