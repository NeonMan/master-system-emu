/** @file z80.h
 *  @brief The z80 emulation functions.
 */
#ifndef __Z80_H
#define __Z80_H

#define Z80_CLOCK_NTSC  (3579545) /*<-- PSG clock (pal NTSC version)*/
#define Z80_CLOCK_PAL (3546893) /*<-- PSG clock (pal PAL version)*/

#include "z80_externs.h"

#ifdef __cplusplus
extern "C" {
#endif

//Types

///z80 internal state structure
struct z80_s {
    //Single 8bit registers
    uint8_t rAF[4]; ///<-- Accumulator/Flags
    uint8_t rI;    ///<-- Interrupt register
    uint8_t rR;    ///<-- DRAM Refresh counter

    //8-bit Registers which can be paired into 16bit ones
    uint8_t rWZ[4];
    uint8_t rBC[4]; ///<-- General purpose registers
    uint8_t rDE[4]; ///<-- General purpose registers
    uint8_t rHL[4]; ///<-- Indirect addressing registers

    //16-bit registers
    uint16_t rIX; ///<-- X Index register
    uint16_t rIY; ///<-- Y Index register
    uint16_t rSP; ///<-- Stack pointer
    uint16_t rPC; ///<-- Program counter

    //Latches
    uint8_t data_latch; ///<-- When data bus is sampled, it is stored here.
    uint8_t iff[2]; ///<-- Interrupt flip/flops

    uint8_t opcode[4]; ///<-- Opcode bytes, an opcode is at most 4 bytes long.
    uint8_t opcode_index; ///<-- Current read opcode size.

    //Current stage
    uint8_t stage; ///<-- Current Z80 stage. Either M1, M2, M3 or RESET (first M1).

    //Each stages' state
    unsigned int m1_tick_count; ///<-- Counts the number of half cycles on the M1 stage.
    unsigned int m2_tick_count; ///<-- Counts the number of half-cycles on the M2 stage.
    unsigned int m3_tick_count; ///<-- Counts the number of half-cycles on the M3 stage.

    //Temporal memory read storage
    uint16_t read_address;   ///<-- Which address to read from
    uint8_t  read_buffer[2]; ///<-- Up to 16-bit reads
    uint8_t  read_index;     ///<-- How many bytes we have read
    uint8_t  read_is_io;     ///<-- True if read is IO instead of mem

    //Temporal memory write storage
    uint16_t write_address;   ///<-- Which address to write to
    uint8_t  write_buffer[2]; ///<-- Up to 16-bit writes
    uint8_t  write_index;     ///<-- How many bytes we have written
    uint8_t  write_is_io;     ///<-- True if write is IO instead of mem
};

///Executes a z80 half-cycle.
void z80_tick();

///Initializes the z80 state.
void z80_init(void(*data_f) (uint8_t), void(*ctrl_f) (uint8_t));

//Debug functions
///Returns a pointer to the current z80 struct.
struct z80_s* z80dbg_get_z80();
///Returns a pointer to the breakpoint table
uint8_t* z80dbg_get_breakpoints();
///Sets a breakpoint
void z80dbg_set_breakpoint(uint16_t address, uint8_t flags);

#ifdef __cplusplus
}
#endif

#endif
