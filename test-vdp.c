#include <SDL/SDL.h>
#include <SDL/SDL_version.h>
#include <stdio.h>
#include "vdp/vdp.h"
#include "vdp/vdp_constants.h"

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
    SDL_UpdateWindowSurface(window);
    SDL_Delay(5000);
    SDL_Quit();
    return 0;
}