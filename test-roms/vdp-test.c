/**
 * @file  test-roms/vdp-test.c
 * @brief Sample VDP message.
 */

#include "sms/vdp.h"
#include "bbc_micro_font.h"
#include "sms/intv-dummy.h" /*<-- Ignore interrupts*/

void main(){
    volatile uint8_t c;
    uint8_t i,j;
    for(i=0; i<BBC_FONT_SIZE; i++){
        for(j=0; j<BBC_FONT_CHAR_SIZE; j++){
            c = bbc_font[i][j];
        }
    }
    /*Do nothing*/
    while(1){
        
    }
}