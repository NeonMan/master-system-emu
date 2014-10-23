#ifndef __SDSC_H
#define __SDSC_H

#include <stdint.h>

void sdsc_write(uint8_t b);
void sdsc_control(uint8_t b);
void sdsc_write_str(char* str);

#endif