/*
 Build with:
   sdcc -mz80 --std-c99 -c 
*/

#include <stdint.h>
#include "sdsc.h"
#include "emu-hooks.h"
#include "intv-dummy.h" /*<-- Ignore interrupts*/

/* ------------------------------------- */
/* --- Print stuff do some for loops --- */
/* ------------------------------------- */
void printing_test(){
	uint16_t i;
	//Simple FOR loop
	sdsc_puts("Print 10 dots");
	sdsc_puts("1234567890");
	for(i=0;i<10;++i){
		sdsc_putc('.');
	}
	sdsc_puts("");
	
	//Print some integers
	sdsc_puts("Print hex numbers from 0x00 to 0xFF");
	for(i=0; i<256; ++i){
		if(((i%16)==0) && (i>0)){
			sdsc_puts("");
		}
		sdsc_putc('0');
		sdsc_putc('x');
		sdsc_puth(i);
		sdsc_putc(' ');
	}
	sdsc_puts("");
}



void main(){
	/*Say Hello!*/
	sdsc_puts("Test program for sms emu.");
	sdsc_puts("");
	
	/*Do tests*/
	printing_test();
	
	/*Stop emulation*/
	emu_hook(EMU_BREAK, 0xAAAA, 0x7777);
	while(1){
		
	}
}
