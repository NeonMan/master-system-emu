#include "io.h"

/*IO port for the IO chip*/
__sfr __at 0x3E __io_chip;

uint8_t io_enable(uint8_t f){
	uint8_t v;
	v = __io_chip;
	v = v & (f ^ 0xFF);
	__io_chip = v;
	return 0;
}

uint8_t io_disable(uint8_t f){
	uint8_t v;
	v = __io_chip;
	v = v | f;
	__io_chip = v;
	return 0;
}
