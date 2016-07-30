/**
 * @file  io.h
 * @brief Header for io module.
 */

#ifndef __IO_H
#define __IO_H

#include <stdint.h>

#define IO_EXPANSION  0x80 /*(1<<7)*/
#define IO_CARTRIDGE  0x40 /*(1<<6)*/
#define IO_CARD       0x20 /*(1<<5)*/
#define IO_RAM        0x10 /*(1<<4)*/
#define IO_BIOS       0x08 /*(1<<3)*/
#define IO_PERIPHERAL 0x04 /*(1<<2)*/
#define IO_UNK1       0x02 /*(1<<1)*/
#define IO_UNK0       0x01 /*(1   )*/

#define IO_ENABLE_EXPANSION  0x7F /*(~IO_EXPANSION)*/
#define IO_ENABLE_CARTRIDGE  0xBF /*(~IO_CARTRIDGE)*/
#define IO_ENABLE_CARD       0xDF /*(~IO_CARD)*/
#define IO_ENABLE_RAM        0xEF /*(~IO_RAM)*/
#define IO_ENABLE_BIOS       0xF7 /*(~IO_BIOS)*/
#define IO_ENABLE_PERIPHERAL 0xFB /*(~IO_PERIPHERAL)*/
#define IO_ENABLE_UNK1       0xFD /*(~IO_UNK1)*/
#define IO_ENABLE_UNK0       0xFE /*(~IO_UNK0)*/

#define IO_DISABLE_EXPANSION  IO_EXPANSION
#define IO_DISABLE_CARTRIDGE  IO_CARTRIDGE
#define IO_DISABLE_CARD       IO_CARD
#define IO_DISABLE_RAM        IO_RAM
#define IO_DISABLE_BIOS       IO_BIOS
#define IO_DISABLE_PERIPHERAL IO_PERIPHERAL
#define IO_DISABLE_UNK1       IO_UNK1
#define IO_DISABLE_UNK0       IO_UNK0

/** Lower 2 bits must be always cleared, use this mask for that.*/
#define UNDEFINED_BIT_MASK 0xFC

#define IO_PORT_ADDRESS 0x3E

/*IO port for the IO chip*/
__sfr __at IO_PORT_ADDRESS __io_chip;

uint8_t io_enable(uint8_t f);
uint8_t io_disable(uint8_t f);
void    io_set(uint8_t r);
uint8_t io_get();

#endif
