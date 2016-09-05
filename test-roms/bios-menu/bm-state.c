#include <stdint.h>
#include <sms/console.h>
#include "bios-menu.h"

#define ON_ENTRY -1
#define ON_EXEC  0
#define ON_EXIT  1

#define STATE_INITIAL     0
#define STATE_MAIN_MENU   1

static uint8_t state_current;
static uint8_t state_next;

/* State implementations */
static uint8_t state_initial(int8_t mode){
    (void) mode;
    return STATE_MAIN_MENU;
}

static uint8_t state_main_menu(int8_t mode){
    (void) mode;
    return STATE_MAIN_MENU;
}

/* State-update functions */

static uint8_t call_state(uint8_t state_id, int8_t mode){
    switch(state_id){
        case STATE_INITIAL:
        return state_initial(mode);
        
        case STATE_MAIN_MENU:
        return state_main_menu(mode);
        
        default:
        return state_id;
    }
}

void bm_state_init(){
    state_current = STATE_INITIAL;
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