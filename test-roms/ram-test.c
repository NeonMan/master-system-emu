/**
 * @file  test-roms/ram-test.c
 * @brief Perform a RAM self-test.
 *
 * This program will check the correctness of the SMS RAM reads/writes.
 */

#include "sms/intv-dummy.h"
#include <stdint.h>
#include "sms/sdsc.h"
#include "sms/sms.h"

volatile uint8_t* const ram_base   = ((volatile uint8_t*)SMS_RAM_BASE_ADDRESS);
volatile uint8_t* const ram_mirror = ((volatile uint8_t*)SMS_RAM_MIRROR_ADDRESS);

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
	for(i=0x100; i<(SMS_RAM_SIZE-0x100); i++){
		ram_base[i] = (uint8_t)i;
	}
	
	/* Check if the pattern is available at the mirror */
	for(i=0; i<SMS_RAM_SIZE; i++){
		if(ram_base[i] != ram_mirror[i]){
			read_err(ram_base + i);
		}
	}
	
	sdsc_puts(" -- Mirror write/Base read.");
	/* Write a pattern on the upper mirror */
	for(i=0x100; i<(SMS_RAM_SIZE-0x100); i++){
		ram_mirror[i] = (uint8_t)i;
	}
	
	/* Check if the pattern is available at the mirror */
	for(i=0; i<SMS_RAM_SIZE; i++){
		if(ram_base[i] != ram_mirror[i]){
			read_err(ram_base + i);
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
