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
