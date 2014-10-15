/** @file z80.c
 *  @brief z80 implementation.
 *
 *  Implementation of a z80 CPU, ideally cycle-perfect.
 */
#include "z80.h"
#include <stdint.h>
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
uint8_t  z80_n_busreq = 1; ///<-- !Bus request (???)
uint8_t  z80_n_busack = 1; ///<-- !Bus-acknowledge (???)

#define Z80_STAGE_M1 0
#define Z80_STAGE_M2 1
#define Z80_STAGE_M3 2

unsigned int z80_tick_count = 0; ///<-- Counts the number of half cycles on each M stage.
uint8_t z80_stage = Z80_STAGE_M1;

struct {
    //Single 8bit registers
    uint8_t rA[2]; ///<-- Accumulator
    uint8_t rF[2]; ///<-- Flags
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
    uint8_t data_latch;

    ///Current opcode
    uint8_t opcode[4]; //An opcode is at most 4 bytes long
    ///Current opcode byte
    uint8_t opcode_index;
    ///Expected opcode size
    uint8_t opcode_size;

} z80 = {   { 0, 0 }, //A
            { 0, 0 }, //F
            0, //I
            0, //R
            { 0, 0, 0, 0 }, //WZ
            { 0, 0, 0, 0 }, //BC
            { 0, 0, 0, 0 }, //DE
            { 0, 0, 0, 0 }, //HL
            0, //IX
            0, //IY
            0xffff, //SP
            0, //PC
            0, //data_latch
            { 0, 0, 0, 0 }, //Opcode
            0,  //Opcode idx
            0   //opcode_size
            };

//Register macros. *BEWARE* some macros are endianness sensitive!
//Little endian (x86) is presumed unless noted otherwise.

//A (endianness insensitive)
#define Z80_A  z80.rA[0]
#define Z80_Ap z80.rA[1]

//F (endianness insensitive)
#define Z80_F  z80.rF[0]
#define Z80_Fp z80.rF[1]

//I (endianness insensitive)
#define Z80_I  z80.rI

//R (endianness insensitive)
#define Z80_R  z80.rR

//BC
#define Z80_B  z80.rBC[1]
#define Z80_C  z80.rBC[0]
#define Z80_Bp z80.rBC[3]
#define Z80_Cp z80.rBC[2]
#define Z80_BC  *((uint16_t*) z80.rBC)
#define Z80_BCp *((uint16_t*) z80.rBC + 2)

//DE
#define Z80_D  z80.rDE[1]
#define Z80_E  z80.rDE[0]
#define Z80_Dp z80.rDE[3]
#define Z80_Ep z80.rDE[2]
#define Z80_DE  *((uint16_t*) z80.rDE)
#define Z80_DEp *((uint16_t*) z80.rDE + 2)

//HL
#define Z80_H  z80.rHL[1]
#define Z80_L  z80.rHL[0]
#define Z80_Hp z80.rHL[3]
#define Z80_Lp z80.rHL[2]
#define Z80_HL  *((uint16_t*) z80.rHL)
#define Z80_HLp *((uint16_t*) z80.rHL + 2)

//WZ
#define Z80_W  z80.rWZ[1]
#define Z80_Z  z80.rWZ[0]
#define Z80_Wp z80.rWZ[3]
#define Z80_Zp z80.rWZ[2]
#define Z80_WZ  *((uint16_t*) z80.rWZ)
#define Z80_WZp *((uint16_t*) z80.rWZ + 2)

//IX (endianness insensitive)
#define Z80_IX z80.rIX
#define Z80_IY z80.rIY
#define Z80_SP z80.rSP
#define Z80_PC z80.rPC

/*
 Most information taken from z80.info

 Decoding Z80 instructions:
     http://www.z80.info/decoding.htm
*/


///Called when reaching an illegal or unimplemented state
void z80_panic(int stat, void* data){

}


///Executes the M1 stage (Instruction fetch)
__inline void z80_stage_m1(){
    switch (z80_tick_count){
    case 0:
        //On T1 up (first Rising edge)
        //    -M1 is pulled down
        z80_n_m1 = 0;
        //    -RFSH pulled up. @check
        z80_n_rfsh = 1;
        //    -R register is incremented @check
        ++Z80_R;
        break;
        
    case 1:
        //On T1 dn (first falling edge) 
        //    -PC is written to the address bus
        z80_address = Z80_PC;
        //    -MREQ and RD are pulled down
        z80_n_mreq = 0;
        z80_n_rd = 0;
        break;

    case 2:
        //On T2 up
        //    <nothing>
        break;

    case 3:
        //On T2 dn
        //    - WAIT line is sampled. If down decrease the tick count (T2 up)
        if   (z80_n_wait) break; //<-- Continue
        else{                    //<-- GoTo T2 up
            --z80_tick_count;
            return; //<-- Prevent tick from increasing
        }

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
        break;

    case 4:
        //On T3 dn
        //    -MREQ pulled down
        z80_n_mreq = 0;
        break;

    case 5:
        //On T4 up
        //    <nothing>
        break;

    case 6:
        //On T4 dn
        //    -MREQ pulled up
        z80_n_mreq = 1;
        break;
    default:
        z80_panic(Z80_STAGE_M1, 0);
    }
    //Increment tick count
    ++z80_tick_count;

    //Now that this M1 is finished we have to decide if:
    //   -Execute another M1 stage (multi-byte instructions)
    //   -Pass continue to M2
    //   -Discard data and start M1 again (NOP)
    //In essence, decode the instruction



}

/** Executes a z80 half-clock.
 * 
 */
void z80_tick(){

}