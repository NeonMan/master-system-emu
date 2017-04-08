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

/**
* @file sms-emu.h
* @brief Master System Emulator.
*
*/
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