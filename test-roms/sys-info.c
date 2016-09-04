/**
 * @file  test-roms/sys-info.c
 * @brief BIOS program showing SMS information.
 */

#include "sms/console.h"
#include "sms/intv-dummy.h" /*<-- Ignore interrupts*/
#include <stdint.h>

#define READ_BUFFER_SIZE 128


static uint8_t read_buffer[READ_BUFFER_SIZE];

static void compiler(){
    con_put("Compiler: ");
    
#ifdef __SDCC
    con_put("SDCC\n");
#else
    con_put("Unknown\n");
#endif

}

static void crt_version(){
    con_put("    Crt0: ");
    con_put((void*)0x0050);
    con_put("\n");
}

static void dump_info(){
    compiler();
    crt_version();
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
