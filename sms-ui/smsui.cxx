#include <stdint.h>
#include <stdlib.h>

#include <iostream>

#include <FL/Fl.H>
#include "DialogZ80.h"
#include "DialogBreakpoint.h"

///Z80->Running callback
void clocked_callback(Fl_Widget* widget, void* u){
    uint8_t* volatile clocked = (uint8_t*)u;
    *clocked = (uint8_t)((Fl_Light_Button*)widget)->value();
}

int main(int argc, char** argv){
    //Create dialogs
    DialogZ80* z80         = new DialogZ80;
    DialogBreakpoints* brk = new DialogBreakpoints;

    //Show dialogs
    z80->windowDialog->show();
    brk->windowDialog->show();

    volatile uint8_t is_running = 1; //<-- When this becomes false, the app exits
    volatile uint8_t is_clocked = 1; //<-- When this becames false, the execution is paused.

    //Connect callbacks
    z80->buttonRunning->callback(clocked_callback, (void*) &is_clocked);

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