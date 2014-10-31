/** @file z80.h
 *  @brief The z80 module functions.
 */
#ifndef __Z80_H
#define __Z80_h

#include "z80_externs.h"

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

    ///Current opcode
    uint8_t opcode[4]; //An opcode is at most 4 bytes long
    ///Current opcode byte
    uint8_t opcode_index;

    //Current stage
    uint8_t stage;

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

void z80_tick();
void z80_init(void(*data_f) (uint8_t), void(*ctrl_f) (uint8_t));

#endif