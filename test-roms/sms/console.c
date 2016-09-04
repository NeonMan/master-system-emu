#include "console.h"
#include "vdp.h"
#include "bbc_micro_font.h"

#include <stdint.h>

#define FONT      bbc_font
#define FONT_SIZE BBC_FONT_SIZE

/* VDP init constants */
#define NameTableAddress ((uint16_t) 0x3800) /*<-- must be a multiple of $800; usually $3800; fills $700 bytes (unstretched) */
#define SpriteTableAddress ((uint16_t) 0x3f00) /*<-- must be a multiple of $100; usually $3f00; fills $100 bytes */
#define SpriteSet 0 /*<--  0 for sprites to use tiles 0-255, 1 for 256+ */


#define REGISTER_INIT_SIZE 18
const uint8_t vdp_init_regs[REGISTER_INIT_SIZE] = {
    0b00100100, 0x80, 
/*  |||||||`- Disable synch                        */
/*  ||||||`-- Enable extra height modes            */
/*  |||||`--- SMS mode instead of SG               */
/*  ||||`---- Shift sprites left 8 pixels          */
/*  |||`----- Enable line interrupts               */
/*  ||`------ Blank leftmost column for scrolling  */
/*  |`------- Fix top 2 rows during scrolling      */
/*  `-------- Fix right 8 columns during scrolling */
    0b10000000, 0x81,
/*  ||||| |`- Zoomed sprites -> 16x16 pixels              */
/*  ||||| `-- Doubled sprites -> 2 tiles per sprite, 8x16 */
/*  ||||`---- 30 row/240 line mode                        */
/*  |||`----- 28 row/224 line mode                        */
/*  ||`------ VBlank interrupts                           */
/*  |`------- Enable display                              */
/*  `-------- Must be set (VRAM size bit)                 */
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
const uint8_t vdp_init_palette[INIT_PALETTE_SIZE] = {
    0x00,0x30,0x0c,0x03,0x3c,0x33,0x0f,0x16,0x19,0x06,0x35,0x21,0x0d,0x37,0x23,0x3f,
    0x00,0x30,0x0c,0x03,0x3c,0x33,0x0f,0x16,0x19,0x06,0x35,0x21,0x0d,0x37,0x23,0x07
};

uint16_t vram_addr;

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
    uint8_t vram_h;
    uint8_t vram_l;
    
    vram_l = (vram_addr   ) & 0x00FF;
    vram_h = (vram_addr>>8) & 0x00FF;
    
    /* make unprintable characters '?' */
    if((c<0x20) || (c>0x7F)){
        c = '?';
    }
    
    /* Write the char */
    vdp_set_control(vram_l);
    vdp_set_control(vram_h | 0x78);
    
    c = c - 0x20; /*<-- Character array starts at 0x20*/
    vdp_set_data(c);
    vdp_set_data(1);
    
    /*Increment VRAM*/
    vram_addr = vram_addr + 2;
}

void con_put(const char* str){
    while(*str){
        con_putc(*str);
        ++str;
    }
}
