#ifndef __Z80_DASM
#define __Z80_DASM

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int z80d_decode(uint8_t* opcode, unsigned int size, char* result);

#ifdef __cplusplus
}
#endif

#endif