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

/**
 * @file peripherial.c
 * @brief peripherial ports implementation.
 *
 * Implements the master system peripherial ports, usually used by gamepads.
 */
#include "peripherial.h"

/**
 * Peripherial ports, the two 9-pin D-sub things at the front
 * Relevant ports 3F DC DD.
 * 
 * Port 0x3F: IO port control (Write only?)
 * Port 0xDC: Port A and B    (Read only?)
 * Port 0xDD: Port B and misc (Read only?)
 */

uint8_t per_control = 0xFF; ///<-- Port configuration. All input by default
uint8_t per_port_ab = 0xFF; ///<-- Port AB register
uint8_t per_port_bm = 0xFF; ///<-- Port B+Misc register

void per_tick(){
    const uint8_t addrl = z80_address & 0x00FF;

    //If chip is not selected, return
    if (io_stat & IO_PERIPHERIAL) return;

    //Peripherial control register.
    //is a write to 0x3F (and all mirrors)
    //All odd addresses from 0x01 to 0x3F
    if   ((z80_n_wr == 0) && (addrl <= 0x3F) && (addrl & 0x01)){
        per_control = z80_data;
    }

    //PortAB read.
    //is a read from 0xDC port (and all its mirrors)
    //All even address from 0xC0 to 0xFE
    else if ((z80_n_rd == 0) && (addrl >= 0xC0) && ((addrl & 0x01) == 0) && (addrl <= 0xFE)){
        z80_data = per_port_ab;
    }

    //Port B+Misc.
    //is a read from 0xDD port (and all its mirrors)
    //All odd address from 0xC1 to 0xFF
    else if ((z80_n_rd == 0) && (addrl >= 0xC1) && (addrl & 0x01) && (addrl <= 0xFF)){
        z80_data = per_port_bm;
    }
}