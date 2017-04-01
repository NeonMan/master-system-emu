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

#include <assert.h>
#include <stdio.h>

#include "z80.h"
#include "z80_interrupt.h"
#include "z80_macros.h"
#include "z80_externs.h"

//Access z80 internals
extern struct z80_s  z80;

// ------------------------
// --- Interrupt mode 1 ---
// ------------------------

static int mode1_m1() {
    //M1 Stage. 7 ticks
    //Decrement SP
    assert(0); //<-- Unimplemented
    return Z80_STAGE_RESET;
}

static int mode1_m2() {
    //M2 Stage. 3 ticks
    //Write PCh to SP
    //Decrement SP
    assert(0); //<-- Unimplemented
    return Z80_STAGE_RESET;
}

static int mode1_m3() {
    //M3 Stage. 3 ticks
    //Write PCl to SP
    //Set PC to 0x0038
    assert(0); //<-- Unimplemented
    return Z80_STAGE_RESET;
}


// ----------------------------------
// --- Select interrupt mode path ---
// ----------------------------------

int z80_stage_int_m1() {
#ifndef NDEBUG
    /**/
    if(z80.m1_tick_count == 0){
        if (Z80_IFF1 && Z80_IFF2) {
            fprintf(stderr, "INT triggered!  Mode %d.\r\n", z80.int_mode);
            fflush(stderr);
        }
        else if (Z80_IFF2 && (!Z80_IFF2)) {
            fprintf(stderr, "NMI triggered!\r\n");
            fflush(stderr);
        }
    }
    /**/
#endif
    switch (z80.int_mode) {
    case 0:
        assert(0); //<-- Unimplemented
        return Z80_STAGE_RESET;
    case 1:
        return mode1_m1();
    case 2:
        assert(0); //<-- Unimplemented
        return Z80_STAGE_RESET;
    default:
        assert(z80.int_mode < 3);  //<-- Bad interrupt mode
    }
    return Z80_STAGE_RESET;
}

int z80_stage_int_m2(uint8_t noexec) {
    switch (z80.int_mode) {
    case 0:
        assert(0); //<-- Unimplemented
        return Z80_STAGE_RESET;
    case 1:
        return mode1_m2();
    case 2:
        assert(0); //<-- Unimplemented
        return Z80_STAGE_RESET;
    default:
        assert(z80.int_mode < 3);  //<-- Bad interrupt mode
    }
    return Z80_STAGE_RESET;
}

int z80_stage_int_m3(uint8_t noexec) {
    switch (z80.int_mode) {
    case 0:
        assert(0); //<-- Unimplemented
        return Z80_STAGE_RESET;
    case 1:
        return mode1_m3();
    case 2:
        assert(0); //<-- Unimplemented
        return Z80_STAGE_RESET;
    default:
        assert(z80.int_mode < 3);  //<-- Bad interrupt mode
    }
    return Z80_STAGE_RESET;
}
