#include <SDL/SDL.h>
#include <SDL/SDL_version.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "z80/fake_z80.h"

#include "vdp/vdp.h"
#include "vdp/vdp_constants.h"
#include "vdp/vdp_font.h"

//Naive and slow conversion from VDP framebuffer to SDL surface.
int vdp_to_sdl(void* vdp_fb, void* vdp_pal, SDL_Surface* surf){
    if (surf->h < VDP_HEIGHT_PIXELS) return 0;
    if (surf->w < VDP_WIDTH_PIXELS) return 0;
    uint8_t* fb = (uint8_t*)vdp_fb;

    for (int i = 0; i < VDP_FRAMEBUFFER_SIZE; i++){
        const uint32_t color = vdp_tmscolors[ fb[i]];
        SDL_Rect r;
        r.w = 1;
        r.h = 1;
        r.x = i % VDP_WIDTH_PIXELS;
        r.y = i / VDP_WIDTH_PIXELS;
        SDL_FillRect(surf, &r, color);
    }
    return 1;
}

void init(){
    SDL_Quit();
}

void cleanup(){
    SDL_version ver;
    SDL_Init(SDL_INIT_VIDEO);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    //SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    printf("SDL Rev: %s\n", SDL_GetRevision());
    SDL_GetVersion(&ver);
    printf("SDL Ver: %d.%d.%d\n", ver.major, ver.minor, ver.patch);

}

void control_write(uint16_t control_word){
    z80_n_ioreq = 0;
    z80_n_wr = 0;
    z80_address = 0xBF;
    z80_data = control_word & 0xFF; //<--
    vdp_tick();
    z80_n_ioreq = 1;
    z80_n_wr = 1;
    vdp_tick();         /*Write first byte*/
    z80_n_ioreq = 0;
    z80_n_wr = 0;
    z80_address = 0xBF;
    z80_data = (control_word>>8) & 0xFF; //<--
    vdp_tick();
    z80_n_ioreq = 1;
    z80_n_wr = 1;
    vdp_tick();         /*Write second byte*/
}

void register_write(uint8_t reg, uint8_t val){
    uint16_t control_word = val + (((uint16_t)reg) << 8) + (2 << 14);
    control_write(control_word);
}

void cram_write(uint16_t addr){
    uint16_t control_word = (3 << 14) + (addr & 0x3F);
    control_write(control_word);
}

void vram_write(uint16_t addr){
    uint16_t control_word = (1 << 14) + (addr & 0x3FFF);
    control_write(control_word);
}

void data_write(uint8_t b){
    z80_n_ioreq = 0;
    z80_n_wr = 0;
    z80_address = 0xBE;
    z80_data = b; //<--
    vdp_tick();
    z80_n_ioreq = 1;
    z80_n_wr = 1;
    vdp_tick();         /*Write first byte*/
}

int main(int argc, char** argv){
    init();
    //Create a SDL window
    SDL_Window* window = SDL_CreateWindow("VDP testing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 256 + 64, 192 + 64, 0);
    SDL_Surface* screen = SDL_GetWindowSurface(window);
    SDL_FillRect(screen, 0, 128);
    //Paint somebkg color

    // --- VDP tests ---
    //VDP initialization
    vdp_init();

    //Read H counter (Port: 7F)
    z80_n_ioreq = 0;
    z80_n_rd = 0;
    z80_address = 0x7F;
    vdp_tick();
    printf("H Counter is: 0x%02X\n", z80_data);
    z80_n_ioreq = 1;
    z80_n_rd = 1;
    vdp_tick();

    //Read V counter (Port: 7E)
    z80_n_ioreq = 0;
    z80_n_rd = 0;
    z80_address = 0x7E;
    vdp_tick();
    printf("V Counter is: 0x%02X\n", z80_data);
    z80_n_ioreq = 1;
    z80_n_rd = 1;
    vdp_tick();

    //Read control port (0xBF). Returns status register
    z80_n_ioreq = 0;
    z80_n_rd = 0;
    z80_address = 0xBF;
    vdp_tick();
    printf("Status is: 0x%02X\n", z80_data);
    z80_n_ioreq = 1;
    z80_n_rd = 1;
    vdp_tick();

    //Write control port (0xBF). Register write
    register_write(VDP_REG_NAME_TABLE_ADDR, 0); //Name table at 0x00. Next free address is 0x400
    register_write(VDP_REG_COLOR_TABLE_ADDR, 0x10); //Color table at 0x400. Next free address 0x440
    register_write(VDP_REG_PATTERN_GENERATOR_ADDR, 0x01); //Pattern generator table at 0x800.

    //Write control port (0xBF) and write CRAM (port 0xBE)
    cram_write(1);
    data_write(3 << 4); //Red
    data_write(3 << 2); //Green
    data_write(3 << 0); //Blue
    data_write(3 << 4); //Red
    data_write(3 << 2); //Green
    data_write(3 << 0); //Blue

    //Write control port (0xBF) and write Name table (VRAM @ 0x0000)
    vram_write(0x0000); //Set write address to 0x0000
    //32x24 characters
    for (int i = 0; i < (32*24); i++)
        data_write(i%256);

    //Write control port and write color table
    vram_write(0x0400); //Set write addr to 0x0400
    //Size is 0x40
    for (int i = 0; i < VDP_COLOR_TABLE_SIZE; i++)
        data_write(i % 3 + 2); //Use red-green-blue colors.

    //Write control port and write pattern generator table
    vram_write(0x800);
    //Pattern generator table is 8x256 bytes long.
    for (int i = 0; i < (256 * 8); i++)
        data_write(vdp_font[i]);

    // -----------------

    //Get the picture
    vdp_to_sdl(vdp_get_pixels(), 0, screen);
	//Wait for exit
	uint8_t running = 1;
	while (running){
		SDL_UpdateWindowSurface(window);
		SDL_Event evt;
		while (SDL_PollEvent(&evt)){
			if (evt.type == SDL_QUIT)
				running = 0;
		}
    	SDL_Delay(100);
	}

    cleanup();
    return 0;
}