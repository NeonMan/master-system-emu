#ifndef __SMS_EMU_H
#define __SMS_EMU_H

#define EMU_LOG_CRITICAL 0
#define EMU_LOG_ERROR    1
#define EMU_LOG_WARNING  2
#define EMU_LOG_INFO     3
#define EMU_LOG_DEBUG0   4
#define EMU_LOG_DEBUG1   5
#define EMU_LOG_DEBUG2   6

const char* const emu_logelvel_names[8][2] = { 
    { "CRIT", "Critical error" }, 
    { " ERR", "Error" },
    { "WARN", "Warning" },
    { "INFO", "Info" },
    { "DBG0", "Debug 0" },
    { "DBG1", "Debug 1" },
    { "DBG2", "Debug 2" },
    { 0, 0 }
};

#endif