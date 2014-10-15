/**
 *  @file rom.c
 *  @brief Implementation of ROM cartridge.
 *
 *  Implements a master system cartridge emulating a full sega mapper.
 */
#include "../z80/z80_externs.h"
#include "../io/io_externs.h"
#include <string.h>

#include "rom.h"

uint8_t mapper_slots[3] = {0,1,2}; ///<-- ROM slot selector
uint8_t mapper_ram = 0; ///<-- RAM slot config @Todo
uint8_t rom_image[ROM_MAX_SIZE]; ///<-- ROM contents.

void rom_set_image(uint8_t* data, size_t count){
    if (count < 1) return;
    memcpy(rom_image, data, count <= ROM_MAX_SIZE ? count : ROM_MAX_SIZE);
}

void rom_tick(){
    //if MREQ is high there's nothing to do. return.
    //Since MREQ is shared with other circuits, we 
    //use the IO mapper instead (#CE pin).
    if (io_stat & IO_CARTRIDGE_SLOT) return;
    //if RD is low and address is in ROM space (first 48K), perform a read operation
    if ((z80_n_rd == 0) && (z80_address < 0xC000)){
        // ----------------
        // --- ROM READ ---
        // ----------------
        if      (z80_address <= 0x03FF)
            z80_data = rom_image[z80_address]; //First 1K is unmapped
        //Slot 0
        else if (z80_address <= 0x3FFF)
            z80_data = rom_image[(0x4000 * mapper_slots[0]) + (z80_address & 0x3FFF)];
        //Slot 1
        else if (z80_address <= 0x7FFF)
            z80_data = rom_image[(0x4000 * mapper_slots[1]) + (z80_address & 0x3FFF)];
        //Slot 2
        else /*if (z80_address <= 0xBFFF)*/
            z80_data = rom_image[(0x4000 * mapper_slots[2]) + (z80_address & 0x3FFF)];
    }
    //If WR is low and address is in mapper space (last 4 bytes), perform a mapper write
    else if ((z80_n_wr == 0) && (z80_address >= 0xfffc)){
        // --------------------
        // --- MAPPER WRITE ---
        // --------------------
        if   (z80_address == 0xFFFC) mapper_ram = z80_data;
        else mapper_slots[z80_address - 0xFFFD] = z80_data;
    }
}
