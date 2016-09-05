
#include "sms/sms.h"
#include "sms/console.h"
#include "sms/intv-dummy.h" /*<-- Ignore interrupts*/
#include <stdint.h>

void delay_loop(){
    volatile uint16_t i;
    for(i=0;i<1000;i++){
        i = i;
    }
}

#define LEFT_MARGIN 2
#define TOP_MARGIN 2
#define MAX_OPTION_COUNT 20

#define KEY_NONE 0
#define KEY_UP   1
#define KEY_DOWN 2
#define KEY_LEFT 3
#define KEY_RIGH 4
#define KEY_1    5
#define KEY_2    6

static __sfr __at  (SMS_IO_AB_PORT)     io_ab;
static __sfr __at  (SMS_IO_B_MISC_PORT) io_bm;

static int8_t  cursor_position;
static int8_t  cursor_min;
static int8_t  cursor_max;

static uint8_t last_key;

static void init(){
    con_init();
    cursor_position = 0;
    cursor_min = 0;
    cursor_max = MAX_OPTION_COUNT - 1;
    last_key    = KEY_NONE;
}

static void draw_cursor(int8_t index){
    uint8_t i;
    /*Fix index value to stay within limits*/
    if(index > cursor_max){
        index = cursor_min;
    }
    
    if(index < cursor_min){
        index = cursor_max;
    }
    cursor_position = index;
    
    /*Clear the cursor column*/
    for(i=0; i<MAX_OPTION_COUNT; i++){
        con_gotoxy(LEFT_MARGIN, TOP_MARGIN + i + 2);
        con_putc(' ');
    }
    
    /*Draw cursor*/
    con_gotoxy(LEFT_MARGIN, TOP_MARGIN + cursor_position + 2);
    con_putc('>');
}

static void increment_cursor(){
    draw_cursor(cursor_position + 1);
}

static void dump_input(){
    uint8_t ab;
    uint8_t bm;
    
    /*Draw input dev registers*/
    con_gotoxy(0,0);
    ab = io_ab;
    bm = io_bm;
    
    /*Pad one*/
    con_put(" 1:");
    (ab & (1<<0)) ? con_putc('-') : con_putc('U');
    (ab & (1<<1)) ? con_putc('-') : con_putc('D');
    (ab & (1<<2)) ? con_putc('-') : con_putc('L');
    (ab & (1<<3)) ? con_putc('-') : con_putc('R');
    (ab & (1<<4)) ? con_putc('-') : con_putc('1');
    (ab & (1<<5)) ? con_putc('-') : con_putc('2');
    
    /*Pad two*/
    con_put(" 2:");
    (ab & (1<<6)) ? con_putc('-') : con_putc('U');
    (ab & (1<<7)) ? con_putc('-') : con_putc('D');
    (bm & (1<<0)) ? con_putc('-') : con_putc('L');
    (bm & (1<<1)) ? con_putc('-') : con_putc('R');
    (bm & (1<<2)) ? con_putc('-') : con_putc('1');
    (bm & (1<<3)) ? con_putc('-') : con_putc('2');
    
    /*Misc*/
    con_put(" M:");
    (bm & (1<<4)) ? con_putc('-') : con_putc('R');
    (bm & (1<<5)) ? con_putc('-') : con_putc('C');
    (bm & (1<<6)) ? con_putc('-') : con_putc('A');
    (bm & (1<<7)) ? con_putc('-') : con_putc('B');
}

/*Returns the recently pressed key on change*/
static uint8_t update_input(){
    /*Scan pad 1 for input changes*/
    uint8_t ab;
    uint8_t current_key;
    ab = io_ab;
    current_key = KEY_NONE;
    if      (!(ab & (1<<0))){
        current_key = KEY_UP;
    }
    else if (!(ab & (1<<1))){
        current_key = KEY_DOWN;
    }
    else if (!(ab & (1<<2))){
        current_key = KEY_LEFT;
    }
    else if (!(ab & (1<<3))){
        current_key = KEY_RIGH;
    }
    else if (!(ab & (1<<4))){
        current_key = KEY_1;
    }
    else if (!(ab & (1<<5))){
        current_key = KEY_2;
    }
    
    if (current_key != last_key){
        last_key = current_key;
        return current_key;
    }
    else{
        return KEY_NONE;
    }
}

void main(){
    uint8_t i;
    init();
    
    /* Draw main menu */
    con_gotoxy(1, TOP_MARGIN + 0);
    con_put("Menu BIOS for Master System");
    for(i = 0; i<MAX_OPTION_COUNT; i++){
        con_gotoxy(LEFT_MARGIN + 2, TOP_MARGIN + 2 + i);
        switch(i){
            case 0:
            con_put("Boot cartridge"); break;
            case 1:
            con_put("Boot card slot"); break;
            case 2:
            con_put("Boot expansion port"); break;
            case 3:
            con_put("System info"); break;
            default:
            break;
        }
    }
    
    /*Draw cursor*/
    cursor_min = 0;
    cursor_max = 3;
    cursor_position = 0;
    draw_cursor(cursor_position);
    while(1){
        dump_input();
        switch(update_input()){
            case KEY_UP:
            draw_cursor(cursor_position - 1); break;
            case KEY_DOWN:
            draw_cursor(cursor_position + 1); break;
            default:
            break;
        }
        delay_loop();
    }
}