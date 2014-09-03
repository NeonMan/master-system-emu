#ifndef __Z80_H
#define __Z80_h

#ifndef WITHOUT_LIB
#ifdef NDEBUG
#pragma comment(lib, "../Release/z80.lib")
#else
#pragma comment(lib, "../Debug/z80.lib")
#endif
#endif

#include "z80_externs.h"

void z80_tick();

#endif