#ifndef __IO_H
#define __IO_H

#include <stdint.h>
#include "../z80/z80_externs.h"

//IO mapper enable bits. Active LOW
#define IO_EXPANSION_SLOT (1<<7)
#define IO_CARTRIDGE_SLOT (1<<6)
#define IO_CARD_SLOT      (1<<5)
#define IO_RAM            (1<<4)
#define IO_BIOS           (1<<3)
#define IO_PERIPHERIAL    (1<<2)
#define IO_UNK1           (1<<1)
#define IO_UNK0           (1)

extern uint8_t io_stat;
void io_tick();

#endif