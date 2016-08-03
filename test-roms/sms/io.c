/**
 * @file  test-roms/sms/io.c
 * @brief Functions to control the IO chip state.
 */
#include "io.h"
#include "sms.h"

/** Copy of the IO register.*/
uint8_t io_reg;

/** Sets the IO chip register with the given value.*/
void io_set(uint8_t r){
  __io_chip = r;
  io_reg = r;
}

/** Gets the IO register value.*/
uint8_t io_get(){
  return io_reg;
}

/** Enables a feature.*/
uint8_t io_enable(uint8_t f){
  uint8_t new_reg;
  new_reg = io_reg;
  new_reg = new_reg & f;
  new_reg = new_reg & UNDEFINED_BIT_MASK;
  
	__io_chip = new_reg;
  io_reg = new_reg;
	return new_reg;
}

/** Disables a feature.*/
uint8_t io_disable(uint8_t f){
  uint8_t new_reg;
  new_reg = io_reg;
  new_reg = new_reg | f;
  new_reg = new_reg & UNDEFINED_BIT_MASK;
  
	__io_chip = new_reg;
  io_reg = new_reg;
	return new_reg;
}
