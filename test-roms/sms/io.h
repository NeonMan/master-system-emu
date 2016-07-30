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

#define IO_ENABLE_EXPANSION  (~IO_EXPANSION)
#define IO_ENABLE_CARTRIDGE  (~IO_CARTRIDGE)
#define IO_ENABLE_CARD       (~IO_CARD)
#define IO_ENABLE_RAM        (~IO_RAM)
#define IO_ENABLE_BIOS       (~IO_BIOS)
#define IO_ENABLE_PERIPHERAL (~IO_PERIPHERAL)
#define IO_ENABLE_UNK1       (~IO_UNK1)
#define IO_ENABLE_UNK0       (~IO_UNK0)

#define IO_DISABLE_EXPANSION  (IO_EXPANSION)
#define IO_DISABLE_CARTRIDGE  (IO_CARTRIDGE)
#define IO_DISABLE_CARD       (IO_CARD)
#define IO_DISABLE_RAM        (IO_RAM)
#define IO_DISABLE_BIOS       (IO_BIOS)
#define IO_DISABLE_PERIPHERAL (IO_PERIPHERAL)
#define IO_DISABLE_UNK1       (IO_UNK1)
#define IO_DISABLE_UNK0       (IO_UNK0)

#define IO_PORT_ADDRESS 0x3E

/*IO port for the IO chip*/
__sfr __at IO_PORT_ADDRESS __io_chip;

uint8_t io_enable(uint8_t f);
uint8_t io_disable(uint8_t f);
void    io_set(uint8_t r);
uint8_t io_get();

#endif
