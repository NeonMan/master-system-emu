#ifndef __Z80_DECODER_H
#define __Z80_DECODER_H

#include "z80.h"

#ifdef __cplusplus
extern "C" {
#endif

//Sdsc data/control function pointers (decouple z80 from sdsc.h)
//void(*z80_sdsc_data) (uint8_t) = 0; ///<-- Function pointer for SDSC data port.
//void(*z80_sdsc_control) (uint8_t) = 0; ///<-- Function pointer for SDSC control port.

extern struct z80_s z80; //<-- Requires access to the z80 internals

//Functions
int z80_instruction_decode();

#ifdef __cplusplus
}
#endif

#endif