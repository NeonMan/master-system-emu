#include "sms.h"

static __sfr __at (SMS_IO_CONTROL_PORT) io_control;
/*static __sfr __at (SMS_IO_AB_PORT)      io_ab;*/
static __sfr __at (SMS_IO_B_MISC_PORT)  io_misc;

#define REGION_CHECK_LOOPS 16

uint8_t sms_region(){
    uint8_t region_val;
    uint8_t i;
    
    /*Japanese SMS lacks a periferal controller   */
    /*so flipping th values on the io port should */
    /*not be detected on a JAP console.           */
    region_val = SMS_REGION_EXPORT;
    for(i=0; i<REGION_CHECK_LOOPS; i++){
        io_control = 0b11111101;
        if((io_misc & 0b01000000) != 0b01000000){
            region_val = SMS_REGION_JAPAN;
        }
        
        io_control = 0b11011101;
        if((io_misc & 0b01000000) != 0b00000000){
            region_val = SMS_REGION_JAPAN;
        }
    }
    io_control = 0b11111111;
    
    return region_val;
}
