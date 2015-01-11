#include <stdint.h>
#include <stdlib.h>

#include <iostream>

#include <FL/Fl.H>
#include "DialogZ80.h"
#include "DialogBreakpoint.h"

int main(int argc, char** argv){
    //Create dialogs
    DialogZ80* z80         = new DialogZ80;
    DialogBreakpoints* brk = new DialogBreakpoints;

    //Show dialogs
    z80->windowDialog->show();
    brk->windowDialog->show();

    volatile uint8_t is_running = 1; //<-- When this becomes false, the app exits
    volatile uint8_t is_clocked = 1; //<-- When this becames false, the execution is paused.

    //Provide the UI with relevant variables
    z80->set_running_ptr((uint8_t*) &is_clocked);
    z80->set_z80_ptr(0);

    int tick_count = 0;
    while (is_running){
        while (is_clocked && is_running){
            ++tick_count;
            if ((tick_count%20) == 0) std::cout << "Tick!" << std::endl;

            //Update UI
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