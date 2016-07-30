// Copyright 2015 Juan Luis Álvarez Martínez
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file sdsc/sdsc.c
 * @brief SDSC debug console implementation.
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
            fprintf(stdout, "SDSC: Suspend emulation... [!]\n");
            break;
        case 2: /* -- Clear console emulation*/
            fprintf(stdout, "SDSC: Clear screen.\n");
            fprintf(stdout, "----------------------------\n"); ///<-- @bug do a better (and portable) clear screen procedure
            break;
        case 3: /* -- Set attribute*/
            fprintf(stdout, "SDSC: Set attribute... [!]\n");
            break;
        case 4: /* -- Move cursor*/
            fprintf(stdout, "SDSC: Move cursor... [!]\n");
            break;
        default:
            fprintf(stdout, "SDSC: Unknown byte (0x%X)\n", b);
            break;
        }
    }
    else{ //Multi-byte commands

    }
}

///sdsc console data port 0xFD
void sdsc_write(uint8_t b){
    //Dumb console passthrough
    putc(b, stdout);
}
