//Emulator includes
#include "io/io.h"
#include "peripherial/peripherial.h"
#include "psg/psg.h"
#include "ram/ram.h"
#include "rom/rom.h"
#include "sdsc/sdsc.h"
#include "vdp/vdp.h"
#include "z80/z80.h"

//Other includes
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "sms-emu.h"

SDL_Window* emu_window;

// ######################################################
// ## Replace this function with something better ASAP ##
// ######################################################

//Naive and slow conversion from VDP framebuffer to SDL surface.
int vdp_to_sdl(void* vdp_fb, void* vdp_pal, SDL_Surface* surf){
	uint8_t* fb = (uint8_t*)vdp_fb;
	if (surf->h < VDP_HEIGHT_PIXELS) return 0;
    if (surf->w < VDP_WIDTH_PIXELS) return 0;

    int i;
    /*OpenMP might speedup things*/
    /*Don't know how SDL behaves with OpenMP*/
    //#pragma omp parallel for private(i)
    for (i = 0; i < VDP_FRAMEBUFFER_SIZE; i++){
        const uint32_t red = (((uint32_t)fb[i] & (3 << 4)) >> 4) << (16 + 6);
        const uint32_t green = (((uint32_t)fb[i] & (3 << 2)) >> 2) << (8 + 6);
        const uint32_t blue = (((uint32_t)fb[i] & (3 << 0)) >> 0) << (0 + 6);
        const uint32_t alpha = fb[i] ? 0xff000000 : 0x00000000;
        const uint32_t color = red | green | blue | alpha;
        SDL_Rect r;
        r.w = 1;
        r.h = 1;
        r.x = (i % VDP_WIDTH_PIXELS) + 32;
        r.y = (i / VDP_WIDTH_PIXELS) + 32;
        SDL_FillRect(surf, &r, color);
    }
    return 1;
}
// ######################################################

void emu_log(char* msg, int level){
    SDL_GetTicks();
    fprintf(stderr, "%09d [%s] %s\n", SDL_GetTicks(), emu_logelvel_names[level][0], msg);
}

void emu_cleanup(){
    emu_log("Emulator exitting", EMU_LOG_INFO);
    Mix_Quit();
    SDL_Quit();
}

int emu_init(){
    // Initialize libraries
    int rv;
    vdp_init();
    z80_init(sdsc_write, sdsc_control);
    rv = SDL_Init(SDL_INIT_EVERYTHING);
    if (!(rv | SDL_INIT_VIDEO))
        return -1;
    rv = Mix_Init(0);

    emu_window = SDL_CreateWindow("SMSE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 256 + 64, 192 + 64, 0);
    if (!emu_window)
        return -2;

    // Load a ROM
    uint8_t* full_rom = malloc(ROM_MAX_SIZE);
    memset(full_rom, 0, ROM_MAX_SIZE);
    FILE* in_f = fopen("zexdoc.sms", "rb");
    if (in_f == 0){
        emu_log("Failed to load rom", EMU_LOG_CRITICAL);
        return -3;
    }
    size_t read_bytes = fread(full_rom, 1, ROM_MAX_SIZE, in_f);
    char read_bytes_s[32];
    _itoa(read_bytes, read_bytes_s, 10);
    emu_log("ROM Loaded. Size:", EMU_LOG_DEBUG0);
    emu_log(read_bytes_s, EMU_LOG_DEBUG0);
    _itoa(ROM_MAX_SIZE, read_bytes_s, 10);
    emu_log("Max:", EMU_LOG_DEBUG0);
    emu_log(read_bytes_s, EMU_LOG_DEBUG0);
    fclose(in_f);
    rom_set_image(full_rom, ROM_MAX_SIZE);
    free(full_rom);

    //All OK
    return 1;
}

void emu_loop(){
    uint8_t running = 1;
    unsigned int edge_count = 0;
    uint8_t framebuffer[VDP_FRAMEBUFFER_SIZE];

    SDL_Surface* screen = SDL_GetWindowSurface(emu_window);
    SDL_FillRect(screen, 0, 128);

    while (running){
        //
        // Every clock edge, the z80 is 'ticked'. Other modules must be clocked
        // synchronously at different rates (clock divider). Other modules can
        // be ticked asynchronosly when required.
        //
        // Synchronous modules:
        //   * z80
        //   * PSG (16 prescaler/32 edge prescaler)
        //   * VDP (no prescaler?)
        //
        // Asynchronous modules:
        //  * RAM
        //  * ROM
        //  * IO  (must be ticked before any MREQ/IOREQ operation)
        //  * Peripherial
        //
        z80_tick();
        if ((edge_count % 2) == 0)
            vdp_tick();
        if ((edge_count % 32) == 0)
            psg_tick();
        if ((!z80_n_mreq) && ((!z80_n_rd) || (!z80_n_wr))){ //Memory-mapped operation
            io_tick();
            rom_tick();
            ram_tick();
        }
        if ((!z80_n_ioreq) && ((!z80_n_rd) || (!z80_n_wr))){ //IO operation
            io_tick();
            per_tick();
        }

        ///Update SDL. 59659
        if ((edge_count % 59659) == 0){
            vdp_to_sdl(framebuffer, 0, screen);
            SDL_UpdateWindowSurface(emu_window);
        }
        SDL_Event evt;
        while (SDL_PollEvent(&evt)){
            if (evt.type == SDL_QUIT)
                running = 0;
        }

        ++edge_count;
    }
}

int main(int argc, char** argv){
    // --- Initializing ---
    if (emu_init()<0){
        emu_log("Failed to initialize", EMU_LOG_CRITICAL);
        emu_cleanup();
        return -1;
    }
    emu_log("Emulator started", EMU_LOG_INFO);

    emu_loop();

    // --- Cleanup and exit ---
    emu_cleanup();
    return 0;
}
