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

// --- Helper functions & macros ---

//Update UI, where X is a milisecond delta since last update
#define __UPDATE_FLTK \
{ \
    const DWORD ticks = GetTickCount(); \
    if ((ticks % 10) == 0) Fl::check();  /*Refresh FLTK every .1 seconds*/ \
    if ((ticks % 100) == 0) dlg_z80->update_values(); /*Refresh Values every second*/ \
    if (!dlg_z80->windowDialog->shown()) \
        is_running = 0; \
}

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

// --- Emulator ---
void emu_loop(){

}

int main(int argc, char** argv){
    emu_init();
    //Create dialogs
    DialogZ80* dlg_z80         = new DialogZ80;
    DialogBreakpoints* dlg_brk = new DialogBreakpoints;

    //Show dialogs
    
    dlg_z80->windowDialog->show();
    //brk->windowDialog->show();

    volatile uint8_t is_running = 1; //<-- When this becomes false, the app exits
    volatile uint8_t is_clocked = 1; //<-- When this becames false, the execution is paused.

    //Provide the UI with relevant variables
    dlg_z80->set_running_ptr((uint8_t*) &is_clocked);
    dlg_z80->set_z80_ptr(z80dbg_get_z80());

    int edge_count = 0;
    while (is_running){
        while (is_clocked && is_running){
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
                psg_io();
            }
            __UPDATE_FLTK;
            ++edge_count;
        }
        // --- Just update the UI if not clocked ---
        {
            __UPDATE_FLTK;
        }
    }
    //Cleanup
    delete dlg_z80;
    delete dlg_brk;
    emu_cleanup();
    return 0;
}
