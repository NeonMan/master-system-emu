#include "sms/intv-dummy.h"
#include <stdint.h>
#include "sms/sdsc.h"

volatile uint8_t* const RAM_BASE   = ((volatile uint8_t*)0xc000);
volatile uint8_t* const RAM_MIRROR = ((volatile uint8_t*)0xe000);
#define RAM_SIZE   0x2000

static uint8_t all_ok;

static void read_err(uint16_t* addr){
	uint16_t addr_val = (uint16_t) addr;
	if(all_ok){
		sdsc_puts("Read missmatch at:");
		all_ok = 0;
	}
	sdsc_putc('0');
	sdsc_putc('x');
	sdsc_puth(addr_val>>8);
	sdsc_puth(addr_val);
	sdsc_puts("");
}

void main(){
	uint16_t i;

	/* Say hello */
	sdsc_puts("SMS RAM self-test.");
	all_ok = 1;
	
	/* --------------------------- */
	/* --- RAM read-write test --- */
	/* --------------------------- */
	sdsc_puts(" -- Base write/Mirror read.");
	/* Write a pattern on the lower mirror */
	/* Not all of it since this test needs some RAM ;) */
	for(i=0x100; i<(RAM_SIZE-0x100); i++){
		RAM_BASE[i] = (uint8_t)i;
	}
	
	/* Check if the pattern is available at the mirror */
	for(i=0; i<RAM_SIZE; i++){
		if(RAM_BASE[i] != RAM_MIRROR[i]){
			read_err(RAM_BASE + i);
		}
	}
	
	sdsc_puts(" -- Mirror write/Base read.");
	/* Write a pattern on the upper mirror */
	for(i=0x100; i<(RAM_SIZE-0x100); i++){
		RAM_MIRROR[i] = (uint8_t)i;
	}
	
	/* Check if the pattern is available at the mirror */
	for(i=0; i<RAM_SIZE; i++){
		if(RAM_BASE[i] != RAM_MIRROR[i]){
			read_err(RAM_BASE + i);
		}
	}
	
	/* --------------------------- */
	/* --- Show result message --- */
	/* --------------------------- */
	if(all_ok){
		sdsc_puts("Test OK");
	}
	else{
		sdsc_puts("Test FAIL");
	}
	
	while(1){
		;
	}
}