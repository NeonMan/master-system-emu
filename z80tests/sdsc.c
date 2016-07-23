#include "sdsc.h"
__sfr __at 0xFD __sdsc_data;
__sfr __at 0xFC __sdsc_control;

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
	unsigned char h = ((c>>4) & 0xF) + '0';
	unsigned char l = ((c>>0) & 0xF) + '0';
	__sdsc_data = h;
	__sdsc_data = l;
}
