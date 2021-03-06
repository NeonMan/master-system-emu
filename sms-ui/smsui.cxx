// Copyright 2015 Juan Luis Álvarez Martínez
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <SDL2/SDL.h>
#include <cassert>
#include <ctime>


//FLTK and dialogs
#include <FL/Fl.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Native_File_Chooser.H>
#include "DialogDebug.hxx"

//Emulator includes
#include <io/io.h>
#include <peripheral/peripheral.h>
#include <psg/psg.h>
#include <ram/ram.h>
#include <rom/rom.h>
#include <sdsc/sdsc.h>
#include <vdp/vdp.h>
#include <z80/z80.h>
#include <savestate/savestate.h>
#include "sms-emu.h" /*<-- Refactor me!*/

#include "zed/zedeighty.hxx"

#ifdef _WIN32
#define quick_exit exit
#endif

//Control variables
uint8_t is_running = 1; //<-- When this becomes false, the app exits
uint64_t is_clocked = 0; //<-- When this becames false, the execution is paused.

//Global stuff
static SDL_Window *ui_window = 0;
static SDL_Renderer *ui_renderer = 0;

// --- Helper functions & macros ---

//Update UI
#define __UPDATE_FLTK \
{ \
    const unsigned long ticks = SDL_GetTicks(); \
    if ((ticks - last_update) > 100){ \
      Fl::check();  /*Refresh FLTK every .1 seconds*/ \
      dlg_debug->update_values();\
      last_update = ticks; \
    } \
    if (!dlg_debug->windowDialog->shown()) \
        is_running = 0; \
}

using namespace std;

void emu_log(const char* msg, int level){
    std::cerr << "[" << emu_logelvel_names[level][0] << "] " << msg << std::endl;
}

static void load_rom(const char* f_path) {

    const char* f_default = "zexdoc_sdsc.sms";
    FILE* in_f = 0;
    if (f_path) {
        in_f = fl_fopen(f_path, "rb");
    }
    else {
        in_f = fl_fopen("zexdoc.sms", "rb");
    }
    if (!in_f) {
        emu_log("Unable to open ROM:", EMU_LOG_CRITICAL);
        emu_log(f_path ? f_path : f_default, EMU_LOG_CRITICAL);
        quick_exit(-1);
    }
    uint8_t* read_buffer = (uint8_t*)malloc(ROM_MAX_SIZE);
    if (!read_buffer) {
        emu_log("Unable to allocate read buffer.", EMU_LOG_CRITICAL);
        quick_exit(-1);
    }
    memset(read_buffer, 0, ROM_MAX_SIZE);
    {
        char read_bytes_s[12];
        sprintf(read_bytes_s, "%d", (int)fread(read_buffer, 1, ROM_MAX_SIZE, in_f));
        emu_log("Rom loaded:", EMU_LOG_INFO);
        emu_log(f_path ? f_path : f_default, EMU_LOG_INFO);
        emu_log("Read bytes:", EMU_LOG_INFO);
        emu_log(read_bytes_s, EMU_LOG_INFO);
    }
    rom_set_image(read_buffer, ROM_MAX_SIZE);
    free(read_buffer);
    fclose(in_f);
}

static void restore_state(const char* f_path) {
    FILE* in_f = fl_fopen(f_path, "rb");
    if (ss_restore(in_f) == 0) {
        //All ok
    }
    else {
        exit(-2);
    }
    fclose(in_f);
}

