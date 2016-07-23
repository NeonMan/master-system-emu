/*
 Build with:
   sdcc -mz80 --std-c99 -c 
*/

#include <stdint.h>
#include "sdsc.h"

void sdsc_test(){
	uint8_t i;
	//Say Hello!
	sdsc_puts("Hello world!");
	
	//Simple FOR loop
	for(i=0;i<10;++i){
		sdsc_putc('.');
	}
	sdsc_puts("");
	
	//Print some integers
	for(i=0; i<16; ++i){
		sdsc_putc('0');
		sdsc_putc('x');
		sdsc_puth(i);
		sdsc_puts("");
	}
}

int main(){
	sdsc_test();
	while(1){
		
	}
}
