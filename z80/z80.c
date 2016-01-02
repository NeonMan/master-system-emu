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

/** @file z80.c
 *  @brief z80 implementation.
 *
 *  Implementation of a z80 CPU, ideally cycle-perfect.
 *
 *  Known issues.
 *  @note HI-Impedance is not implemented, no busreq or busack.
 *  @note instruction decode/execute.
 */

#include "z80.h"
#include "z80_macros.h"
#include "z80_dasm.h"
#include "z80_register_lut.h"
#include "z80_decoder.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

// #### For debug purposes only ####
#include "ram/ram.h"
#include "io/io_externs.h"
uint16_t dbg_last_sp = 0xFFFF;
// #################################

// Z80 Buses
uint8_t  z80_data;     ///<-- Data bus, 8 bit wide (Input/Output)
uint16_t z80_address;  ///<-- Address bus   (Output)

// Z80 output signals
uint8_t  z80_n_rd = 1;    ///<-- !Read enable  (Output)
uint8_t  z80_n_wr = 1;    ///<-- !Write enable (Output)
uint8_t  z80_n_ioreq = 1; ///<-- !IO request (Output)
uint8_t  z80_n_mreq = 1;  ///<-- !Memory request (Output)
uint8_t  z80_n_rfsh = 1;  ///<-- !DRAM refresh (Output)
uint8_t  z80_n_m1 = 1;    ///<-- !Instrunction-one (Output)

// Z80 input signals
uint8_t  z80_n_int = 1;   ///<-- !Interrup (Input)
uint8_t  z80_n_nmi = 1;   ///<-- !Non-maskable-interrupt (Input)
uint8_t  z80_n_reset = 1; ///<-- !Reset (Input)
uint8_t  z80_n_wait = 1;  ///<-- !Wait (Input)

// Z80 to-be-documented signals
uint8_t  z80_n_busreq = 1; ///<-- !Bus request (Input)
uint8_t  z80_n_busack = 1; ///<-- !Bus-acknowledge (Output)

//Sdsc data/control function pointers (decouple z80 from sdsc.h)
void(*z80_sdsc_data) (uint8_t) = 0; ///<-- Function pointer for SDSC data port.
void(*z80_sdsc_control) (uint8_t) = 0; ///<-- Function pointer for SDSC control port.

void(*z80_break_io) (uint16_t /*Addr*/, uint8_t /*Read*/) = 0; ///<-- Function pointer for IO breakpoints
void(*z80_break_mem) (uint16_t /*Addr*/, uint8_t /*Read*/) = 0; ///<-- Function pointer for mem breakpoints
void(*z80_break_pc) (uint16_t /*Addr*/) = 0; ///<-- Function pointer for PC breakpoints

///Instance of the z80 struct. Used by the z80 module internally
struct z80_s  z80;

///Breakpoint table. If an address is nonzero there may be a breakpoint.
uint8_t z80_breakpoints[Z80_ADDRESS_SIZE];

// Forward declaration of functions
int z80_stage_m1();
int z80_stage_m2(uint8_t noexec);
int z80_stage_m3(uint8_t noexec);

///Set IO breakpoint callback
void z80dbg_set_io_breakpoint_cb(void(*cb) (uint16_t /*Addr*/, uint8_t /*Read*/)){
    z80_break_io = cb;
}

///Set Mem breakpoint callback
void z80dbg_set_mem_breakpoint_cb(void(*cb) (uint16_t /*Addr*/, uint8_t /*Read*/)){
    z80_break_mem = cb;
}

///Set PC breakpoint callback
void z80dbg_set_pc_breakpoint_cb(void(*cb) (uint16_t /*Addr*/)){
    z80_break_pc = cb;
}

///Returns the pointer to the breakpoint table
uint8_t* z80dbg_get_breakpoints(){
    return z80_breakpoints;
}

