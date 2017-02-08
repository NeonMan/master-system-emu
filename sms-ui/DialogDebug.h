#ifndef __DIALOG_DEBUG_H
#define __DIALOG_DEBUG_H

#include "_DialogDebug.h"
#include <string>
#include <cstdint>

class DialogDebug : public _DialogDebugger {

public:
    DialogDebug();
    virtual ~DialogDebug();
    void make_window();
    void update_values();
    void setClockCounter(uint64_t* p);

private:
    std::string last_cmd;
    uint64_t* volatile clock_counter_p;

    void onInputCommand(Fl_Input * o, void* v);
    void onFlagsChanged();
    void onRegistersChanged();
    void onAddBreakpoint();
    void onDebugStart();
};

#endif
