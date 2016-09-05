
#include "sms/console.h"
#include "sms/intv-dummy.h" /*<-- Ignore interrupts*/
#include <stdint.h>

void delay_loop(){
    volatile uint16_t i;
    for(i=0;i<4000;i++){
        i = i;
    }
}

#define LEFT_MARGIN 2
#define TOP_MARGIN 2

void main(){
    con_init();
    con_gotoxy(1, TOP_MARGIN + 0);
    con_put("Menu BIOS for Master System");
    
    con_gotoxy(LEFT_MARGIN + 2, TOP_MARGIN + 2 + 0);
    con_put("Boot cartridge");
    con_gotoxy(LEFT_MARGIN + 2, TOP_MARGIN + 2 + 1);
    con_put("Boot card slot");
    con_gotoxy(LEFT_MARGIN + 2, TOP_MARGIN + 2 + 2);
    con_put("Boot expansion port");
    con_gotoxy(LEFT_MARGIN + 2, TOP_MARGIN + 2 + 3);
    con_put("System info");
    
    while(1){
        uint8_t i;
        for(i=0; i<4; i++){
            con_gotoxy(LEFT_MARGIN, TOP_MARGIN + i + 2);
            con_putc('>');
            delay_loop();
            con_relxy(-1, 0);
            con_putc(' ');
        }
    }
}