///Sets a breakpoint
void z80dbg_set_breakpoint(uint16_t address, uint8_t flags){
    //Unless it is an 8-bit IO breakpoint, simply set the coresponding
    //byte on the breakpoint table.
    if ((flags & (Z80_BREAK_IO_RD | Z80_BREAK_IO_WR)) && ((flags & Z80_BREAK_IO_16B) == 0)){
        //8-bit IO breakpoint
        for (uint16_t i = 0x0000; i < 0xFF00; i += 0x100){
            z80_breakpoints[i | (address & 0xFF)] = flags;
        }
    }
    else{
        //Normal breakpoint
        z80_breakpoints[address] = flags;
    }

}

///Prints the z80 registers to stdout
void z80_dump_reg(){
    printf("General purpose registers\n");
    printf("   IR: 0x%X %Xr\n", Z80_I, Z80_R);
    printf("   AF: 0x%X  AF': 0x%X\n", Z80_AF, Z80_AFp);
    printf("   BC: 0x%X  BC': 0x%X\n", Z80_BC, Z80_BCp);
    printf("   DE: 0x%X  DE': 0x%X\n", Z80_DE, Z80_DEp);
    printf("   HL: 0x%X  HL': 0x%X\n", Z80_HL, Z80_HLp);
    printf("   IX: 0x%X\n", Z80_IX);
    printf("   IY: 0x%X\n", Z80_IY);
    printf("Control registers\n");
    printf("   SP: 0x%X\n", Z80_SP);
    printf("   PC: 0x%X\n", Z80_PC);
    printf("   iff: [0x%X, 0x%X]\n", z80.iff[0], z80.iff[1]);
    printf("Internal state (implementation state)\n");
    printf("   Data latch: 0x%X\n", z80.data_latch);
    printf("   Opcode:");
    {
        for (int i = 0; (i < 4) && (i < z80.opcode_index); i++)
            printf(" 0x%X", z80.opcode[i]);
        if (z80.opcode_index > 4)
            printf(" [Overflow! Size: %d]\n", z80.opcode_index);
        else
            printf(" [Size: %d]\n", z80.opcode_index + 1);
    }
    printf("   Stage: M%d\n", z80.stage);
    printf("   Read addr: 0x%X %s\n", z80.read_address, z80.read_is_io ? "(IO)" : "");
    printf("   Read buff:");
    {
        for (int i = 0; (i < 2) && (i < z80.read_index); i++){
            printf(" 0x%X", z80.read_buffer[i]);
        }
        if (z80.read_index > 2)
            printf("[Overflow! Size: %d]\n", z80.read_index);
        else
            printf("[Size: %d]\n", z80.read_index);
    }
    printf("   Write addr: 0x%X %s\n", z80.write_address, z80.write_is_io ? "(IO)" : "");
    printf("   Write buff: 0x%X 0x%X [Index: %d]\n", z80.write_buffer[0], z80.write_buffer[1], z80.write_index);
}

/**
 * @brief Dumps the stack contents to stderr.
 *
 * @param ram Pointer to the ram image.
 * @param sp  Stack pointer value (16bit).
 * @param base_addr Ram base address.
 * @param count Number of stack bytes to print, including SP. Always even, last bit ignored.
 * @param count_below number of bytes to read below SP. Always even, last bit ignored.
 */
void z80_dump_stack(void* ram, uint16_t sp, uint16_t base_addr, uint16_t count, uint16_t count_below){
    /**/
    if(!ram)
        return;
    count = count & 0xFFFE;
    count_below = count_below & 0xFFFE;
    fprintf(stderr, "Stack:\n");
    for (int i = -(int)count_below; i < count; i += 2){
        if (i == 0)
            fprintf(stderr, " SP--> ");
        else if (i>0)
            fprintf(stderr, " +%02d   ", i);
        else
            fprintf(stderr, "       ");
        fprintf(stderr, "0x%04X: 0x%02X%02X\n", sp + i, ((uint8_t*)ram)[(sp - base_addr + i + 1)%RAM_SIZE], ((uint8_t*)ram)[(sp - base_addr + i)%RAM_SIZE]);
    }
    /**/
}

