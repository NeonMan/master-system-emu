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
