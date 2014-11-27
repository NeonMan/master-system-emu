#include <SDL/SDL.h>
#include <SDL/SDL_version.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "z80/fake_z80.h"

#include "vdp/vdp.h"
#include "vdp/vdp_constants.h"

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

int main(int argc, char** argv){
    SDL_version ver;
    SDL_Init(SDL_INIT_VIDEO);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    //SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    printf("SDL Rev: %s\n", SDL_GetRevision());
    SDL_GetVersion(&ver);
    printf("SDL Ver: %d.%d.%d\n", ver.major, ver.minor, ver.patch);

    SDL_Window* window = SDL_CreateWindow("VDP testing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 256 + 64, 192 + 64, 0);
    SDL_Surface* screen = SDL_GetWindowSurface(window);
    SDL_Surface* vdp_surf = SDL_CreateRGBSurface(0, VDP_WIDTH_PIXELS, VDP_HEIGHT_PIXELS, 32, 255 << 16, 255 << 8, 255, 255 << 24);
    SDL_FillRect(screen, 0, 128);
    for (int x = 0; x < 4; x++){
        for (int y = 0; y < 4; y++){
            SDL_Rect video_area; 
            video_area.x = 32 + ((256 / 4) * x);
            video_area.y = 32 + ((192 / 4) * y);
            video_area.w = 256/4; 
            video_area.h = 192/4;
            SDL_FillRect(screen, &video_area, vdp_tmscolors[(4*y) + x] );
        }
    }
    SDL_Rect dst_rect;
    dst_rect.w = VDP_WIDTH_PIXELS;
    dst_rect.h = VDP_HEIGHT_PIXELS;
    dst_rect.x = 0;
    dst_rect.y = 0;
    vdp_to_sdl(vdp_get_pixels(), 0, screen);
	//Wait for exit
	uint8_t running = 1;
	SDL_DisplayMode mode;
	SDL_GetCurrentDisplayMode(0, &mode);
	while (running){
		SDL_UpdateWindowSurface(window);
		SDL_Event evt;
		while (SDL_PollEvent(&evt)){
			if (evt.type == SDL_QUIT)
				running = 0;
		}
		if (mode.refresh_rate)
			SDL_Delay(1000 / mode.refresh_rate);
		else
			SDL_Delay(100);
	}
    SDL_FreeSurface(vdp_surf);
    SDL_Quit();
    return 0;
}