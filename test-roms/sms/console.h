#ifndef __CONSOLE_H
#define __CONSOLE_H

#include <stdint.h>

#define CON_LINE_WIDTH 31 /* <-- VDP can show 31 8x8 sprites per line. */
#define CON_LINE_COUNT 24 /* <-- And 24 lines on screen. */

void con_init();
void con_put(const char* str);
void con_putc(char c);
void con_gotoxy(uint8_t x, uint8_t y);
void con_relxy(int8_t x, int8_t y);
void con_clear();

#endif
