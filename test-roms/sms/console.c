#include "console.h"
#include "vdp.h"
#include "font8x8.h"

#include <stdint.h>

#define FONT      font8x8_basic
#define FONT_SIZE FONT8x8_SIZE

/* VDP init constants */
#define NameTableAddress ((uint16_t) 0x3800) /*<-- must be a multiple of $800; usually $3800; fills $700 bytes (unstretched) */
#define SpriteTableAddress ((uint16_t) 0x3f00) /*<-- must be a multiple of $100; usually $3f00; fills $100 bytes */
#define SpriteSet 0 /*<--  0 for sprites to use tiles 0-255, 1 for 256+ */


#define REGISTER_INIT_SIZE 18
static const uint8_t vdp_init_regs[REGISTER_INIT_SIZE] = {
    0b00100100, 0x80, 
/*    |||||||`- Disable synch                        */
/*    ||||||`-- Enable extra height modes            */
/*    |||||`--- SMS mode instead of SG               */
/*    ||||`---- Shift sprites left 8 pixels          */
/*    |||`----- Enable line interrupts               */
/*    ||`------ Blank leftmost column for scrolling  */
/*    |`------- Fix top 2 rows during scrolling      */
/*    `-------- Fix right 8 columns during scrolling */
    0b10000000, 0x81,
/*    ||||| |`- Zoomed sprites -> 16x16 pixels              */
/*    ||||| `-- Doubled sprites -> 2 tiles per sprite, 8x16 */
/*    ||||`---- 30 row/240 line mode                        */
/*    |||`----- 28 row/224 line mode                        */
/*    ||`------ VBlank interrupts                           */
/*    |`------- Enable display                              */
/*    `-------- Must be set (VRAM size bit)                 */
    ((NameTableAddress>>10)|0x01), 0x82, /*<-- (erq) OR data with $01 for 315-5124 compatibility */
    (SpriteTableAddress>>7), 0x85,
    (SpriteSet<<2), 0x86,
    0x00, 0x87,
/*    `-------- Border palette colour (sprite palette) */
    0x08, 0x88,
/*    ``------- Horizontal scroll */
    0x00, 0x89,
/*    ``------- Vertical scroll */
    0xff, 0x8a
/*    ``------- Line interrupt spacing */
};

#define INIT_PALETTE_SIZE 32
static const uint8_t vdp_init_palette[INIT_PALETTE_SIZE] = {
    0x00,0x30,0x0c,0x03,0x3c,0x33,0x0f,0x16,0x19,0x06,0x35,0x21,0x0d,0x37,0x23,0x3f,
    0x00,0x30,0x0c,0x03,0x3c,0x33,0x0f,0x16,0x19,0x06,0x35,0x21,0x0d,0x37,0x23,0x07
};

#define SCROLL_WRAP 28 /* <-- Memory line Wrap-around count*/

static uint8_t scroll_index;
static uint16_t vram_addr;
static uint8_t  cursor_x;
static uint8_t  cursor_y;

/* --- Inner functions, shall be made static eventually. --- */

static void draw_char(char c){
    uint8_t vram_h;
    uint8_t vram_l;
    
    vram_l = (vram_addr   ) & 0x00FF;
    vram_h = (vram_addr>>8) & 0x00FF;
    
    /* Make unprintable characters '?' */
    /* Extended ASCII are not avail either */
    if(c<0x20){
        c = '?';
    }
    
#ifndef EXTENDED_ASCII
    if(c>0x7F){
        c = '?';
    }
#endif
    
    /* Write the char */
    vdp_set_control(vram_l);
    vdp_set_control(vram_h | 0x78);
    
    c = c - 0x20; /*<-- Character array starts at 0x20*/
    vdp_set_data(c);
    vdp_set_data(1);
    
    /*Increment VRAM*/
    vram_addr = vram_addr + 2;
}

/**Pretares for a newline; Scrolls up all text by one line if needed.*/
static void new_line(){
    /* Update the vram_addr to point to the next line.*/
    vram_addr = vram_addr + (2 * (CON_LINE_WIDTH - cursor_x + 1));
    if(vram_addr > (2 * (SCROLL_WRAP - 1) * (CON_LINE_WIDTH + 1))){
        vram_addr = 0;
    }
    
    /*Reset cursor_x and increment cursor_y*/
    cursor_x = 0;
    cursor_y = cursor_y + 1;
    
    /* If cursor_y equals CON_LINE_COUNT, decrement, scroll the whole screen up */
    /* And update vram_addr accordingly. */
    if(cursor_y == CON_LINE_COUNT){
        uint8_t i;
        uint16_t new_vram_addr;
        /*Clear the next line with spaces*/
        new_vram_addr = vram_addr;
        for(i=0; i<CON_LINE_WIDTH; i++){
            /*
            uint16_t vram_l = (vram_addr   ) & 0x00FF;
            uint16_t vram_h = (vram_addr>>8) & 0x00FF;
            vdp_set_control(vram_l);
            vdp_set_control(vram_h | 0x78);
            vdp_set_data(0x00);
            vdp_set_data(0x01);
            vram_addr = vram_addr + 2;
            */
            draw_char(' ');
        }
        vram_addr = new_vram_addr;
        
        /*Calculate scroll position*/
        scroll_index = (scroll_index + 1) % SCROLL_WRAP;
        cursor_y = cursor_y - 1;
        
        /*send new scroll index to the VDP*/
        vdp_set_control(scroll_index << 3);
        vdp_set_control(0x89);
    }
}

