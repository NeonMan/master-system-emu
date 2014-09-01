#ifndef __ROM_H
#define __ROM_H

#ifdef NDEBUG
#pragma comment(lib, "../Release/rom.lib")
#else
#pragma comment(lib, "../Debug/rom.lib")
#endif

#include <stdint.h>

//Required Z80 buses and signals
// Z80 Buses
extern uint8_t  z80_data;     ///<-- Data bus, 8 bit wide (Input/Output)
extern uint16_t z80_address;  ///<-- Address bus   (Output)

// Z80 output signals
extern uint8_t  z80_n_rd = 1;   ///<-- !Read enable  (Output)
extern uint8_t  z80_n_wr = 1;   ///<-- !Write enable (Output)
extern uint8_t  z80_n_mreq = 1; ///<-- !Memory request (Output)
extern uint8_t  z80_n_m1 = 1;   ///<-- !Instrunction-one (Output)

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