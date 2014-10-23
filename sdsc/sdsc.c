#include "sdsc.h"
#include <stdint.h>
#include <stdio.h>

/**
 * Implements the SDSC debug console.
 */

//sdsc console control port 0xFC
void sdsc_control(uint8_t b){
    fprintf(stderr, "SDSC control: 0x%X\n", b);
}

///sdsc console data port 0xFD
void sdsc_write(uint8_t b){
    //Dumb console passthrough
    putc(b, stderr);
}

///Write a string to the sdsc console
void sdsc_write(char* str){
    while ((*str) != 0){
        sdsc_write(*str);
        str++;
    }
}