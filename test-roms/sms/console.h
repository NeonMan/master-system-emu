#ifndef __CONSOLE_H
#define __CONSOLE_H

#include <stdint.h>

void con_init();
void con_put(const char* str);
void con_putc(char c);
void con_gotoxy(uint8_t x, uint8_t y);
void con_clear();

#endif
