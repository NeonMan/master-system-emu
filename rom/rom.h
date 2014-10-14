#ifndef __ROM_H
#define __ROM_H

#include <stdint.h>
#include <stddef.h>

//Required Z80 buses and signals
// Z80 Buses
extern uint8_t  z80_data;     ///<-- Data bus, 8 bit wide (Input/Output)
extern uint16_t z80_address;  ///<-- Address bus   (Output)

//Constants
#define ROM_MAX_SIZE (1024*1024*4) /**<-- Largest possible rom is 4M. */
#define ROM_SLOT0_ADDR 0xFFFD
#define ROM_SLOT1_ADDR 0xFFFE
#define ROM_SLOT2_ADDR 0xFFFF

//Functions

/**
* @brief Perform a ROM read/write. Sega mapper is implied.
*
* ROM is async, so every time this function is called, a read/write
* operation will be completed.
*/
void rom_tick();

/**
 * @brief Writes the rom image to be used.
 *
 * @param data Pointer to the rom bytes
 * @param count Size of the rom
 */
void rom_set_image(uint8_t* data, size_t count);

#endif
