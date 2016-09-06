#include <stdint.h>
#include <sms/console.h>
#include "bios-menu.h"

#define ON_ENTRY -1
#define ON_EXEC  0
#define ON_EXIT  1

#define STATE_NULL            0
#define STATE_MAIN_MENU       1
#define STATE_BOOT_CARTRIDGE  2
#define STATE_BOOT_CARD_SLOT  3
#define STATE_BOOT_EXPANSION  4
#define STATE_SYSTEM_INFO     5
#define STATE_BOOT_GENERIC    6


#define STATE_INITIAL     STATE_MAIN_MENU

/* --- state variables --- */
static uint8_t state_current;
static uint8_t state_next;
static uint8_t boot_media;

/* --- Helper functions --- */
static uint8_t update_input_and_cursor(){
    uint8_t rv;
    rv = update_input();
    switch(rv){
        case KEY_UP:
        draw_cursor(get_cursor() - 1); break;
        case KEY_DOWN:
        draw_cursor(get_cursor() + 1); break;
        default:
        break;
    }
    return rv;
}


/* --- State implementations --- */
static uint8_t state_main_menu(int8_t mode){
    if (mode == ON_ENTRY){
        /* Draw main menu */
        con_clear();
        con_gotoxy(1, TOP_MARGIN + 0);
        con_put("BIOS menu for Master System");
        
        con_gotoxy(LEFT_MARGIN + 2, TOP_MARGIN + 2 + 0);
        con_put("Boot cartridge");
        con_gotoxy(LEFT_MARGIN + 2, TOP_MARGIN + 2 + 1);
        con_put("Boot card slot");
        con_gotoxy(LEFT_MARGIN + 2, TOP_MARGIN + 2 + 2);
        con_put("Boot expansion port");
        con_gotoxy(LEFT_MARGIN + 2, TOP_MARGIN + 2 + 3);
        con_put("System info");
        
        con_gotoxy(3, 23);
#ifdef COMMIT_ID
        con_put("Rev: " COMMIT_ID);
#else
        con_put("Rev: " __DATE__ " " __TIME__);
#endif
        
        set_cursor_limits(0,3);
        draw_cursor(0);
    }
    else if(mode == ON_EXIT){
        return STATE_MAIN_MENU;
    }
    else{
        uint8_t key;
        uint8_t cursor;
        /*Scan input, if button 1 is pressed and cursor is on*/
        /*the correct position, transition to a state, if not*/
        /*stay here.*/
        key = update_input_and_cursor();
        cursor = get_cursor();
        
        if((key == KEY_1) && (cursor == 0)){
            return STATE_BOOT_CARTRIDGE;
        }
        else if((key == KEY_1) && (cursor == 1)){
            return STATE_BOOT_CARD_SLOT;
        }
        else if((key == KEY_1) && (cursor == 2)){
            return STATE_BOOT_EXPANSION;
        }
        else if((key == KEY_1) && (cursor == 3)){
            return STATE_SYSTEM_INFO;
        }
        
    }
    return STATE_MAIN_MENU;
}

static uint8_t state_system_info(int8_t mode){
    if(mode == ON_ENTRY){
        /* Draw header */
        con_clear();
        con_gotoxy(1, TOP_MARGIN + 0);
        con_put("System info\n\n");
        
        /*Dump info*/
        show_sysinfo();
    }
    else if(mode == ON_EXIT){
        
    }
    else{
        uint8_t key;
        uint8_t cursor;

        key = update_input_and_cursor();
        cursor = get_cursor();
        
        /*If button 2 is pressed, go back to main menu*/
        if(key == KEY_2){
            return STATE_MAIN_MENU;
        }
    }
    return STATE_SYSTEM_INFO;
}

static uint8_t state_boot_cartridge(int8_t mode){
    if (mode == ON_ENTRY){
        /* Draw boot header */
        con_clear();
        con_gotoxy(1, TOP_MARGIN + 0);
        con_put("Boot cartridge");
        
        /*Set BOOT mode to CARTRIDGE SLOT*/
        boot_media = ROM_CARTRIDGE;
    }
    return STATE_BOOT_GENERIC;
}

static uint8_t state_boot_card_slot(int8_t mode){
    if (mode == ON_ENTRY){
        /* Draw card boot header */
        con_clear();
        con_gotoxy(1, TOP_MARGIN + 0);
        con_put("Boot card slot");
        
        /*Set BOOT mode to CARD SLOT*/
        boot_media = ROM_CARD_SLOT;
    }
    return STATE_BOOT_GENERIC;
}

static uint8_t state_boot_expansion(int8_t mode){
    if (mode == ON_ENTRY){
        /* Draw card boot header */
        con_clear();
        con_gotoxy(1, TOP_MARGIN + 0);
        con_put("Boot expansion port");
        
        /*Set BOOT mode to EXPANSION PORT*/
        boot_media = ROM_EXPANSION;
    }
    return STATE_BOOT_GENERIC;
}

static uint8_t state_boot_generic(int8_t mode){
    if (mode == ON_ENTRY){
        /* Draw boot options */
        con_gotoxy(LEFT_MARGIN + 2, TOP_MARGIN + 2 + 0);
        con_put("Boot");
        con_gotoxy(LEFT_MARGIN + 2, TOP_MARGIN + 2 + 1);
        con_put("Boot (without tests)");
        con_gotoxy(LEFT_MARGIN + 2, TOP_MARGIN + 2 + 2);
        con_put("ROM info");
        
        set_cursor_limits(0,2);
        draw_cursor(0);
    }
    else if(mode == ON_EXIT){
        
    }
    else{
        uint8_t key;
        uint8_t cursor;
        
        key = update_input_and_cursor();
        cursor = get_cursor();
        
        /*If button 2 is pressed, go back to main menu*/
        if(key == KEY_2){
            return STATE_MAIN_MENU;
        }
        else if(key == KEY_1){
            switch(cursor){
                case 0:
                case 1:
                rom_boot(boot_media); break;
                default:
                break;
            }
        }
    }
    return STATE_BOOT_GENERIC;
}

/* State-update functions */

static uint8_t call_state(uint8_t state_id, int8_t mode){
    switch(state_id){
        case STATE_MAIN_MENU:
        return state_main_menu(mode);
        case STATE_BOOT_CARD_SLOT:
        return state_boot_card_slot(mode);
        case STATE_BOOT_CARTRIDGE:
        return state_boot_cartridge(mode);
        case STATE_BOOT_EXPANSION:
        return state_boot_expansion(mode);
        case STATE_BOOT_GENERIC:
        return state_boot_generic(mode);
        case STATE_SYSTEM_INFO:
        return state_system_info(mode);
        
        default:
        return state_id;
    }
}

void bm_state_init(){
    state_current = STATE_NULL;
    state_next    = STATE_INITIAL;
}

void bm_state_tick(){
    if(state_current != state_next){
        call_state(state_current, ON_EXIT);
        call_state(state_next, ON_ENTRY);
        state_current = state_next;
    }
    state_next = call_state(state_current, ON_EXEC);
}