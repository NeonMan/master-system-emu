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
    
    con_put("01234567890123456789012345678901");
    /*Do nothing*/
    while(1){
        
    }
}