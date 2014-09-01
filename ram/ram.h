#ifndef __RAM_H
#define __RAM_H

#ifndef WITHOUT_LIB
#ifdef NDEBUG
#pragma comment(lib, "../Release/ram.lib")
#else
#pragma comment(lib, "../Debug/ram.lib")
#endif
#endif

void ram_tick();

#endif