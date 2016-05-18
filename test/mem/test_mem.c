// Copyright 2015 Juan Luis Álvarez Martínez
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "z80/fake_z80.h"
#include "rom/rom.h"
#include "io/io.h"
#include "ram/ram.h"

uint8_t read_byte(uint16_t addr){
    z80_address = addr;
    z80_n_mreq = 0;
    z80_n_rd = 0; //Pull !RD down
    io_tick();
    rom_tick();
    ram_tick();
    z80_n_rd = 1; //Pull !RD up
    z80_n_mreq = 1;
    return z80_data;
}

void write_byte(uint16_t addr, uint8_t b){
    z80_address = addr;
    z80_data = b;
    z80_n_mreq = 0;
    z80_n_wr = 0;
    io_tick();
    rom_tick();
    ram_tick();
    z80_n_mreq = 1;
    z80_n_wr = 1;
}

int main(int argc, char**argv){

    uint8_t* full_rom = (uint8_t*) malloc(ROM_MAX_SIZE);
    assert(full_rom);

    //Initialize the full rom
    for (int i = 0; i < ROM_MAX_SIZE; i++){
        uint8_t rnd_b = rand() & 0xFF;
        full_rom[i] = rnd_b;
    }

    // ----------------------
    // --- ROM Bank tests ---
    // ----------------------


    //Upload the full_rom to the ROM.
    rom_set_image(full_rom, ROM_MAX_SIZE);
	
    //Test the slots
    int bank = 0;
    int all_ok = 1;
	
	//compare internal ROM and full ROM
	if(memcmp(full_rom, romdbg_get_rom(), ROM_MAX_SIZE) != 0){
		all_ok = 0;
	}
	
    for (bank = 0; bank < ROM_MAX_SIZE / 1024 / 16; bank++){
        int slot0_1k_ok = 1;
        int slot0_ok = 1;
        int slot1_ok = 1;
        int slot2_ok = 1;

        //Set the slots
        write_byte(0xFFFF, bank); //Slot2
        write_byte(0xFFFE, bank); //Slot1
        write_byte(0xFFFD, bank); //Slot0
		
		//Peep the slot via internal function
		if(*romdbg_get_slot(0) != bank) all_ok = 0;
		if(*romdbg_get_slot(1) != bank) all_ok = 0;
		if(*romdbg_get_slot(2) != bank) all_ok = 0;

        for (int i = 0; i < (1024 * 16); i++){
            //Slot0
            if (i < 1024){
                //First 1K
                slot0_1k_ok = slot0_1k_ok && (read_byte(i) == full_rom[i]);
            }
            else{
                slot0_ok = slot0_ok && (read_byte(i) == full_rom[i + (bank * 1024 * 16)]);
            }
            //Slot1
            slot1_ok = slot1_ok && (read_byte(i + (16 * 1024)) == full_rom[i + (bank * 1024 * 16)]);
            //Slot2
            slot2_ok = slot2_ok && (read_byte(i + (16 * 1024 * 2)) == full_rom[i + (bank * 1024 * 16)]);
        }
        if (slot0_ok && slot1_ok && slot2_ok){
            ; /*Do nothing*/
            /*printf("Bank %d OK\n", bank);*/
        }
        else{
            all_ok = 0;
            printf("Bank %d Err. %d(%d) %d %d\n", bank, slot0_ok, slot0_1k_ok, slot1_ok, slot2_ok);
        }
    }
    if (all_ok)
        printf("--- ROM OK ---\n");
    else
        printf("--- ROM: There were errors ---\n");

    // -----------------
    // --- RAM Tests ---
    // -----------------

    //Ram is 8K, mirrored twice on the last 16K slot

    //Initialize RAM
    for (int i = 0xC000; i < 0xE000; i++){
        write_byte(i, full_rom[i]);
    }

    //Test RAM (Read)
    int ram_ok = 1;
    for (int i = 0xC000; i < 0xE000; i++){
        ram_ok = ram_ok & (read_byte(i) == full_rom[i]);
        ram_ok = ram_ok & (read_byte(i + 0xE000 - 0xC000) == full_rom[i]);
    }

    if (ram_ok)
        printf("--- RAM READ OK ---\n");
    else
        printf("--- RAM READ Error ---\n");

	//Test RAM (Write)
	for (int i = 0xC000; i < 0xE000; i++) {
		write_byte(i, full_rom[i] ^ 0xFF);
		ram_ok = ram_ok & (read_byte(i) == (full_rom[i] ^0xFF));
		ram_ok = ram_ok & (read_byte(i + 0xE000 - 0xC000) == (full_rom[i] ^ 0xFF));

		write_byte(i + 0xE000 - 0xC000, full_rom[i]);
		ram_ok = ram_ok & (read_byte(i) == (full_rom[i]));
		ram_ok = ram_ok & (read_byte(i + 0xE000 - 0xC000) == (full_rom[i]));
	}

	if (ram_ok)
		printf("--- RAM WRITE OK ---\n");
	else
		printf("--- RAM WRITE Error ---\n");

	//Cleanup and exit
    free(full_rom);

    if (all_ok && ram_ok){
        return 0;
    }
    else{
        return -1;
    }
}
