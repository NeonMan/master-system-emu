#ifndef __DIALOG_DEBUG_H
#define __DIALOG_DEBUG_H

#include "_DialogDebug.h"
#include <string>

class DialogDebugger : public _DialogDebugger {

public:
    DialogDebugger();
    virtual ~DialogDebugger();
    void make_window();
    void update_values();

private:
    std::string last_cmd;

    void onInputCommand(Fl_Input * o, void* v);
    void onFlagsChanged();
    void onRegistersChanged();
    void onAddBreakpoint();
};

#endif