///Initializes the z80 struct and SDSC function callbacks.
void z80_init(void(*data_f) (uint8_t), void(*ctrl_f) (uint8_t)){
    //Zero the z80 struct.
    memset(&z80, 0x00, sizeof(struct z80_s));
    //Zero the breakpoint table
    memset(z80_breakpoints, 0x00, Z80_ADDRESS_SIZE);
    //Set anything non-zero here
    z80.rSP = 0xFFFF; //<-- Stack pointer starts at 0xFFFF

    //Setup SDSC
    z80_sdsc_data = data_f;
    z80_sdsc_control = ctrl_f;
}

/**
* @brief clears the z80 state to prepare for a new opcode.
*/
void z80_reset_pipeline(){
#ifndef NDEBUG
    /**/
    char opcode_str[100];
    int disasm_size = 0;
    opcode_str[0] = 0;
    if (z80.opcode_index) //There must be something to feed the disasm
        disasm_size = z80d_decode(z80.opcode, 100, opcode_str);
    if (Z80_SP != dbg_last_sp){
        z80_dump_stack(ramdbg_get_mem(), Z80_SP, RAM_BASE_ADDRESS, 12, 4);
        dbg_last_sp = Z80_SP;
    }
    fprintf(stderr, "Last Opcode: (nx PC:0x%04X) %s; 0x", Z80_PC, opcode_str);
    for (int i = 0; i < z80.opcode_index; i++)
        fprintf(stderr, "%02X", z80.opcode[i]);
    fprintf(stderr, "\n");
    if (disasm_size != z80.opcode_index)
        fprintf(stderr, "Warning: Disasm opcode size mismatch! Dasm: %d; z80: %d\n", disasm_size, z80.opcode_index);
    fflush(stderr);
    /**/
#endif
    z80.opcode_index = 0;
    z80.write_index = 0;
    z80.read_index = 0;
    z80.read_is_io = 0;
    z80.write_is_io = 0;
    z80.m1_tick_count = 0;
    z80.m2_tick_count = 0;
    z80.m3_tick_count = 0;

    //Check for an execution (PC) breakpoint
    if (z80_break_pc) {
        if (z80_breakpoints[Z80_PC] & Z80_BREAK_PC){
            z80_break_pc(Z80_PC);
        }
    }
}

