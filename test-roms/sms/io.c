#include "io.h"

void io_set(uint8_t r){
  __io_chip = r;
}

uint8_t io_get(){
  return __io_chip;
}

uint8_t io_enable(uint8_t f){
	__io_chip = __io_chip & f;
	return __io_chip;
}

uint8_t io_disable(uint8_t f){
	__io_chip = __io_chip | f;
	return __io_chip;
}
