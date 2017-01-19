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

#include "sms_debug.h"
#include <z80/z80.h>
#include <z80/z80_macros.h>
#include <stdio.h>
#include "savestate/savestate.h"
#include <string.h>
#include <stdlib.h>

extern z80_t z80;

static int trim_path(const char* path) {
    //Search, right to left, the second slash.
    size_t len = strlen(path);
    int slash_count = 0;
    while ((slash_count < 2) && (len > 0)) {
        if ((path[len] == '/') || (path[len] == '\\')) {
            ++slash_count;
        }
        --len;
    }
    if (slash_count == 2) {
        return len + 2;
    }
    else {
        return len;
    }
}

#ifdef WIN32
#include <assert.h>
static void platform_assert(const char* exp, const char* file, unsigned int line) {
    assert(0);
}
#else
static void platform_assert(const char* exp, const char* file, unsigned int line) {
    exit(-1);
}
#endif

void sms_assert(const char* exp, const char* file, unsigned int line) {
    const char* trimmed_path = file + trim_path(file);
    //Show info
    fprintf(stderr, "\r\n\r\n");
    fprintf(stderr, "--- ASSERTION ERROR !!! ---\r\n");
    fprintf(stderr, "   [%s @%d] Assert failed: %s\r\n", trimmed_path, line, exp);
    fprintf(stderr, "   Opcode buffer: %02X %02X %02X %02X (%d)\r\n", z80.opcode[0], z80.opcode[1], z80.opcode[2], z80.opcode[3], z80.opcode_index);
    fprintf(stderr, "   Stage: %d\r\n", z80.stage);
    fprintf(stderr, "   PC: 0x%04X\r\n", z80.rPC);
    
    //Fixup crash savestate so it can be loaded directly.
    //In essence, set the state _just before_ it crashed.
    z80.rPC -= z80.opcode_index;
    z80.opcode_index = 0;
    z80.stage = Z80_STAGE_RESET;

    //Dump state
    FILE* f = fopen("CRASH_DUMP.sav", "wb");
    if (f) {
        fprintf(stderr, "   Dumping state to CRASH_DUMP.sav\r\n");
        ss_save(f, 0);
        fclose(f);
    }
    else {
        fprintf(stderr, "   Warning: Unable to create crash dump.\r\n");
    }
    //Some more info
    fprintf(stderr, "---------------------------\r\n");
    fprintf(stderr, "Please file a bug report with the crash dump file and the block above.\r\n");
    //Call inner assert
    platform_assert(exp, trimmed_path, line);
}
