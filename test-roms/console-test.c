/**
 * @file  test-roms/vdp-test.c
 * @brief Sample VDP message.
 */

#include "sms/console.h"
#include "sms/intv-dummy.h" /*<-- Ignore interrupts*/
#include <stdint.h>

void main(){
    uint16_t i;
    con_init();
    
    con_put("      Hello SMS Console!!       ");
    con_put("                                ");
    
    /*Print the whole ASCII range*/
    for(i=0; i<128; i++){
        con_putc(i & 0x7F);
    }
    
    /*Do nothing*/
    while(1){
        
    }
}