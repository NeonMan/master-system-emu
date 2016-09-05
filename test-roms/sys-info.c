/**
 * @file  test-roms/sys-info.c
 * @brief BIOS program showing SMS information.
 */

#include "sms/sms.h"
#include "sms/console.h"
#include "sms/intv-dummy.h" /*<-- Ignore interrupts*/
#include <stdint.h>

#define READ_BUFFER_SIZE 1024
#define CRT_SIGNATURE_ADDRESS 0x0050

static uint8_t read_buffer[READ_BUFFER_SIZE];

static void build_info(){
    /*Compiler*/
    con_put("Compiler: ");
#ifdef COMPILER_ID 
    con_put(COMPILER_ID "\n");
#else
    con_put("Unknown\n");
#endif

    /*Crt0 version*/
    {
        char crt_string[17];
        uint8_t i;
        for(i=0; i<16; i++){
            crt_string[i] = ((char*)CRT_SIGNATURE_ADDRESS)[i];
        }
        crt_string[16] = '\0';
        con_put("Crt0: ");
        con_put(crt_string);
        con_put("\n");
    }
    
    /*Date & Time*/
    con_put("Date: ");
    con_put(__DATE__ " " __TIME__);
    con_put("\n");
}

static void region(){
    con_put("Region: ");
    if(sms_region() == SMS_REGION_JAPAN){
        con_put("Japan");
    }
    else{
        con_put("Export");
    }
    con_put("\n");
}

static void dump_info(){
    con_put(" -- Build info --\n");
    build_info();
    con_put("\n -- Hardware info --\n");
    region();
    /*Do more stuff~ (ToDo)*/
}

static void init(){
    uint16_t i;
    /*Init vars and buffers*/
    for(i=0; i<READ_BUFFER_SIZE; i++){
        read_buffer[i] = 0;
    }
    
    /*Init console*/
    con_init();
}

void main(){
    /*Initialize*/
    init();
    
    /*Say Hello!*/
    con_put("SMS System info:\n\n");
    
    /*Dump info~*/
    dump_info();
    
    /*Say byte*/
    con_put("\n -- Info End -- ");

    /*Do nothing~*/
    while(1){
        
    }
}
