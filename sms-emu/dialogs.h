#ifndef __DIALOGS_H
#define __DIALOGS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

char* DLG_file_chooser(const char* msg, const char* pat, const char* fname, int relative);
void  DLG_update();
void* DLG_z80(uint8_t* volatile running);

#ifdef __cplusplus
}
#endif

#endif