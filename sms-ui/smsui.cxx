#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <SDL/SDL.h>


//FLTK and dialogs
#include <FL/Fl.H>
#include <FL/Fl_File_Chooser.H>
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
    const unsigned long ticks = GetTickCount(); \
    if ((ticks - last_update) > 100){ \
      Fl::check();  /*Refresh FLTK every .1 seconds*/ \
      dlg_z80->update_values(); \
      last_update = ticks; \
    } \
    if (!dlg_z80->windowDialog->shown()) \
        is_running = 0; \
}

void emu_log(const char* msg, int level){
    std::cerr << "[" << emu_logelvel_names[level][0] << "] " << msg << std::endl;
}

int emu_init(){
    emu_log("Hello!", EMU_LOG_INFO);
    //Init emulator modules
    vdp_init();
    z80_init(0, 0); ///<-- @note No SDSC callbacks

    //Load ROM
    const char* f_path = fl_file_chooser("Open ROM", "Mastersystem ROM (*.{sms,bin})", "", 1);
    const char* f_default = "zexdoc.sms";
    FILE* in_f = 0;
    if (f_path){
        in_f = fopen(f_path, "rb");
    }
    else{
        in_f = fopen("zexdoc.sms", "rb");
    }
    if (!in_f){
        emu_log("Unable to open ROM:", EMU_LOG_CRITICAL);
        emu_log(f_path ? f_path : f_default, EMU_LOG_CRITICAL);
        exit(-1);
    }
    uint8_t* read_buffer = (uint8_t*) malloc(ROM_MAX_SIZE);
    if (!read_buffer){
        emu_log("Unable to allocate read buffer.", EMU_LOG_CRITICAL);
        exit(-1);
    }
    memset(read_buffer, 0, ROM_MAX_SIZE);
    fread(read_buffer, 1, ROM_MAX_SIZE, in_f);
    rom_set_image(read_buffer, ROM_MAX_SIZE);
    free(read_buffer);
    fclose(in_f);
    emu_log("Rom loaded:", EMU_LOG_INFO);
    emu_log(f_path ? f_path : f_default, EMU_LOG_INFO);

    //Setup SDL
    ///@note Implement me
    SDL_Init(SDL_INIT_EVERYTHING);
}

void emu_cleanup(){
    SDL_Quit();
    emu_log("Bye!", EMU_LOG_INFO);
}

// --- Emulator ---



int main(int argc, char** argv){
    emu_init();
    //Create dialogs
    DialogZ80* dlg_z80         = new DialogZ80;
    DialogBreakpoints* dlg_brk = new DialogBreakpoints;

    //Show dialogs
    dlg_brk->windowDialog->show();
    dlg_z80->windowDialog->show();

    //Control variables
    uint8_t is_running = 1; //<-- When this becomes false, the app exits
    uint32_t is_clocked = 0; //<-- When this becames false, the execution is paused.
    unsigned long last_update = 0;

    //Provide the UI with relevant variables
    dlg_z80->set_running_ptr((uint32_t*) &is_clocked);
    dlg_z80->set_z80_ptr(z80dbg_get_z80());
    dlg_brk->set_breakpoint_table(z80dbg_get_breakpoints());

    unsigned int edge_count = 0;
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
            ++edge_count;
            --is_clocked;
            __UPDATE_FLTK;
        }
        // --- Just update the UI if not clocked ---
        {
            __UPDATE_FLTK;
            //Add a delay
            Fl::wait(0.1);
        }
    }
    //Cleanup
    delete dlg_z80;
    delete dlg_brk;
    emu_cleanup();
    return 0;
}
