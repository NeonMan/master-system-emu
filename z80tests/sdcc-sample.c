/*
 Build with:
   sdcc -mz80 --std-c99 -c 
*/

#include <stdint.h>
#include "sdsc.h"

void sdsc_test(){
	uint8_t i;
	
	//Simple FOR loop
	sdsc_puts("Print 10 dots\r\n1234567890");
	for(i=0;i<10;++i){
		sdsc_putc('.');
	}
	sdsc_puts("");
	
	//Print some integers
	sdsc_puts("Print hex numbers from 0x00 to 0xFF");
	for(i=0; i<16; ++i){
		sdsc_putc('0');
		sdsc_putc('x');
		sdsc_puth(i);
		sdsc_putc(' ');
	}
	sdsc_puts("");
}

int main(){
	sdsc_test();
	while(1){
		
	}
}
