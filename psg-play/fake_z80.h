#ifndef __FAKE_Z80_H
#define __FAKE_Z80_H

/**
 * This file provides the z80 buses and signals *only*.
 * No Z80 emulator is present here.
 */

#include <stdint.h>
// Z80 Buses
uint8_t  z80_data;     ///<-- Data bus, 8 bit wide (Input/Output)
uint16_t z80_address;  ///<-- Address bus   (Output)

// Z80 output signals
uint8_t  z80_n_rd = 1; ///<-- !Read enable  (Output)
uint8_t  z80_n_wr = 1; ///<-- !Write enable (Output)
uint8_t  z80_n_ioreq = 1; ///<-- !IO request (Output)
uint8_t  z80_n_mreq = 1;  ///<-- !Memory request (Output)
uint8_t  z80_n_rfsh = 1; ///<-- !DRAM refresh (Output)
uint8_t  z80_n_m1 = 1;   ///<-- !Instrunction-one (Output)

// Z80 input signals
uint8_t  z80_n_int = 1; ///<-- !Interrup (Input)
uint8_t  z80_n_nmi = 1; ///<-- !Non-maskable-interrupt (Input)
uint8_t  z80_n_reset = 1; ///<-- !Reset (Input)
uint8_t  z80_n_wait = 1;   ///<-- !Wait (Input)

// Z80 to-be-documented signals
uint8_t  z80_n_busreq = 1; ///<-- !Bus request (???)
uint8_t  z80_n_busack = 1; ///<-- !Bus-acknowledge (???)

#endif