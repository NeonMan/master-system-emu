#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../z80/fake_z80.h"
#include "../rom/rom.h"

uint8_t read_byte(uint16_t addr){
    z80_address = addr;
    z80_n_mreq = 0;
    z80_n_rd = 0; //Pull !RD down
    rom_tick();
    z80_n_rd = 1; //Pull !RD up
    z80_n_mreq = 1;
    return z80_data;
}

void write_byte(uint16_t addr, uint8_t b){
    z80_address = addr;
    z80_data = b;
    z80_n_mreq = 0;
    z80_n_wr = 0;
    rom_tick();
    z80_n_mreq = 1;
    z80_n_wr = 1;
}

int test_first_k(){
    //For each slot config
    for (int j = 0; j < 256; j++){
        write_byte(ROM_SLOT0_ADDR, j);
        for (uint32_t i = 0; i < 0x03ff; i += sizeof(uint32_t)){
            uint32_t v;
            v = read_byte(i);
            v += read_byte(i + 1) << 8;
            v += read_byte(i + 2) << 16;
            v += read_byte(i + 3) << 24;
            if (v != (i / 4)){
                printf("mismatch reading address 0x%X. Found: 0x%X\n", i, v);
                return 0;
            }
        }
    }
    return 1;
}

int main(int argc, char**argv){
    size_t item_count = ROM_MAX_SIZE / sizeof(uint32_t);
    uint32_t *rom = malloc(ROM_MAX_SIZE);

    for (uint32_t i = 0; i < item_count; i++){
        rom[i] = i;
    }
    rom_set_image((uint8_t*)rom, ROM_MAX_SIZE);

    //Test the correctness of the SLOT mapping
    //Unmapped first 1K
    test_first_k();
    //ToDo: SLOT0
    //ToDo: SLOT1
    //ToDo: SLOT2

    free(rom);
    return 1;
}