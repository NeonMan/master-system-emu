#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <SDL/SDL.h>


//FLTK and dialogs
#include <FL/Fl.H>
#include "DialogZ80.h"
#include "DialogBreakpoint.h"

//Emulator includes
#include <io/io.h>
#include <peripherial/peripherial.h>
#include <psg/psg.h>
#include <ram/ram.h>
#include <rom/rom.h>
#include <sdsc/sdsc.h>
#include <vdp/vdp.h>
#include <z80/z80.h>
#include "sms-emu.h" /*<-- Refactor me!*/

void emu_log(const char* msg, int level){
    std::cerr << "[" << emu_logelvel_names[level][0] << "] " << msg << std::endl;
}

void emu_init(){
    emu_log("Hello!", EMU_LOG_INFO);
}

void emu_cleanup(){
    //SDL_Quit();
    emu_log("Bye!", EMU_LOG_INFO);
}


int main(int argc, char** argv){
    emu_init();
    //Create dialogs
    DialogZ80* z80         = new DialogZ80;
    DialogBreakpoints* brk = new DialogBreakpoints;

    //Show dialogs
    z80->windowDialog->show();
    //brk->windowDialog->show();

    volatile uint8_t is_running = 1; //<-- When this becomes false, the app exits
    volatile uint8_t is_clocked = 1; //<-- When this becames false, the execution is paused.

    //Provide the UI with relevant variables
    struct z80_s fake_z80;
    for (int i = 0; i < sizeof(struct z80_s); i++){
        *(((uint8_t*)&fake_z80) + i) = i & 0xFF;
    }

    z80->set_running_ptr((uint8_t*) &is_clocked);
    z80->set_z80_ptr(&fake_z80);

    int tick_count = 0;
    while (is_running){
        while (is_clocked && is_running){
            ++tick_count;
            if ((tick_count%20) == 0) std::cout << "Tick!" << std::endl;

            //Update UI
            z80->update_values();
            Fl::check();
            Fl::wait(0.1);
            if (!z80->windowDialog->shown())
                is_running = 0;
        }
        // --- Just update the UI if not clocked ---
        {
            //Update UI
            Fl::check();
            Fl::wait(0.1);
            if (!z80->windowDialog->shown())
                is_running = 0;
        }
    }
    //Cleanup
    delete z80;
    delete brk;
    emu_cleanup();
    return 0;
}