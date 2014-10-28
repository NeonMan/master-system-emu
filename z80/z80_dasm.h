#ifndef __Z80_DASM
#define __Z80_DASM

#include <stdint.h>

int z80d_decode(uint8_t* opcode, char* result);

#endif