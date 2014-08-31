#ifndef __FAKE_Z80_H
#define __FAKE_Z80_H

/**
 * This file provides the z80 buses and signals *only*.
 * No Z80 emulator is present here.
 */
#include <stdint.h>
//Z80 buses and signals
uint8_t  z80_data;     ///<-- Data bus, 8 bit wide (Input)
uint16_t z80_address;  ///<-- Address bus (bit 7 up selects PSG)
uint8_t     z80_n_wr = 1; ///<-- !Write enable (Input)
uint8_t     z80_n_ioreq = 1; ///<-- !IOreq

#endif