static void wait_vblank(){
    /*
    uint8_t v_old;
    uint8_t v_new;
    
    do{
        v_old = vdp_get_v();
        do{
            v_new = vdp_get_v();
            
        }while(v_new != v_old);
    }while(v_new != 0xc0);
    */
}

/* --- Exported functions --- */

void con_clear(){
    uint8_t x;
    uint8_t y;
    uint16_t vram_addr_new;
    uint8_t  scroll_index_copy;
    /*Wait for vblank*/
    wait_vblank();
    
    /*Reset everything, fill screen with spaces*/
    scroll_index_copy = scroll_index;
    vram_addr_new = vram_addr + (scroll_index * (CON_LINE_WIDTH + 1) * 2);
    vram_addr = vram_addr_new;
    for(y = 0; y<CON_LINE_COUNT; y++){
        for(x = 0; x<CON_LINE_WIDTH; x++){
            con_putc(' ');
        }
    }
    vram_addr = vram_addr_new;
    scroll_index = scroll_index_copy;
    
    /*send old scroll index to the VDP*/
    vdp_set_control(scroll_index << 3);
    vdp_set_control(0x89);
}

void con_gotoxy(uint8_t x, uint8_t y){
    if(x >= CON_LINE_WIDTH){
        x = CON_LINE_WIDTH - 1;
    }
    
    if(y >= CON_LINE_COUNT){
        y = CON_LINE_COUNT - 1;
    }
    cursor_x = x;
    cursor_y = y;
    
    /*Calculate new vram_addr*/
    vram_addr = ((y * (CON_LINE_WIDTH + 1)) + (x)) * 2;
    /*Correct the scroll index*/
    vram_addr = vram_addr + (scroll_index * (CON_LINE_WIDTH + 1) * 2);
}

void con_relxy(int8_t x, int8_t y){
    con_gotoxy(cursor_x + x, cursor_y + y);
}

void con_init(){
    uint16_t i;
    /*Shameless ripoff of ZEXALL init code*/
    
    /*Setup registers*/
    for(i=0; i < REGISTER_INIT_SIZE; i++){
        vdp_set_control(vdp_init_regs[i]);
    }
    
    /*Clear VRAM*/
    for(i=0; i<0x4000; i++){
        vdp_set_data(0);
    }
    
    /*Set VRAM address*/
    vdp_set_control(0x00);
    vdp_set_control(0x60);
    
    /*Load font*/
    for(i=0; i<FONT_SIZE; i++){
        /*There are 8 bytes per char*/
        uint8_t c_byte;
        for(c_byte=0; c_byte<8; c_byte++){
            /*Byte must be written 4 times*/
            vdp_set_data(FONT[i][c_byte]);
            vdp_set_data(FONT[i][c_byte]);
            vdp_set_data(FONT[i][c_byte]);
            vdp_set_data(FONT[i][c_byte]);
        }
    }
    
    /*Set VRAM address*/
    vdp_set_control(0x00);
    vdp_set_control(0xc0);
    
    /*Load Palette*/
    for(i = 0; i<INIT_PALETTE_SIZE; i++){
        vdp_set_data(vdp_init_palette[i]);
    }
    
    /*Initialize state vars*/
    vram_addr = 0;
    cursor_x = 0;
    cursor_y = 0;
    scroll_index = 0;
    
    /*Turn screen on*/
    vdp_set_control(0b11000000);
/*                    ||||| |`- Zoomed sprites -> 16x16 pixels */
/*                    ||||| `-- Doubled sprites -> 2 tiles per sprite, 8x16 */
/*                    ||||`---- 30 row/240 line mode */
/*                    |||`----- 28 row/224 line mode */
/*                    ||`------ VBlank interrupts */
/*                    |`------- Enable display */
/*                    `-------- Must be set (VRAM size bit) */
    vdp_set_control(0x81);
}

void con_putc(char c){
    /* If character is '\n' or reach the screen's line end */
    /* we call new_line() */
    if(c == '\n'){
        new_line();
    }
    else{
        if(cursor_x == CON_LINE_WIDTH){
            new_line();
        }
        /* Draw the character*/
        draw_char(c);
        /* Increment cursor's X position*/
        cursor_x = cursor_x + 1;
    }
}

void con_put(const char* str){
    while(*str){
        con_putc(*str);
        ++str;
    }
}
