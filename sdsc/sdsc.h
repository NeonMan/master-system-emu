#ifndef __SDSC_H
#define __SDSC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void sdsc_write(uint8_t b);
void sdsc_control(uint8_t b);
void sdsc_write_str(char* str);

#ifdef __cplusplus
}
#endif

#endif