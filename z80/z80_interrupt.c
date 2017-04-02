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
    ///@bug Mode1 INT M1 pin accuracy ignored, probe the real Z80 and read what happens.
    switch (z80.m1_tick_count) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        z80.m1_tick_count++;
        return Z80_STAGE_M1_INT;
    case 7:
        Z80_SP--;
        return Z80_STAGE_M2_INT;
    }
    assert(0); //<-- Unimplemented
    return Z80_STAGE_RESET;
}

static int mode1_m2() {
    //M2 Stage. 3 ticks
    //Write PCh to SP
    //Decrement SP
    ///@bug Mode1 INT M2 pin accuracy ignored, probe the real Z80 and read what happens.
    switch (z80.m2_tick_count) {
    case 0:
        z80_address = Z80_SP;
        z80_data = (uint8_t)(Z80_PC >> 8);
        z80_n_mreq = 0;
        z80.m2_tick_count++;
        return Z80_STAGE_M2_INT;
    case 1:
        z80_n_wr = 0;
        z80.m2_tick_count++;
        return Z80_STAGE_M2_INT;
    case 2:
        z80_n_wr = 1;
        z80_n_mreq = 1;
        z80.m2_tick_count++;
        return Z80_STAGE_M2_INT;
    case 3:
        Z80_SP--;
        z80.m2_tick_count++;
        return Z80_STAGE_M3_INT;
    }
    assert(0); //<-- Unimplemented
    return Z80_STAGE_RESET;
}

static int mode1_m3() {
    //M3 Stage. 3 ticks
    //Write PCl to SP
    //Set PC to 0x0038
    ///@bug Mode1 INT M3 pin accuracy ignored, probe the real Z80 and read what happens.
    switch (z80.m3_tick_count) {
    case 0:
        z80_address = Z80_SP;
        z80_data = (uint8_t)(Z80_PC);
        z80_n_mreq = 0;
        z80.m3_tick_count++;
        return Z80_STAGE_M3_INT;
    case 1:
        z80_n_wr = 0;
        z80.m3_tick_count++;
        return Z80_STAGE_M3_INT;
    case 2:
        z80_n_wr = 1;
        z80_n_mreq = 1;
        z80.m3_tick_count++;
        return Z80_STAGE_M3_INT;
    case 3:
        Z80_PC = 0x0038;
        return Z80_STAGE_RESET;
    }
    assert(0); //<-- Unimplemented
    return Z80_STAGE_RESET;
}


// ------------------------------------
// --- INT interrupts. Select mode. ---
// ------------------------------------

int z80_stage_int_m1() {
#ifndef NDEBUG
    /**/
    if(z80.m1_tick_count == 0){
        fprintf(stderr, "INT triggered!  Mode %d.\r\n", z80.int_mode);
        fflush(stderr);
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

int z80_stage_int_m2() {
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

int z80_stage_int_m3() {
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

// ---------------------
// --- NMI interrupt ---
// ---------------------

int z80_stage_nmi_m1() {
    //5T-sates
    //Decrement SP
    ///@bug NMI M1 pin accuracy ignored, probe the real Z80 and read what happens.

#ifndef NDEBUG
    /**/
    if (z80.m1_tick_count == 0) {
        fprintf(stderr, "NMI triggered!\r\n");
        fflush(stderr);
    }
    /**/
#endif
    switch (z80.m1_tick_count) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        z80.m1_tick_count++;
        return Z80_STAGE_M1_NMI;
    case 9:
        Z80_SP--;
        return Z80_STAGE_M2_NMI;
    }
    assert(0); //<-- Unimplemented
    return Z80_STAGE_RESET;
}

int z80_stage_nmi_m2() {
    //3T-States
    //Write PCh to (SP), decrement SP
    ///@bug NMI M2 pin accuracy ignored, probe the real Z80 and read what happens.
    switch (z80.m2_tick_count) {
    case 0:
        z80_address = Z80_SP;
        z80_data = (uint8_t)(Z80_PC >> 8);
        z80.m2_tick_count++;
        return Z80_STAGE_M2_NMI;
    case 1:
        z80_n_mreq = 0;
        z80.m2_tick_count++;
        return Z80_STAGE_M2_NMI;
    case 2:
        z80_n_wr = 0;
        z80.m2_tick_count++;
        return Z80_STAGE_M2_NMI;
    case 3:
        z80_n_wr = 1;
        z80_n_mreq = 1;
        z80.m2_tick_count++;
        return Z80_STAGE_M2_NMI;
    case 4:
        Z80_SP--;
        z80.m2_tick_count++;
        return Z80_STAGE_M2_NMI;
    case 5:
        return Z80_STAGE_M3_NMI;
    }
    assert(0); //<-- Unimplemented
    return Z80_STAGE_RESET;
}

int z80_stage_nmi_m3() {
    //3T-States
    //Write PCl to (SP), PC <= 0x0066
    ///@bug NMI M3 pin accuracy ignored, probe the real Z80 and read what happens.
    switch (z80.m2_tick_count) {
    case 0:
        z80_address = Z80_SP;
        z80_data = (uint8_t)(Z80_PC);
        z80.m2_tick_count++;
        return Z80_STAGE_M2_NMI;
    case 1:
        z80_n_mreq = 0;
        z80.m2_tick_count++;
        return Z80_STAGE_M2_NMI;
    case 2:
        z80_n_wr = 0;
        z80.m2_tick_count++;
        return Z80_STAGE_M2_NMI;
    case 3:
        z80_n_wr = 1;
        z80_n_mreq = 1;
        z80.m2_tick_count++;
        return Z80_STAGE_M2_NMI;
    case 4:
        Z80_PC = 0x0066;
        z80.m2_tick_count++;
        return Z80_STAGE_M2_NMI;
    case 5:
        return Z80_STAGE_M3_NMI;
    }
    assert(0); //<-- Unimplemented
    return Z80_STAGE_RESET;
}
