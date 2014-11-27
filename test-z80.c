#include "rom/rom.h"
#include "ram/ram.h"
#include "z80/z80.h"
#include "io/fake_io.h"
#include "sdsc/sdsc.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

int main(int argc, char* argv[]){
    //Redirect stds to files
    //*stderr = *fopen("std.err", "w");
    //*stdout = *fopen("std.out", "w");

    //Setup Z80
    z80_init(sdsc_write, sdsc_control);

    //Setup the ROM, all NOPs
    uint8_t *full_rom = malloc(ROM_MAX_SIZE);
    assert(full_rom);
    memset(full_rom, 0, ROM_MAX_SIZE);
    //If Try loading a rom. zexdoc_sdsc.sms
    {
        FILE* in_f = 0;
        //in_f = fopen("zexdoc_sdsc.sms", "rb");
        in_f = fopen("SDI.sms", "rb");
        if (in_f){
            fread(full_rom, 1, ROM_MAX_SIZE, in_f);
            fclose(in_f);
        }
        else{
            printf("Unable to open file\n");
        }
    }

    rom_set_image(full_rom, ROM_MAX_SIZE);
    free(full_rom);

    //Setup the (fake) IO chip. All enabled
    io_stat = 0;

    //Perform 100 ticks
    for (int i = 0; i < 40000; i++){
    //for (;;){
        z80_tick();
        ram_tick();
        rom_tick();
        z80_tick(); //Cycle 1

        z80_tick();
        ram_tick();
        rom_tick();
        z80_tick(); //Cycle 2

        z80_tick();
        ram_tick();
        rom_tick();
        z80_tick(); //Cycle 3
        
        z80_tick();
        ram_tick();
        rom_tick();
        z80_tick(); //Cycle 4
        
        z80_tick(); 
        ram_tick();
        rom_tick();
        z80_tick(); //Cycle 5
    }
    //fclose(stderr);
    //fclose(stdout);
    return 0;
}