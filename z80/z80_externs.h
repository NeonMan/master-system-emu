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

/** @file z80_externs.h
 *  @brief The Z80 buses and signals. 
 * 
 *  Used by modules communicating with the Z80. (Everyone!)
 */
#ifndef __Z80_EXTERNS_H
#define __Z80_EXTERNS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Z80 Buses
extern uint8_t  z80_data;     ///<-- Data bus, 8 bit wide (Input/Output)
extern uint16_t z80_address;  ///<-- Address bus   (Output)

// Z80 output signals
extern uint8_t  z80_n_rd;    ///<-- !Read enable  (Output)
extern uint8_t  z80_n_wr;    ///<-- !Write enable (Output)
extern uint8_t  z80_n_ioreq; ///<-- !IO request (Output)
extern uint8_t  z80_n_mreq;  ///<-- !Memory request (Output)
extern uint8_t  z80_n_rfsh;  ///<-- !DRAM refresh (Output)
extern uint8_t  z80_n_m1;    ///<-- !Instrunction-one (Output)

// Z80 input signals
extern uint8_t  z80_n_int;   ///<-- !Interrup (Input)
extern uint8_t  z80_n_nmi;   ///<-- !Non-maskable-interrupt (Input)
extern uint8_t  z80_n_reset; ///<-- !Reset (Input)
extern uint8_t  z80_n_wait;  ///<-- !Wait (Input)

// Z80 to-be-documented signals
extern uint8_t  z80_n_busreq; ///<-- !Bus request (???)
extern uint8_t  z80_n_busack; ///<-- !Bus-acknowledge (???)

#ifdef __cplusplus
}
#endif

#endif