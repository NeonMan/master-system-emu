#include "sms.h"

static __sfr __at (SMS_IO_CONTROL_PORT) io_control;
/*static __sfr __at (SMS_IO_AB_PORT)      io_ab;*/
static __sfr __at (SMS_IO_B_MISC_PORT)  io_misc;

uint8_t sms_region(){
    uint8_t region_val;
    
    /*Japanese SMS lacks a periferal controller   */
    /*so flipping th values on the io port should */
    /*not be detected on a JAP console.           */
    region_val = SMS_REGION_EXPORT;
    io_control = 0b11110101;
    if((io_misc & 0b11000000) != 0b11000000){
        region_val = SMS_REGION_JAPAN;
    }
    
    io_control = 0b01010101;
    if((io_misc & 0b11000000) != 0b00000000){
        region_val = SMS_REGION_JAPAN;
    }
    
    io_control = 0b00000000;
    
    return region_val;
}