///Executes the M3 stage (Memory write).
int z80_stage_m3(uint8_t noexec){
    switch (z80.m3_tick_count){
    case 0:
        /* IO Write Breakpoints */
        if (z80_breakpoints[z80.write_address & 0x00FF]){
            if (z80.write_is_io && (z80_breakpoints[z80.write_address & 0x00FF] & Z80_BREAK_IO_WR))
                if (z80_break_io) z80_break_io(z80.write_address, 0);
        }
        /* Memory write breakpoints */
        if (z80_breakpoints[z80.write_address]){
            if ((!z80.write_is_io) && (z80_breakpoints[z80.write_address] & Z80_BREAK_WR))
                if (z80_break_mem) z80_break_mem(z80.write_address, 0);
        }
        /* ------------------------- */
        //1st rising edge
        //Write address to bus
        z80_address = z80.write_address;

        //Prepare for next tick
        ++(z80.m3_tick_count);
        return Z80_STAGE_M3; //<-- Stage not finished
    case 1:
        //1st falling edge
        //#MREQ/#IOREQ down
        if (z80.write_is_io)
            z80_n_ioreq = 0;
        else
            z80_n_mreq = 0;
        //Write data to data bus
        assert(z80.write_index < 2);
        z80_data = z80.write_buffer[z80.write_index];
        //If this is a SDSC write, covertly call its functions.
        if (z80.write_is_io){
            if ((z80.write_address & 0xFF) == 0xFD){
                if (z80_sdsc_data)
                    z80_sdsc_data(z80.write_buffer[z80.write_index]);
            }
            else if ((z80.write_address & 0xFF) == 0xFC){
                if (z80_sdsc_control)
                    z80_sdsc_control(z80.write_buffer[z80.write_index]);
            }
        }
        //Update index
        ++z80.write_index;

        //Prepare for next tick (If not an IO cycle, skip next 2 cases)
        if (!z80.write_is_io)
            z80.m3_tick_count += 3;
        else
            ++(z80.m3_tick_count);
        return Z80_STAGE_M3; //<-- Stage not finished
    case 2:
        //Forced IO wait cycle
        //Do nothing

        //Prepare for next tick
        ++(z80.m3_tick_count);
        return Z80_STAGE_M3; //<-- Stage not finished
    case 3:
        //Forced IO wait cycle
        //Do nothing

        //Prepare for next tick
        ++(z80.m3_tick_count);
        return Z80_STAGE_M3; //<-- Stage not finished

    case 4:
        //2nd rising edge
        //nothing

        //Prepare for next tick
        ++(z80.m3_tick_count);
        return Z80_STAGE_M3; //<-- Stage not finished
    case 5:
        //2nd falling edge
        //Sample #wait
        if (z80_n_wait == 0){
            z80.m3_tick_count -= 2;
        }
        else{
            //Push #Write down
            z80_n_wr = 0;
        }

        //Prepare for next tick
        ++(z80.m3_tick_count);
        return Z80_STAGE_M3; //<-- Stage not finished
    case 6:
        //3rd rising edge
        //nothing

        //Prepare for next tick
        ++(z80.m3_tick_count);
        return Z80_STAGE_M3; //<-- Stage not finished
    case 7:
        //3rd falling edge
        //#MREQ/#IOREQ up
        z80_n_mreq = 1;
        z80_n_ioreq = 1;
        //#WR up
        z80_n_wr = 1;

        //Prepare for another M3
        z80.m3_tick_count = 0;
        //Call instruction decoder, return whatever it wants
        if (noexec)
            return Z80_STAGE_RESET;
        else
            return z80_instruction_decode();
    }
    assert(0); //Should never get here.
    return Z80_STAGE_RESET;
}

