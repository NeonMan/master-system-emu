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
 * @file  peripheral/peripheral.c
 * @brief peripheral ports implementation.
 *
 * Implements the master system peripherial ports, usually used by gamepads.
 */
#include "peripheral.h"

/**
 * Peripherial ports, the two 9-pin D-sub things at the front
 * Relevant ports 3F DC DD.
 * 
 * Port 0x3F: IO port control (Write only?)
 * Port 0xDC: Port A and B    (Read only?)
 * Port 0xDD: Port B and misc (Read only?)
 */

static uint8_t per_control = 0xFF; ///<-- Port configuration. All input by default
static uint8_t per_port_ab = 0xFF; ///<-- Port AB register
static uint8_t per_port_bm = 0xFF; ///<-- Port B+Misc register

//UART port emulation
#define RX_TICKS_PER_SAMPLE 100
static uint8_t  rx_char;
static uint8_t  rx_is_sampling = 0;
static uint32_t rx_tick_count;
static uint8_t  rx_sample_count;
static void(*uart_rx_callback)(uint8_t) = 0;

void per_register_uart_rx(void(*callback)(uint8_t)) {
    uart_rx_callback = callback;
}

void per_uart_tx(char c) {
    /*ToDo*/
}

uint8_t* perdbg_reg_control(){
    return &per_control;
}

uint8_t* perdbg_reg_ab(){
    return &per_port_ab;
}

uint8_t* perdbg_reg_bm(){
    return &per_port_bm;
}

void per_tick(){
    const uint8_t addrl = z80_address & 0x00FF;

    //If chip is not selected, return
    if (io_stat & IO_PERIPHERIAL) return;

    //If not ioreq, return
    if (z80_n_ioreq) return;

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
    else if ((z80_n_rd == 0) && (addrl >= 0xC1) && (addrl & 0x01)){ /*&& (addrl <= 0xFF)  <-- always true ;)*/
        z80_data = per_port_bm;
    }

    //UART port emulation
    if (rx_is_sampling) {
        /*We are sampling~*/
        rx_tick_count++;
        if      (rx_tick_count > (RX_TICKS_PER_SAMPLE * 10)) {
            /*Stop bit*/
            /*Framing error if Zero*/
            if (rx_sample_count == 9) {

                rx_sample_count++;
                if (uart_rx_callback) {
                    uart_rx_callback(rx_char);
                }
            }
            rx_is_sampling = 0;
        }
        else if (rx_tick_count > (RX_TICKS_PER_SAMPLE * 9)) {
            /*Data bit 7*/
            if (rx_sample_count == 8) {
                rx_char = (rx_char >> 1) | ((per_control & PER_PORTB_TH_OUT) ? (1 << 7) : 0);
                rx_sample_count++;
            }
        }
        else if (rx_tick_count > (RX_TICKS_PER_SAMPLE * 8)) {
            /*Data bit 6*/
            if (rx_sample_count == 7) {
                rx_char = (rx_char >> 1) | ((per_control & PER_PORTB_TH_OUT) ? (1 << 7) : 0);
                rx_sample_count++;
            }
        }
        else if (rx_tick_count > (RX_TICKS_PER_SAMPLE * 7)) {
            /*Data bit 5*/
            if (rx_sample_count == 6) {
                rx_char = (rx_char >> 1) | ((per_control & PER_PORTB_TH_OUT) ? (1 << 7) : 0);
                rx_sample_count++;
            }
        }
        else if (rx_tick_count > (RX_TICKS_PER_SAMPLE * 6)) {
            /*Data bit 4*/
            if (rx_sample_count == 5) {
                rx_char = (rx_char >> 1) | ((per_control & PER_PORTB_TH_OUT) ? (1 << 7) : 0);
                rx_sample_count++;
            }
        }
        else if (rx_tick_count > (RX_TICKS_PER_SAMPLE * 5)) {
            /*Data bit 3*/
            if (rx_sample_count == 4) {
                rx_char = (rx_char >> 1) | ((per_control & PER_PORTB_TH_OUT) ? (1 << 7) : 0);
                rx_sample_count++;
            }
        }
        else if (rx_tick_count > (RX_TICKS_PER_SAMPLE * 4)) {
            /*Data bit 2*/
            if (rx_sample_count == 3) {
                rx_char = (rx_char >> 1) | ((per_control & PER_PORTB_TH_OUT) ? (1 << 7) : 0);
                rx_sample_count++;
            }
        }
        else if (rx_tick_count > (RX_TICKS_PER_SAMPLE * 3)) {
            /*Data bit 1*/
            if (rx_sample_count == 2) {
                rx_char = (rx_char >> 1) | ((per_control & PER_PORTB_TH_OUT) ? (1 << 7) : 0);
                rx_sample_count++;
            }
        }
        else if (rx_tick_count > (RX_TICKS_PER_SAMPLE * 2)) {
            /*Data bit 0*/
            if (rx_sample_count == 1) {
                rx_char = (rx_char >> 1) | ((per_control & PER_PORTB_TH_OUT) ? (1 << 7) : 0);
                rx_sample_count++;
            }
        }
        else if (rx_tick_count > (RX_TICKS_PER_SAMPLE * 1)) {
            /*Start bit*/
            /*Framing error if one*/
            rx_sample_count = 1;
        }
    }
    else {
        /*If RX line is down, start sampling*/
        if(!(per_control & PER_PORTB_TH_OUT)){
            rx_sample_count = 0;
            rx_tick_count = 0;
            rx_char = 0;
            rx_is_sampling = 1;
        }
    }
}
