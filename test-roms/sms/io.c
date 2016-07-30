#include "io.h"

/** Lower 2 bits must be always cleared, use this mask for that.*/
#define UNDEFINED_BIT_MASK 0xFC

/** Sets the IO chip register with the given value.*/
void io_set(uint8_t r){
  __io_chip = r;
}

/** Gets the IO register value.*/
uint8_t io_get(){
  return __io_chip;
}

/** Enables a feature.*/
uint8_t io_enable(uint8_t f){
  uint8_t new_reg;
  new_reg = __io_chip;
  new_reg = new_reg & f;
  new_reg = new_reg & UNDEFINED_BIT_MASK;
  
	__io_chip = new_reg;
	return new_reg;
}

/** Disables a feature.*/
uint8_t io_disable(uint8_t f){
  uint8_t new_reg;
  new_reg = __io_chip;
  new_reg = new_reg | f;
  new_reg = new_reg & UNDEFINED_BIT_MASK;
  
	__io_chip = new_reg;
	return __io_chip;
}
