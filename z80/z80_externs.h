#ifndef __Z80_EXTERNS_H
#define __Z80_EXTERNS_H

/**
 * The Z80 buses and signals. for modules communicating with the Z80.
 */

#include <stdint.h>
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

#endif