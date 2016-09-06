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

/* --- rom-tools.c --- */
#define ROM_BUFFER_SIZE      1024
#define ROM_CODE_BUFFER_SIZE 128

#define ROM_CARTRIDGE 0b10101000
#define ROM_CARD_SLOT 0b11001000
#define ROM_EXPANSION 0b01101000
#define ROM_BIOS      0b11100000
#define ROM_RAM       0b00000000 /*<-- No, is not a joke u_u */

void     rom_info(uint8_t rom_media);
void     rom_boot(uint8_t rom_media);
uint8_t* rom_get_buffer();

#endif
