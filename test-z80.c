#include "rom/rom.h"
#include "ram/ram.h"
#include "z80/z80.h"
#include "io/fake_io.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int main(){

    //Setup the ROM, all NOPs
    uint8_t *full_rom = malloc(ROM_MAX_SIZE);
    memset(full_rom, 0, ROM_MAX_SIZE);
    rom_set_image(full_rom, ROM_MAX_SIZE);
    free(full_rom);

    //Setup the (fake) IO chip. All enabled
    io_stat = 0;

    //Perform 100 ticks
    for (int i = 0; i < 100; i++){
        z80_tick();
        z80_tick();
        z80_tick();
        ram_tick();
        rom_tick();
        z80_tick();
        z80_tick();
        z80_tick();
        z80_tick();
    }
    return 0;
}