int emu_init(){
    emu_log("Hello!", EMU_LOG_INFO);
    //Redirect stderr/stdout
    //*stderr = *fl_fopen("err.txt", "wb");
    //*stdout = *fl_fopen("out.txt", "wb");

    //Init emulator modules
    vdp_init();
    z80_init(sdsc_write, sdsc_control); 
    //Init RAM
    uint8_t* ram_ptr = (uint8_t*) ramdbg_get_mem();
    for (int i = 0; i < RAM_SIZE; i++)
        *ram_ptr = i & 0xFF;

    //Load ROM
    const char* f_path = NULL;
	Fl_File_Chooser fnfc(
        ".", 
        "Supported files (*.{sms,bin,sav})\tROM files (*.{sms,bin})\tSavestates (*.{sav})",
        Fl_File_Chooser::SINGLE,
        "Open ROM"
    );
	Fl::wait(0.5);
	fnfc.show();
	while (fnfc.shown()) {
		Fl::wait(0.1);
	}
    f_path = fnfc.value();
    if (f_path) {
        size_t path_len = strlen(f_path);
        const char* ext = f_path + path_len - 3;
        if (strncmp(ext, "sav", 3) == 0) {
            restore_state(f_path);
        }
        else {
            load_rom(f_path);
        }
    }
    else {
        exit(-1);
    }

    //Analyze the first slot
    zed::analyze((uint8_t*)romdbg_get_rom(), 0x0000, 1024 * 48 - 1);

    //Setup SDL
    ///@note Implement me
    SDL_Init(SDL_INIT_EVERYTHING);
    ui_window = SDL_CreateWindow("sms-ui", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 480, 320, 0);
    if (ui_window == nullptr) {
        std::cerr << "FSDL_CreateWindow error: " << SDL_GetError() << std::endl;
        return -2;
    }

    //Setup SDL Renderer
    //ui_renderer = SDL_CreateRenderer(ui_window, -1, SDL_RENDERER_ACCELERATED);
    ui_renderer = SDL_CreateRenderer(ui_window, -1, SDL_RENDERER_SOFTWARE);
    if (ui_window == nullptr) {
        std::cerr << "SDL_CreateRenderer error: " << SDL_GetError() << std::endl;
        return -3;
    }

    return 1;
}

void emu_cleanup(){
    if (ui_renderer)
        SDL_DestroyRenderer(ui_renderer);
    if(ui_window)
        SDL_DestroyWindow(ui_window);
    SDL_Quit();
    emu_log("Bye!", EMU_LOG_INFO);

    fclose(stderr);
    fclose(stdout);
}

// --- Breakpoint callbacks ---
void emu_mem_breakpoint_cb(uint16_t address, uint8_t read){
    fl_beep();
    fl_alert("%s MEMORY breakpoint at: 0x%X", (read ? "READ" : "WRITE"), address);
    is_clocked = 1;
}

void emu_io_breakpoint_cb(uint16_t address, uint8_t read){
    fl_beep();
    fl_alert("%s IO breakpoint at: 0x%02X (0x%04X)", (read ? "READ" : "WRITE"), address&0x00FF, address);
    is_clocked = 1;
}

void emu_pc_breakpoint_cb(uint16_t address){
    fl_beep();
    fl_alert("EXECUTION breakpoint at: 0x%X", address);
    is_clocked = 1;
}

// ----------------
// --- Emulator ---
// ----------------

static void ui_render() {
    SDL_SetRenderDrawColor(ui_renderer, 0, 0, 168, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(ui_renderer);
    SDL_RenderPresent(ui_renderer);
}

int main(int argc, char** argv){
    emu_init();
    //Create dialogs
    DialogDebug* dlg_debug  = new DialogDebug;

    //Show dialogs
    dlg_debug->make_window();
    dlg_debug->windowDialog->show();

    //Control variables
    unsigned long last_update = 0;

    //Set breakpoint callbacks
    z80dbg_set_io_breakpoint_cb(emu_io_breakpoint_cb);
    z80dbg_set_mem_breakpoint_cb(emu_mem_breakpoint_cb);
    z80dbg_set_pc_breakpoint_cb(emu_pc_breakpoint_cb);

    //Provide Debug UI with relevant variables
    dlg_debug->setClockCounter(&is_clocked);

    uint_fast8_t edge_count = 0;
    ui_render();
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
            if (vdp_frame_ready()) {
                ui_render();
            }
        }
        // --- Just update the UI if not clocked ---
        {
            __UPDATE_FLTK;
            //Add a delay
            Fl::wait(0.1);
        }
    }
    //Cleanup
    delete dlg_debug;
    emu_cleanup();
    return 0;
}
