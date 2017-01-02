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
 * @file peripheral.h
 * @brief Peripheral ports module.
 */
#ifndef __PERIPHERIAL_H
#define __PERIPHERIAL_H

#include <stdint.h>
#include "../z80/z80_externs.h"
#include "../io/io_externs.h"

#define PER_PORTB_TH_OUT (1<<7)
#define PER_PORTB_TR_OUT (1<<6)
#define PER_PORTA_TH_OUT (1<<5)
#define PER_PORTA_TR_OUT (1<<4)
#define PER_PORTB_TH_DIR (1<<3)
#define PER_PORTB_TR_DIR (1<<2)
#define PER_PORTA_TH_DIR (1<<1)
#define PER_PORTA_TR_DIR (1<<0)

#define PER_PORTB_DOWN_IN  (1<<7)
#define PER_PORTB_UP_IN    (1<<6)
#define PER_PORTA_TR_IN    (1<<5)
#define PER_PORTA_TL_IN    (1<<4)
#define PER_PORTA_RIGHT_IN (1<<3)
#define PER_PORTA_LEFT_IN  (1<<2)
#define PER_PORTA_DOWN_IN  (1<<1)
#define PER_PORTA_UP_IN    (1<<0)

#define PER_PORTB_TH_IN    (1<<7)
#define PER_PORTA_TH_IN    (1<<6)
#define PER_CONT_IN        (1<<5)
#define PER_RESET_IN       (1<<4)
#define PER_PORTB_TR       (1<<3)
#define PER_PORTB_TL       (1<<2)
#define PER_PORTB_RIGHT_IN (1<<1)
#define PER_PORTB_LEFT_IN  (1<<0)

#ifdef __cplusplus
extern "C" {
#endif

///Updates the peripheral ports.
void per_tick();

//CONTROL2 UART emulation
void per_register_uart_rx(void(*callback)(uint8_t));
void per_uart_tx(char c);

//Debug
uint8_t* perdbg_reg_control();
uint8_t* perdbg_reg_ab();
uint8_t* perdbg_reg_bm();

#ifdef __cplusplus
}
#endif
#endif