///Executes the M2 stage (Memory read)
int z80_stage_m2(uint8_t noexec){
    switch (z80.m2_tick_count){
    case 0:
        /* IO Read Breakpoints */
        if (z80_breakpoints[z80.read_address & 0x00FF]){
            if (z80.read_is_io && (z80_breakpoints[z80.read_address & 0x00FF] & Z80_BREAK_IO_RD))
                if (z80_break_io) z80_break_io(z80.read_address, 1);
        }
        /* Memory read breakpoints */
        if (z80_breakpoints[z80.read_address]){
            if ((!z80.read_is_io) && (z80_breakpoints[z80.read_address] & Z80_BREAK_RD))
                if (z80_break_mem) z80_break_mem(z80.read_address, 1);
        }
        /* ------------------------- */
        //1st rising edge
        //Write address to address bus.
        z80_address = z80.read_address;

        //Prepare for next tick
        ++(z80.m2_tick_count);
        return Z80_STAGE_M2; //<-- Stage hasn't finished yet
    case 1:
        //1st falling edge
        // #MREQ/#IOREQ down
        if (z80.read_is_io)
            z80_n_ioreq = 0;
        else
            z80_n_mreq = 0;
        // #RD down
        z80_n_rd = 0;

        //Prepare for next tick, if not IO, skip next 2 cases
        if (z80.read_is_io)
            z80.m2_tick_count += 3;
        else
            ++(z80.m2_tick_count);
        return Z80_STAGE_M2; //<-- Stage hasn't finished yet
    case 2:
        //Forced wait cycle for IO requests

        //Prepare for next tick
        ++(z80.m2_tick_count);
        return Z80_STAGE_M2; //<-- Stage hasn't finished yet
    case 3:
        //Forced wait cycle for IO requests

        //Prepare for next tick
        ++(z80.m2_tick_count);
        return Z80_STAGE_M2; //<-- Stage hasn't finished yet
    case 4:
        //2nd rising edge
        // Nothing

        //Prepare for next tick
        ++(z80.m2_tick_count);
        return Z80_STAGE_M2; //<-- Stage hasn't finished yet
    case 5:
        //2nd falling edge
        //Sample #WAIT. If down, decrement tick in 2. force one wait if IO.
        if ((z80_n_wait == 0) || z80.read_is_io){
            z80.m2_tick_count -= 2;
            z80.read_is_io = 0;
        }

        //Prepare for next tick
        ++(z80.m2_tick_count);
        return Z80_STAGE_M2; //<-- Stage hasn't finished yet
    case 6:
        //3rd rising edge
        // Nothing

        //Prepare for next tick
        ++(z80.m2_tick_count);
        return Z80_STAGE_M2; //<-- Stage hasn't finished yet
    case 7:
        //3rd falling edge
        // Sample data bus
        assert(z80.read_index < 2); //<-- No more than one byte in buffer.
        z80.read_buffer[z80.read_index] = z80_data;
        ++z80.read_index;

        //Reset #RD, #MREQ, #IOREQ
        z80_n_mreq = 1;
        z80_n_ioreq = 1;
        z80_n_rd = 1;

        //Prepare for another M2 if needed.
        z80.m2_tick_count = 0;
        //Call instruction decoder, retun whatever it wants.
        if (noexec)
            return Z80_STAGE_RESET;
        else
            return z80_instruction_decode();
    }
    assert(0); //Shold never get here.
    return Z80_STAGE_RESET;
}

///Executes the M1 stage (Instruction fetch)
int z80_stage_m1(){
    switch (z80.m1_tick_count){
    case 0:
        //On T1 up (first Rising edge)
        //    -M1 is pulled down
        z80_n_m1 = 0;
        //    -RFSH pulled up. @check
        z80_n_rfsh = 1;
        //    -R register is incremented (7-bit). 8th bit is kept
        Z80_R = (Z80_R & (1 << 7)) | ((Z80_R + 1) % (1 << 7));
        
        //Prepare for next tick
        ++(z80.m1_tick_count);
        return Z80_STAGE_M1; //<-- Stage hasn't finished
    case 1:
        //On T1 dn (first falling edge) 
        //    -PC is written to the address bus
        z80_address = Z80_PC;
        //    -MREQ and RD are pulled down
        z80_n_mreq = 0;
        z80_n_rd = 0;

        //Prepare for next tick
        ++(z80.m1_tick_count);
        return Z80_STAGE_M1; //<-- Stage hasn't finished
    case 2:
        //On T2 up
        //    <nothing>

        //Prepare for next tick
        ++(z80.m1_tick_count);
        return Z80_STAGE_M1; //<-- Stage hasn't finished
    case 3:
        //On T2 dn
        //    - WAIT line is sampled. If down decrease the tick count (T2 up)
        if   (!z80_n_wait){      
            z80.m1_tick_count -= 2;
            return Z80_STAGE_M1; //<-- Prevent tick from increasing
        }

        //Prepare for next tick
        ++(z80.m1_tick_count);
        return Z80_STAGE_M1; //<-- Stage hasn't finished
    case 4:
        //On T3 up
        //    -DATA bus is sampled
        z80.data_latch = z80_data;
        //    -MREQ pulled up
        z80_n_mreq = 1;
        //    -RD pulled up
        z80_n_rd = 1;
        //    -M1 pulled up
        z80_n_m1 = 1;
        //    -RFSH pulled down
        z80_n_rfsh = 0;
        //    -ADDRESS bus is set to REFRESH address
        z80_address = Z80_R;

        //Prepare for next tick
        ++(z80.m1_tick_count);
        return Z80_STAGE_M1; //<-- Stage hasn't finished
    case 5:
        //On T3 dn
        //    -MREQ pulled down (Refresh cycle)
        z80_n_mreq = 0;
        //    -Load I to the high address bus @check
        Z80_ADDRH = Z80_I;

        //Prepare for next tick
        ++(z80.m1_tick_count);
        return Z80_STAGE_M1; //<-- Stage hasn't finished
    case 6:
        //On T4 up
        //    <nothing>

        //Prepare for next tick
        ++(z80.m1_tick_count);
        return Z80_STAGE_M1; //<-- Stage hasn't finished
    case 7:
        //On T4 dn
        //    -MREQ pulled up
        z80_n_mreq = 1;

        /// @bug Identify where on the M1 stage is the (partial) opcode latched
        /// here we are presuming it is done at the end of M1 stage.
        assert(z80.opcode_index < 4);
        z80.opcode[z80.opcode_index] = z80.data_latch;
        ++z80.opcode_index;
        ///@bug Where on the M1 stage is PC incremented? Assuming at the end.
        ++Z80_PC;

        z80.m1_tick_count = 0; //We might want to do another fetch

        //Now that this M1 is finished we have to decide if:
        //   -Execute another M1 stage (multi-byte instructions)
        //   -Pass continue to M2
        //   -Discard data and start M1 again (NOP)
        //In essence, decode the instruction
        return z80_instruction_decode();
    }
    assert(0); //Should never get here.
    return Z80_STAGE_RESET;
}

