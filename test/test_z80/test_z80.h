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

#ifndef __TEST_Z80_H
#define __TEST_Z80_H

#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "unity.h"
#include "unity_fixture.h"
#include "unity_sms_emu.h"
#include "test_z80.h"

#include "z80/z80.h"
#include "z80/z80_externs.h"
#include "z80/z80_macros.h"
#include "z80/z80_register_lut.h"
#include "ram/ram.h"

#include "io/io_externs.h"

/* --- Globals --- */
///Spy on the z80 internals
extern struct z80_s  z80;
///SMS RAM pointer
uint8_t* sms_ram;
///Breakpoint triggered
uint8_t bp_triggered;
///Tick limit
int16_t tick_limit;

/* --- Helpers --- */

/*Clocks the peripherals for a given time*/
#define __RUN_TEST_OPCODES \
        while (tick_limit && (!bp_triggered)){\
        sys_tick();\
        --tick_limit;\
    }

/*Ticks the modules under test (Z80 and RAM)*/
static void sys_tick(){
    z80_tick();
    if ((!z80_n_mreq) && ((!z80_n_rd) || (!z80_n_wr))){ //Memory-mapped operation
        ram_tick();
    }
}

/*Callback for th PC breakpoints*/
static void pc_breakpoint_cb(uint16_t pc){
    bp_triggered = 1;
}

/*Poke a SMS RAM byte*/
static void _set_ram(uint16_t address, uint8_t b){
    if (address < RAM_BASE_ADDRESS){
        TEST_FAIL_MESSAGE("RAM address out of range.");
    }
    else{
        ((uint8_t*)(ramdbg_get_mem()))[(address - RAM_BASE_ADDRESS) % RAM_SIZE] = b;
    }
}


#endif