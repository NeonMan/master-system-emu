/*
  IO chip headers.
*/

#ifndef __IO_H
#define __IO_H

#include <stdint.h>

#define IO_EXPANSION  (1<<7)
#define IO_CARTRIDGE  (1<<6)
#define IO_CARD       (1<<5)
#define IO_RAM        (1<<4)
#define IO_BIOS       (1<<3)
#define IO_PERIPHERAL (1<<2)
#define IO_UNK1       (1<<1)
#define IO_UNK0       (1   )

uint8_t io_enable(uint8_t f);
uint8_t io_disable(uint8_t f);

#endif
