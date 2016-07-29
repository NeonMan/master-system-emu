#include "sdsc.h"
__sfr __at 0xFD __sdsc_data;
__sfr __at 0xFC __sdsc_control;

static const char hex_lut[] = "0123456789ABCDEF";

void sdsc_putc(char c){
	__sdsc_data = c;
}

void sdsc_puts(const char* str){
	while(*str){
		__sdsc_data = *str;
		++str;
	}
	__sdsc_data = '\r';
	__sdsc_data = '\n';
}

void sdsc_puth(char c){
	unsigned char h = ((c>>4) & 0xF);
	unsigned char l = ((c   ) & 0xF);
	__sdsc_data = hex_lut[h];
	__sdsc_data = hex_lut[l];
}
