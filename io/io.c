// Copyright 2015 Juan Luis Álvarez Martínez
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http ://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file io.c
 * @brief Master System IO mapper.
 *
 * Emulates the behaviour of the SMS IO controller chip, accessible using 
 * Port 0x3E.
 */

#include "io.h"
#include "io_externs.h"
#include <stdint.h>

uint8_t io_stat = 0xFF;
uint8_t io_reg  = 0;    ///<-- Internal mapper register

void io_tick(){
    //If IO operation on IOmapper's port
    if ((z80_n_ioreq == 0) && ((z80_address & 0x00FF) == 0x3E)){
        //Write operation
        if (z80_n_wr == 0){
            io_reg = z80_data;
        }
        //Read operation
        else if (z80_n_rd == 0){
            z80_data = io_reg;
        }
    }
    //Update mapper stat
    uint8_t next_state = 0;
    //RAM/ROM bits are #MREQ dependant
    if (z80_n_mreq == 0)
        next_state |= io_reg & (IO_EXPANSION_SLOT | IO_CARTRIDGE_SLOT | IO_CARD_SLOT | IO_RAM | IO_BIOS);
    //Peripherial bits are #IOREQ dependant
    if (z80_n_ioreq == 0)
        next_state |= io_reg & IO_PERIPHERIAL;
    //UNK bits are presumed NC

    //Write to the exported byte
    io_stat = next_state;
}
