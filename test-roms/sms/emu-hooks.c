/**
 * @file  test-roms/sms/emu-hooks.c
 * @brief Access to emulator functionality.
 *
 * Emulator hooks use the NONI/HALT instruction to poke around the emulator
 * internals from ROM code.
 */
#include "emu-hooks.h"

void emu_hook(uint8_t op, uint16_t arg1, uint16_t arg2){
	(void) op;   /* */
	(void) arg1; /* */
	(void) arg2; /* Make sure the compiler does not optimize-out this. */
	
	/*Call hook*/
	__asm__(".db 0xFD");
	__asm__("HALT");
}