int z80_stage_refresh() {
    switch (z80.m1_tick_count) {
    case 0:
        //On T3 up
        //    -DATA bus is sampled
        z80.data_latch = z80_data;
        //    -MREQ pulled up
        z80_n_mreq = 1;
        //    -RD pulled up
        z80_n_rd = 1;
        //    -M1 pulled up
        z80_n_m1 = 1;
        //    -RFSH pulled down
        z80_n_rfsh = 0;
        //    -ADDRESS bus is set to REFRESH address
        z80_address = Z80_R;

        //Prepare for next tick
        ++(z80.m1_tick_count);
        return Z80_STAGE_REFRESH; //<-- Stage hasn't finished
    case 1:
        //On T3 dn
        //    -MREQ pulled down (Refresh cycle)
        z80_n_mreq = 0;
        //    -Load I to the high address bus @check
        Z80_ADDRH = Z80_I;

        //Prepare for next tick
        ++(z80.m1_tick_count);
        return Z80_STAGE_REFRESH; //<-- Stage hasn't finished
    case 2:
        //On T4 up
        //    <nothing>

        //Prepare for next tick
        ++(z80.m1_tick_count);
        return Z80_STAGE_REFRESH; //<-- Stage hasn't finished
    case 3:
        //On T4 dn
        //    -MREQ pulled up
        z80_n_mreq = 1;
        return Z80_STAGE_RESET;
    default:
        assert(0);
        return Z80_STAGE_RESET;
    }
}

/** Executes a z80 half-clock.
 * 
 */
void z80_tick(){
    switch (z80.stage){
    case Z80_STAGE_RESET:
        z80_reset_pipeline();
        ++(z80.stage);
        //Fall-through
    case Z80_STAGE_M1:
        z80.stage = z80_stage_m1();
        break;
    case Z80_STAGE_M2:
        z80.stage = z80_stage_m2(0);
        break;
    case Z80_STAGE_M3:
        z80.stage = z80_stage_m3(0);
        break;
    case Z80_STAGE_REFRESH:
        z80.stage = z80_stage_refresh();
        break;
    default:
        assert(0); //<-- Should never get here
    }
}

// ---  Debug functions ---
struct z80_s* z80dbg_get_z80(){
    return &z80;
}
