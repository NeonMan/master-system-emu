#include "emu-hooks.h"

/*
Emulator hooks use the NONI/HALT instruction to poke around the emulator
internals from ROM code.



*/

uint16_t old_ix;
uint16_t old_iy;


void emu_hook(uint8_t op, uint16_t arg1, uint16_t arg2){
	(void) op;     /**/
	(void) arg_ix; /**/
	(void) arg_iy; /* Make sure the compiler does not optimize-out this. */
	
	/*Call hook*/
	__asm__(".db 0xFD");
	__asm__("HALT");
}
