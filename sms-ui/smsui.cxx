#include <stdint.h>
#include <stdlib.h>

#include <iostream>

#include <FL/Fl.H>
#include "DialogZ80.h"
#include "DialogBreakpoint.h"
#include <z80/z80.h>

int main(int argc, char** argv){
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
    return 0;
}