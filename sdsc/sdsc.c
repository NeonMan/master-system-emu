/** @file sdsc.c
*  @brief SDSC debug console implementation.
*
*/
#include "sdsc.h"
#include <stdint.h>
#include <stdio.h>

/**
 * Implements the SDSC debug console.
 */
#define SDSC_CTL_BUFFER_SIZE 10
uint8_t sdsc_cbuff[SDSC_CTL_BUFFER_SIZE];
uint8_t sdsc_cbuff_index = 0;

//sdsc console control port 0xFC
void sdsc_control(uint8_t b){
    if (sdsc_cbuff_index == 0){ //New command! No previous state exists.
        switch (b){
        case 1: /* -- Suspend emulation*/
            fprintf(stderr, "SDSC: Suspend emulation... [!]\n");
            break;
        case 2: /* -- Clear console emulation*/
            fprintf(stderr, "SDSC: Clear screen.\n");
            fprintf(stderr, "----------------------------\n"); ///<-- @bug do a better (and portable) clear screen procedure
            break;
        case 3: /* -- Set attribute*/
            fprintf(stderr, "SDSC: Set attribute... [!]\n");
            break;
        case 4: /* -- Move cursor*/
            fprintf(stderr, "SDSC: Move cursor... [!]\n");
            break;
        default:
            fprintf(stderr, "SDSC: Unknown byte (0x%X)\n", b);
            break;
        }
    }
    else{ //Multi-byte commands

    }
}

///sdsc console data port 0xFD
void sdsc_write(uint8_t b){
    //Dumb console passthrough
    putc(b, stderr);
}

///Write a string to the sdsc console
void sdsc_write_str(char* str){
    while ((*str) != 0){
        sdsc_write(*str);
        str++;
    }
}