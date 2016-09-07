#ifndef __BIOS_MENU_H
#define __BIOS_MENU_H

#include <stdint.h>

/* --- bm-state.c --- */
void bm_state_init();
void bm_state_tick();

/* --- bios-menu.c --- */
#define LEFT_MARGIN 2
#define TOP_MARGIN 2
#define MAX_OPTION_COUNT 18

#define KEY_NONE 0
#define KEY_UP   1
#define KEY_DOWN 2
#define KEY_LEFT 3
#define KEY_RIGH 4
#define KEY_1    5
#define KEY_2    6

uint8_t get_cursor();
void draw_cursor(int8_t index);
void redraw_cursor();
void set_cursor_limits(uint8_t min, uint8_t max);
uint8_t update_input();

/* --- system-info.c --- */
void show_sysinfo();


#endif
