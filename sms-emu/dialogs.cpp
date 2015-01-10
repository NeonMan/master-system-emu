#include "dialogs.h"
#include <FL/fl_ask.H>
#include <FL/Fl_File_Chooser.H>
#include <stdint.h>

#include "DialogZ80.h"
#include "../z80/z80_externs.h"
#include "../z80/z80.h"

//Forward declaration of callbacks
void DLG_running_cb(Fl_Widget* widget, void* u);

///A file open dialog.
char* DLG_file_chooser(const char* msg, const char* pat, const char* fname, int relative){
    const char* msg_inner = msg ? msg : "Choose a file";
    const char* pat_inner = pat ? pat : "*.*";
    const char* fname_inner = fname ? fname : "";
    return fl_file_chooser(msg_inner, pat_inner, fname_inner, relative);
}

///Refreshes the GUI. Must be called frequently to prevent GUI lockups.
void DLG_update(){
    Fl::check();
}

void* DLG_z80(uint8_t* volatile running){
    DialogZ80* dlg = new DialogZ80();
    dlg->windowDialog->show();
    dlg->buttonRunning->callback(DLG_running_cb, running);
    return dlg;
}


// --- Callbacks ---

///Handles the 'light' running switch, an uint_8 to the running variable is passed via user pointer
void DLG_running_cb(Fl_Widget* widget, void* u){
    uint8_t* volatile running = (uint8_t*)u;
    *running = (uint8_t)((Fl_Light_Button*)widget)->value();
}
