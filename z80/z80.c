/** @file z80.c
 *  @brief z80 implementation.
 *
 *  Implementation of a z80 CPU, ideally cycle-perfect.
 *
 *  Known issues.
 *  @fixme HI-Impedance is not implemented, no busreq or busack.
 *  @fixme instruction decode/execute.
 */
#include "z80.h"
#include "z80_macros.h"
#include "z80_dasm.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "../sdsc/sdsc.h"


// ##################################################
// ###  Debug code! delete me!                    ###
// ##################################################

#include "../ram/ram.h"

uint8_t* dbg_ram;

// ##################################################


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

#define Z80_STAGE_RESET 0
#define Z80_STAGE_M1 1
#define Z80_STAGE_M2 2
#define Z80_STAGE_M3 3



///@bug Move this into the z80 struct.
unsigned int z80_m1_tick_count = 0; ///<-- Counts the number of half cycles on the M1 stage.
unsigned int z80_m2_tick_count = 0; ///<-- Counts the number of half-cycles on the M2 stage.
unsigned int z80_m3_tick_count = 0; ///<-- Counts the number of half-cycles on the M3 stage.
uint8_t z80_stage = Z80_STAGE_M1;

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

struct z80_s  z80;


// Forward declaration of functions
int z80_stage_m1();
int z80_stage_m2(uint8_t noexec);
int z80_stage_m3(uint8_t noexec);

///Parity LUT.
const uint8_t z80_parity_lut[256] = {
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1
};

void z80_dump_reg(){
    printf("General purpose registers\n");
    printf("   IR: 0x%X%r\n", Z80_I, Z80_R);
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
        for (int i = 0; (i < 4) || (i < z80.opcode_index); i++)
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
        for (int i = 0; (i < 2) || (i < z80.read_index); i++){
            printf(" 0x%X", z80.read_buffer[i]);
        }
        if (z80.read_index > 2)
            printf("[Overflow! Size: %d]\n", z80.read_index);
        else
            printf("[Size: %d]\n", z80.read_index);
    }
    printf("   Write addr: 0x%X %s\n", z80.write_address, z80.write_is_io ? "(IO)": "");
    printf("   Write buff: 0x%X 0x%X [Index: %d]\n", z80.write_buffer[0], z80.write_buffer[1], z80.write_index);
}

void z80_init(){
    //### Debug
    dbg_ram = ramdbg_get_mem();
    //###

    //Zero the z80 struct.
    memset(&z80, 0x00, sizeof(struct z80_s));
    //Set anything non-zero here
    z80.rSP = 0xFFFF; //<-- Stack pointer starts at 0xFFFF
}

// --- Register lookup tables ---
// The pointers (all array values) themselves declared here are constant.
// The pointed variable is not.

////Register pairs lookup table
uint16_t* const z80_rp[8] = { &Z80_BC, &Z80_DE, &Z80_HL, &Z80_SP, &Z80_BC, &Z80_DE, &Z80_HL, &Z80_SP };
////Register pairs lookup table (AF option)
uint16_t* const z80_rp2[8] = { &Z80_BC, &Z80_DE, &Z80_HL, &Z80_AF, &Z80_BC, &Z80_DE, &Z80_HL, &Z80_AF };
///Register lookup table
uint8_t* const z80_r[8] = { &Z80_B, &Z80_C, &Z80_D, &Z80_E, &Z80_H, &Z80_L, 0, &Z80_A};
///Condition Flag  mask lookup table (NZ,Z,NC,C,PO,PE,P,M)
uint8_t  const z80_cc[8]      = { Z80_FLAG_ZERO, Z80_FLAG_ZERO, Z80_FLAG_CARRY, Z80_FLAG_CARRY, Z80_FLAG_PARITY, Z80_FLAG_PARITY, Z80_FLAG_SIGN, Z80_FLAG_SIGN };
///Expected flag value (after mask) for the condition to be true
uint8_t  const z80_cc_stat[8] = {             0, Z80_FLAG_ZERO,              0, Z80_FLAG_CARRY,               0, Z80_FLAG_PARITY,             0, Z80_FLAG_SIGN };
///Interrupt mode lookup table
int8_t const z80_im[][2] = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 2 }, { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 2 } };

///@bug missing tables: alu, rot, bli


/**
* @brief clears the z80 state to prepare for a new opcode.
*/
void z80_reset_pipeline(){
#ifndef NDEBUG
    /**/
    char opcode_str[100];
    opcode_str[0] = 0;
    z80d_decode(z80.opcode, 100, opcode_str);
    fprintf(stderr, "Last Opcode: (nx PC:0x%04X) %s; 0x", Z80_PC, opcode_str);
    for (int i = 0; i < z80.opcode_index; i++)
        fprintf(stderr, "%02X", z80.opcode[i]);
    fprintf(stderr, "\n");
    /**/
#endif
    z80.opcode_index = 0;
    z80.write_index = 0;
    z80.read_index = 0;
    z80.read_is_io = 0;
    z80.write_is_io = 0;
    z80_m1_tick_count = 0;
    z80_m2_tick_count = 0;
    z80_m3_tick_count = 0;
}

/*
 Most information taken from z80.info

 Decoding Z80 instructions:
     http://www.z80.info/decoding.htm
*/
/**
* @brief decode/execute a Z80 opcode.
*
* Decode a z80 opcode, if the opcode is completly read, execute it.
* If a memory read/write is needed, signal it by returning the apropriate
* information.
*
* @return Signal wether we need a read/write/fetch or none.
*/
int z80_instruction_decode(){

    //Relevant sub-byte divisions, for each of the 4 bytes (max) in an opcode.
    const uint8_t x[4] = { z80.opcode[0] >> 6, z80.opcode[1] >> 6, z80.opcode[2] >> 6, z80.opcode[3] >> 6 };
    const uint8_t y[4] = { (z80.opcode[0] >> 3) & 0x7, (z80.opcode[1] >> 3) & 0x7, (z80.opcode[2] >> 3) & 0x7, (z80.opcode[3] >> 3) & 0x7 };
    const uint8_t z[4] = { z80.opcode[0] & 0x7, z80.opcode[1] & 0x7, z80.opcode[2] & 0x7, z80.opcode[3] & 0x7 };
    const uint8_t p[4] = { (z80.opcode[0] >> 4) & 0x3, (z80.opcode[1] >> 4) & 0x3, (z80.opcode[2] >> 4) & 0x3, (z80.opcode[3] >> 4) & 0x3 };
    const uint8_t q[4] = { z80.opcode[0] & (1 << 3), z80.opcode[1] & (1 << 3), z80.opcode[2] & (1 << 3), z80.opcode[3] & (1 << 3) };

    //Select by X/Z at the same time
    switch (z80.opcode_index){

    case 1: //First opcode byte
        //Select by X/Z
        switch (z80.opcode[0] & (Z80_OPCODE_X_MASK | Z80_OPCODE_Z_MASK)){
        case Z80_OPCODE_XZ(0, 0):
            //Select by y
            switch (y[0]){
            case 0:                                             /* NOP; Size: 1; Flags: None*/
                return Z80_STAGE_RESET;
            case 1:                                       /*EX AF, AFp; Size: 1; Flags: None*/
            {
                const uint16_t tmp_af = Z80_AF;
                Z80_AF = Z80_AFp;
                Z80_AFp = tmp_af;
                return Z80_STAGE_RESET;
            }
            case 2:                                           /*DJNZ d; Size: 2; Flags: None*/
                return Z80_STAGE_M1; //One extra byte
            case 3:                                             /*JR d; Size: 2; Flags: None*/ 
                return Z80_STAGE_M1; //One extra byte
            default: //(4,5,6,7)                  /* JR [C,NC,Z,NZ], e; Size: 2; Flags: None*/
                return Z80_STAGE_M1; // +1 Byte
            }

        case Z80_OPCODE_XZ(0, 1):
            if (!q[0])                                  /*LD rp[p], nn; Size: 3; Flags: None*/
                return Z80_STAGE_M1; //Needs two extra bytes
            else{                                        /*ADD HL,rp[p] ; Size: 1; Flags N,C*/ 
                const uint16_t old_hl = Z80_HL;
                Z80_HL = Z80_HL + *z80_rp[p[0]];
                //Clear N/Carry flag (bits 1,0)
                Z80_F = (Z80_F & (Z80_CLRFLAG_CARRY & Z80_CLRFLAG_ADD)) 
                    //Set carry flag (bit 0)
                    | Z80_SETFLAG_CARRY(old_hl, Z80_HL);
                return Z80_STAGE_RESET;
            }

        case Z80_OPCODE_XZ(0, 2):
            //Select by q
            if (!(q[0])){
                if (p[0] == 0) {                            /*LD (BC),A; Size:1; Flags: None*/
                    //If the write has not been performed, request it
                    if (z80.write_index == 0){
                        z80.write_address = Z80_BC;
                        z80.write_buffer[0] = Z80_A;
                        return Z80_STAGE_M3;
                    }
                    //Otherwise, reset pipeline
                    else{
                        assert(z80.write_index == 1);
                        return Z80_STAGE_RESET;
                    }
                }
                else if (p[0] == 1)                        /*LD (DE),A; Size: 1; Flags: None*/
                    assert(0); //unimplemented
                else if (p[0] == 2)                       /*LD (nn),HL; Size: 3; Flags: None*/
                    return Z80_STAGE_M1; //Needs two extra bytes
                else if (p[0] == 3)                        /*LD (nn),A; Size: 3; Flags: None*/
                    return Z80_STAGE_M1; //Needs two extra bytes
            }
            else{
                if (p[0] == 0){                            /*LD A,(BC); Size: 1; Flags: None*/
                    assert(0); //Unimplemented
                }
                else if (p[0] == 1){                       /*LD A,(DE); Size: 1; Flags: None*/
                    if (z80.read_index == 0){
                        z80.read_address = Z80_DE;
                        return Z80_STAGE_M2;
                    }
                    else{
                        Z80_A = z80.read_buffer[0];
                        return Z80_STAGE_RESET;
                    }
                }
                else if (p[0] == 2){                      /*LD HL,(nn); Size: 3; Flags: None*/
                    return Z80_STAGE_M1; //2 extra bytes
                }
                else if (p[0] == 3){                       /*LD A,(nn); Size: 3; Flags: None*/
                    return Z80_STAGE_M1; //Needs two extra bytes
                }
            }

        case Z80_OPCODE_XZ(0, 3):
            //Select by q
            if (!(q[0])){                                 /*INC(rp[p]); Size: 1; Flags: None*/
                ++(*(z80_rp[p[0]]));
                return Z80_STAGE_RESET;
            }
            else{                                         /*DEC(rp[p]); Size: 1; Flags: None*/
                --(*(z80_rp[p[0]]));
                return Z80_STAGE_RESET;
            }

        case Z80_OPCODE_XZ(0, 4):                     /*INC(r[y]); Size: 1; Flags: S,Z,H,V,N*/
        {
            const uint8_t old_r = *z80_r[y[0]];
            ++(*(z80_r[y[0]]));
            Z80_F = (Z80_F & (
                 Z80_CLRFLAG_SIGN & Z80_CLRFLAG_ZERO & Z80_CLRFLAG_HC
                 & Z80_CLRFLAG_PARITY & Z80_CLRFLAG_ADD)
                 )  //Clear S,Z,H,P,N (7,6,4,2,1) ***V0-
                 | Z80_SETFLAG_SIGN(*z80_r[y[0]])
                 | Z80_SETFLAG_ZERO(*z80_r[y[0]])
                 | Z80_SETFLAG_HC(old_r, *z80_r[y[0]])
                 | Z80_SETFLAG_OVERFLOW(old_r, *z80_r[y[0]]);
            return Z80_STAGE_RESET;
        }
        case Z80_OPCODE_XZ(0, 5):                      /*DEC(r[y]); Size:1; Flags: S,Z,H,P,N*/
        {
            const uint8_t old_r = *z80_r[y[0]];
            --(*(z80_r[y[0]]));
            Z80_F = (Z80_F & (
                Z80_CLRFLAG_SIGN & Z80_CLRFLAG_ZERO & Z80_CLRFLAG_HC
                & Z80_CLRFLAG_PARITY & Z80_CLRFLAG_ADD)
                )  //Clear S,Z,H,P,N (7,6,4,2,1) ***V0-
                | Z80_SETFLAG_SIGN(*z80_r[y[0]])
                | Z80_SETFLAG_ZERO(*z80_r[y[0]])
                | Z80_SETFLAG_HC(old_r, *z80_r[y[0]])
                | Z80_SETFLAG_OVERFLOW(*z80_r[y[0]], old_r);
            return Z80_STAGE_RESET;
        }
        case Z80_OPCODE_XZ(0, 6):                         /*LD(r[y],n); Size: 2; Flags: None*/
            return Z80_STAGE_M1; //Needs one extra byte

        case Z80_OPCODE_XZ(0, 7):
            switch (y[0]){
            case 0:                                            /*RLCA; Size: 1; Flags: H,N,C*/
                Z80_A = (Z80_A << 1) | (Z80_A & (1 << 7) ? 1 : 0);
                Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_ADD & Z80_CLRFLAG_CARRY))
                    | (Z80_F & 1 ? Z80_FLAG_CARRY : 0);
                return Z80_STAGE_RESET;

            case 1:                                            /*RRCA; Size: 1; Flags: H,N,C*/
                Z80_A = (Z80_A >> 1) | (Z80_A & (1 >> 7) ? (1 << 7) : 0);
                Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_ADD & Z80_CLRFLAG_CARRY))
                    | (Z80_F & (1 << 7) ? Z80_FLAG_CARRY : 0);
                return Z80_STAGE_RESET;

            case 2:                                             /*RLA; Size: 1; Flags: H,N,C*/
            {
                const uint8_t next_carry = Z80_A & (1 << 7);
                Z80_A = (Z80_A << 1) | (Z80_F & Z80_FLAG_CARRY ? 1 : 0);
                Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_ADD & Z80_CLRFLAG_CARRY))
                    | (next_carry ? Z80_FLAG_CARRY : 0);
                return Z80_STAGE_RESET;
            }
            case 3:                                             /*RRA; Size: 1; Flags: H,N,C*/
            {
                const uint8_t next_carry = Z80_A & (1);
                Z80_A = (Z80_A >> 1) | (Z80_F & Z80_FLAG_CARRY ? (1 << 7) : 0);
                Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_ADD & Z80_CLRFLAG_CARRY))
                    | (next_carry ? Z80_FLAG_CARRY : 0);
                return Z80_STAGE_RESET;
            }
            case 4:                                         /*DAA; Size: 1; Flags: S,Z,H,P,C*/
                assert(0); //Unimplemented
                return Z80_STAGE_RESET;

            case 5:                                               /*CPL; Size: 1; Flags: H,N*/
                Z80_A = ~Z80_A;
                Z80_F = Z80_F | Z80_FLAG_HC | Z80_FLAG_ADD;
                return Z80_STAGE_RESET;

            case 6:                                                 /*SCF; Size: 1; Flags: C*/
                Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_ADD)) | Z80_FLAG_CARRY;
                return Z80_STAGE_RESET;

            case 7:                                                 /*CCF; Size: 1; Flags: C*/
                Z80_F = (Z80_F ^ Z80_FLAG_CARRY) & Z80_CLRFLAG_ADD;
                return Z80_STAGE_RESET;
            }

        case Z80_OPCODE_XZ(1, 0):                       /* */
        case Z80_OPCODE_XZ(1, 1):                       /* */
        case Z80_OPCODE_XZ(1, 2):                       /* */
        case Z80_OPCODE_XZ(1, 3):                       /* */
        case Z80_OPCODE_XZ(1, 4):                       /* */
        case Z80_OPCODE_XZ(1, 5):                       /* */
        case Z80_OPCODE_XZ(1, 7):                       /*LD r[y],r[z]; Size: 1; Flags: None*/
            //Target can never be (HL) but source can
            if (z80_r[z[0]]){ //If source != (HL)
                *(z80_r[y[0]]) = *(z80_r[z[0]]);
                return Z80_STAGE_RESET;
            }
            else{ //Source is (HL)
                if (z80.read_index == 0){
                    z80.read_address = Z80_HL;
                    return Z80_STAGE_M2;
                }
                else{
                    *(z80_r[y[0]]) = z80.read_buffer[0];
                    return Z80_STAGE_RESET;
                }
            }
            

        case Z80_OPCODE_XZ(1, 6):
            if (y[0]==6){                                         /*HALT; Size: 1; Flags: None*/
                assert(0); /*Unimplemented*/ return Z80_STAGE_RESET;
            }
            else{                                       /*LD r[y], r[z]; Size:1; Flags: None*/
                //Source can never be (HL)
                const uint8_t value = *(z80_r[y[0]]);
                if (z80.write_index == 0){//Target is always (HL); z==6
                    z80.write_address = Z80_HL;
                    z80.write_buffer[0] = value;
                    return Z80_STAGE_M3;
                }
                else{
                    return Z80_STAGE_RESET;
                }
            }

        case Z80_OPCODE_XZ(2, 0):                            /* */
        case Z80_OPCODE_XZ(2, 1):                            /* */
        case Z80_OPCODE_XZ(2, 2):                            /* */
        case Z80_OPCODE_XZ(2, 3):                            /* */
        case Z80_OPCODE_XZ(2, 4):                            /* */
        case Z80_OPCODE_XZ(2, 5):                            /* */
        case Z80_OPCODE_XZ(2, 6):                            /* */
        case Z80_OPCODE_XZ(2, 7):                            /*alu r[z]; Size: 1; Flags: ALL*/
            //Select ALU operation by 'y'
            switch (y[0]){
            case Z80_ALUOP_XOR:                              /*XOR r[z]; Size: 1; Flags: All*/
            {
                if (z80_r[z[0]]){
                    Z80_A  = Z80_A ^ *(z80_r[z[0]]);
                }
                else{                                         /*XOR (HL); Size: 1; Flags:ALL*/
                    //Requires an extra read
                    if (z80.read_index == 0){
                        z80.read_address = Z80_HL;
                        return Z80_STAGE_M2;
                    }
                    else{
                        Z80_A = Z80_A ^ z80.read_buffer[0];
                    }
                }
                Z80_F = 0;
                Z80_F |= Z80_SETFLAG_SIGN(Z80_A);
                Z80_F |= Z80_SETFLAG_ZERO(Z80_A);
                Z80_F |= Z80_SETFLAG_PARITY(Z80_A);
                return Z80_STAGE_RESET;
            }
            default:
                assert(0); /*Unimplemented*/ return Z80_STAGE_RESET;
            }

        case Z80_OPCODE_XZ(3, 0):                          /*RET cc[y]; Size: 1; Flags: None*/
            assert(0); /*Unimplemented*/ return Z80_STAGE_RESET;

        case Z80_OPCODE_XZ(3, 1):
            if (!q[0]){                                   /*POP rp2[p]; Size: 1; Flags: None*/
                //Read stack
                if (z80.read_index == 0){
                    z80.read_address = Z80_SP;
                    return Z80_STAGE_M2;
                }
                else if (z80.read_index == 1){
                    ++z80.read_address;
                    return Z80_STAGE_M2;
                }
                //Update state
                else{
                    Z80_SP += 2;
                    *(z80_rp2[p[0]]) = *((uint16_t*)z80.read_buffer); ///<-- @bug Endianness!
                    return Z80_STAGE_RESET;
                }
                assert(0); /*Unimplemented*/ return Z80_STAGE_RESET;
            }
            else{
                switch (p[0]){
                case 0:                                          /*RET; Size: 1; Flags: None*/
                    //Read stack
                    if (z80.read_index == 0){
                        z80.read_address = Z80_SP;
                        return Z80_STAGE_M2;
                    }
                    else if (z80.read_index == 1){
                        ++z80.read_address;
                        return Z80_STAGE_M2;
                    }
                    else{
                        Z80_SP += 2;
                        Z80_PC = *((uint16_t*)z80.read_buffer); ///<-- @bug Endianness!
                        return Z80_STAGE_RESET;
                    }
                    assert(0); /*Unimplemented*/ return Z80_STAGE_RESET;
                case 1:                                          /*EXX; Size: 1; Flags: None*/
                {
                    const uint16_t old_bc = Z80_BC;
                    const uint16_t old_de = Z80_DE;
                    const uint16_t old_hl = Z80_HL;
                    Z80_BC = Z80_BCp;
                    Z80_DE = Z80_DEp;
                    Z80_HL = Z80_HLp;
                    Z80_BCp = old_bc;
                    Z80_DEp = old_de;
                    Z80_HLp = old_hl;
                    return Z80_STAGE_RESET;
                }
                case 2:                                      /*JP (HL); Size: 1; Flags: None*/
                    assert(0); /*unimplemented*/ return Z80_STAGE_RESET;
                case 3:                                     /*LD SP,HL; Size: 1; Flags: None*/
                    assert(0); /*unimplemented*/ return Z80_STAGE_RESET;
                }
            }

        case Z80_OPCODE_XZ(3, 2):                       /*JP cc[y], nn; Size: 3; Flags: None*/
            return Z80_STAGE_M1; //+2 bytes

        case Z80_OPCODE_XZ(3, 3):
            switch (y[0]){
            case 0:                                            /*JP nn; Size: 3; Flags: None*/
                return Z80_STAGE_M1; //+2 Bytes
            case 1:
                assert(0); /*unimplemented: 0xCD prefix*/ return Z80_STAGE_M1;
            case 2:                                         /*OUT n, A; Size: 2; Flags: None*/
                return Z80_STAGE_M1; //+1 Byte
            case 3:                                          /*IN A, n; Size: 2; Flags: None*/
                return Z80_STAGE_M1; //+1 Byte
            case 4:                                      /*EX (SP), HL; Size: 1; Flags: None*/
                assert(0); /*Unimplemented*/ return Z80_STAGE_RESET;
            case 5:                                        /*EX(DE,HL); Size: 1; Flags: None*/
            {
                const uint16_t old_de = Z80_DE;
                Z80_DE = Z80_HL;
                Z80_HL = old_de;
                return Z80_STAGE_RESET;
            }
            case 6:                                               /*DI; Size: 1; Flags: None*/
                z80.iff[0] = 0;
                z80.iff[1] = 0;
                return Z80_STAGE_RESET;
            case 7:                                               /*EI; Size: 1; Flags: None*/
                z80.iff[0] = 1;
                z80.iff[1] = 1;
                return Z80_STAGE_RESET;
            }
            
        case Z80_OPCODE_XZ(3, 4):                     /*CALL cc[y], nn; Size: 3; Flags: None*/
            return Z80_STAGE_M1; //+2 bytes

        case Z80_OPCODE_XZ(3, 5):
            if (!q[0]){ /*PUSH rp2[p]; Size: 1; Flags: None*/
                //Prepare a write if needed
                if (z80.write_index == 0){
                    z80.write_address = Z80_SP - 2;
                    *((uint16_t*)z80.write_buffer) = *(z80_rp2[p[0]]); ///<-- @bug Endianness
                    return Z80_STAGE_M3;
                }
                else if (z80.write_index == 1){
                    ++z80.write_address;
                    return Z80_STAGE_M3;
                }
                else{
                    Z80_SP -= 2;
                    return Z80_STAGE_RESET;
                }
            }
            else{
                switch (p[0]){
                case 0:                                      /*CALL nn; Size: 3; Flags: None*/
                    return Z80_STAGE_M1; //+2 Bytes
                case 1:
                    //DD prefix
                case 2:
                    //ED prefix
                case 3:
                    //FD prefix
                    return Z80_STAGE_M1;
                }
            }

        case Z80_OPCODE_XZ(3, 6):                /*alu + 8bit immediate; Size: 2; Flags: ALL*/
            return Z80_STAGE_M1; //+1 byte

        case Z80_OPCODE_XZ(3, 7):                            /*RST y*8; Size: 1; Flags: None*/
            assert(0); /*Unimplemented*/
            return Z80_STAGE_RESET;
        }
        assert(0); /*Should never get here */
        return Z80_STAGE_RESET;

    case 2: //Second opcode byte
        //Test prefixes
        switch (z80.opcode[0]){
        case 0xCB: // --- 0xCB prefixed opcodes
            switch (x[1]){
            case 0:                                     /*rotation r[z]; Size: 2; Flags: ?*/ 
                assert(0); /*Unimplemented*/ return Z80_STAGE_RESET;
            case 1:                                /*BIT y,r[z]; Size: 2; Flags: _S,Z,H,_P,N*/
                Z80_F = (Z80_F & (Z80_CLRFLAG_ZERO & Z80_CLRFLAG_ADD)); //Clear Z,N
                Z80_F = Z80_F | ((1 << y[1]) & (*z80_r[z[1]])) ? 0 : Z80_CLRFLAG_ZERO;
                Z80_F = Z80_F | Z80_FLAG_HC;
                return Z80_STAGE_RESET;
            case 2:                                     /*RES y,r[z]; Size: 2; Flags: None*/
                *(z80_r[z[1]]) = *(z80_r[z[1]]) & (~(1 << y[1]));
                return Z80_STAGE_RESET;
            case 3:                                     /*SET y,r[z]; Size: 2; Flags: None*/
                assert(0); /*Unimplemented*/ return Z80_STAGE_RESET;
            }
        case 0xDD: // --- 0xDD prefixed opcodes
            if (z80.opcode[1] == 0xDD){
                z80.opcode_index--; //0xDDDD = 0xDD
                return Z80_STAGE_M1;
            }
            else if (z80.opcode[1] == 0xFD){
                //0xFD replaces a previous 0xDD
                z80.opcode[0] = 0xFD;
                z80.opcode_index--;
                return Z80_STAGE_M1;
            }
            else if (z80.opcode[1] == 0xED){
                z80.opcode[0] = 0xED;
                z80.opcode_index--;
                return Z80_STAGE_M1;
            }
            //0xDD prefixed opcodes below
            assert(0);
            return Z80_STAGE_RESET;

        case 0xED: // --- 0xED prefixed opcodes
            //Select by X/Z
            switch (z80.opcode[1] & (Z80_OPCODE_X_MASK | Z80_OPCODE_Z_MASK)){
            case Z80_OPCODE_XZ(1, 0):
                if (y[1] != 6){                    /*IN r[y], (C); Size: 2; Flags: S,Z,H,P,N*/
                    assert(0);
                    return Z80_STAGE_RESET;
                }
                else{                                 /*IN F, (C); Size: 2; Flags: S,Z,H,P,N*/
                    assert(0);
                    return Z80_STAGE_RESET;
                }

            case Z80_OPCODE_XZ(1, 1):
                if (y[1] != 6){                        /*OUT r[y], (C); Size: 2; Flags: None*/
                    assert(0);
                    return Z80_STAGE_RESET;
                }
                else          {                           /*OUT F, (C); Size: 2; Flags: None*/
                    assert(0);
                    return Z80_STAGE_RESET;
                }

            case Z80_OPCODE_XZ(1, 2):
                if (!(q[1])){                              /*SBC HL,rp[p]; Size: 2; Flags: ?*/
                    assert(0);
                    return Z80_STAGE_RESET;
                }
                else        {                              /*ADC HL,rp[p]; Size: 2; Flags: ?*/
                    assert(0);
                    return Z80_STAGE_RESET;
                }

            case Z80_OPCODE_XZ(1, 3):
                if (!(q[1])){                          /*LD (nn),rp[p]; Size: 4; Flags: None*/
                    return Z80_STAGE_M1; //Needs two extra bytes
                }
                else        {                          /*LD rp[p],(nn); Size: 4; Flags: None*/
                    return Z80_STAGE_M1; //Needs two extra bytes
                }

            case Z80_OPCODE_XZ(1, 4):                             /*NEG; Size: 2; Flags: All*/
                assert(0);
                return Z80_STAGE_RESET;

            case Z80_OPCODE_XZ(1, 5):
                if (y[1] != 1) {                                /*RETN; Size: 2; Flags: None*/
                    assert(0);
                    return Z80_STAGE_RESET;
                }
                else           {                                /*RETI; Size: 2; Flags: None*/
                    assert(0);
                    return Z80_STAGE_RESET;
                }

            case Z80_OPCODE_XZ(1, 6):                      /*IM(im[y]); Size: 2; Flags: None*/
                z80.iff[0] = z80_im[y[1]][0];
                z80.iff[1] = z80_im[y[1]][1];
                return Z80_STAGE_RESET;

            case Z80_OPCODE_XZ(1, 7):
                /*Make this a switch!*/
                if (y[1] == 0){ assert(0); /*LD(I,A)*/ }
                else if (y[1] == 1){ assert(0); /*LD(R,A)*/ }
                else if (y[1] == 2){ assert(0); /*LD(A,I)*/ }
                else if (y[1] == 3){ assert(0); /*LD(A,R)*/ }
                else if (y[1] == 4){ assert(0); /*RRD*/ }
                else if (y[1] == 5){ assert(0); /*RLD*/ }
                else               { assert(0); /*NOP*/ }
                return Z80_STAGE_RESET;

            case Z80_OPCODE_XZ(2, 0): /* */
            case Z80_OPCODE_XZ(2, 1): /* */
            case Z80_OPCODE_XZ(2, 2): /* */
            case Z80_OPCODE_XZ(2, 3): /*BLOCK(y,z)*/
                switch (z80.opcode[1] & (Z80_OPCODE_Y_MASK | Z80_OPCODE_Z_MASK)){
                case Z80_OPCODE_YZ(4, 0):
                case Z80_OPCODE_YZ(4, 1):
                case Z80_OPCODE_YZ(4, 2):
                case Z80_OPCODE_YZ(4, 3):

                case Z80_OPCODE_YZ(5, 0):
                case Z80_OPCODE_YZ(5, 1):
                case Z80_OPCODE_YZ(5, 2):
                case Z80_OPCODE_YZ(5, 3):

                case Z80_OPCODE_YZ(6, 0):            /*LDIR; Size: 2; Flags: H,P,N (cleared)*/
                    //(DE) <-- (HL); --DE; --HL; --BC; BC? repeat : end;
                    //Test wether we have finished already
                    if (Z80_BC == 0)
                        return Z80_STAGE_RESET;
                    //Perform a read
                    else if (z80.read_index == 0){
                        z80.read_address = Z80_HL;
                        return Z80_STAGE_M2;
                    }
                    //Perform a write
                    else if (z80.write_index == 0){
                        z80.write_address = Z80_DE;
                        z80.write_buffer[0] = z80.read_buffer[0];
                        return Z80_STAGE_M3;
                    }
                    //Update registers and end
                    else{
                        --Z80_HL;
                        --Z80_DE;
                        --Z80_BC;
                        Z80_F = Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_PARITY & Z80_CLRFLAG_ADD);
                        if (Z80_BC){
                            Z80_PC -= 2; //Repeat this intruction
                            return Z80_STAGE_RESET;
                        }
                        else{ 
                            return Z80_STAGE_RESET;
                        }
                    }

                case Z80_OPCODE_YZ(6, 1):
                case Z80_OPCODE_YZ(6, 2):
                case Z80_OPCODE_YZ(6, 3):

                case Z80_OPCODE_YZ(7, 0):
                case Z80_OPCODE_YZ(7, 1):
                case Z80_OPCODE_YZ(7, 2):
                case Z80_OPCODE_YZ(7, 3):
                    assert(0);
                    return Z80_STAGE_RESET;

                default: /*NONI/NOP*/
                    return Z80_STAGE_RESET;
                }

            case Z80_OPCODE_XZ(2, 4):                      /* */
            case Z80_OPCODE_XZ(2, 5):                      /* */
            case Z80_OPCODE_XZ(2, 6):                      /* */
            case Z80_OPCODE_XZ(2, 7):                      /* */
            default:     /*x==0 || x==3*/                  /* NOP/NOPI; Size: 2; Flags: None*/
                return Z80_STAGE_RESET;
            }
        case 0xFD: // --- 0xFD prefixed opcodes
            if (z80.opcode[1] == 0xFD){
                z80.opcode_index--; //0xDDDD = 0xDD
                return Z80_STAGE_M1;
            }
            else if (z80.opcode[1] == 0xDD){
                //0xFD replaces a previous 0xDD
                z80.opcode[0] = 0xFD;
                z80.opcode_index--;
                return Z80_STAGE_M1;
            }
            else if (z80.opcode[1] == 0xED){
                z80.opcode[0] = 0xED;
                z80.opcode_index--;
                return Z80_STAGE_M1;
            }
            //0xFD prefixed opcodes below
            assert(0);
            return Z80_STAGE_RESET;

        default: // --- Unprefixed opcodes (byte 2)
            switch (z80.opcode[0] & (Z80_OPCODE_X_MASK | Z80_OPCODE_Z_MASK)){
            case Z80_OPCODE_XZ(0,0):
                switch (y[0]){
                case 3:                                           /*JR, e; Size: 2; Flags: None*/
                {
                    const int8_t pc_shift = *((int8_t*)&z80.opcode[1]); ///<-- @bug Endianness
                    const int32_t next_pc = Z80_PC + pc_shift; //Signed relative jump
                    Z80_PC = (next_pc & 0xFFFF);
                    return Z80_STAGE_RESET;
                }
                case 4:
                case 5:
                case 6:
                case 7: //(4,5,6,7)                  /* JR [C,NC,Z,NZ], e; Size: 2; Flags: None*/
                    //Test required flag
                    if ((Z80_F & z80_cc[y[0] - 4]) == (z80_cc_stat[y[0] - 4])){
                        const int8_t pc_shift = *((int8_t*)&z80.opcode[1]); ///<-- @bug Endianness
                        const int32_t next_pc = Z80_PC + pc_shift; //Signed relative jump
                        Z80_PC = (next_pc & 0xFFFF);
                    }
                    return Z80_STAGE_RESET;
                default:
                    assert(0); //Unimplemented
                    return Z80_STAGE_RESET;
                }
            case Z80_OPCODE_XZ(0, 1):
                if (!q[0]){                           /* LD rp[n], mm; Size: 3; Flags: None */
                    return Z80_STAGE_M1; //+1 byte
                }
                else{
                    assert(0); /*No opcode reaches this point*/ return Z80_STAGE_RESET;
                }

            case Z80_OPCODE_XZ(0, 6):                      /* LD r, i; Size: 2; Flags: None */
                *(z80_r[y[0]]) = z80.opcode[1];
                return Z80_STAGE_RESET;

            case Z80_OPCODE_XZ(3, 2):                   /* JP cc[y], nn; Size: 3; Flags: None*/
                return Z80_STAGE_M1; //+1 byte
            case Z80_OPCODE_XZ(3,3):
                switch (y[0]){
                case 0:                                       /* JP nn; Size: 3; Flags: None*/
                    return Z80_STAGE_M1; //+1 byte
                case 2:                                  /* OUT (n), A; Size: 2; Flags: None*/
                    //if no byte has been read, prepare a write
                    if (z80.write_index == 0){
                        const uint16_t port_addr = z80.opcode[1] + (((uint16_t)Z80_A) << 8);
                        z80.write_address = port_addr;
                        z80.write_buffer[0] = Z80_A;
                        z80.write_is_io = 1;
                        return Z80_STAGE_M3;
                    }
                    //Otherwise, reset.
                    else
                        return Z80_STAGE_RESET;
                default:
                    assert(0); /*Unimplemented*/
                    return Z80_STAGE_RESET;
                }
            case Z80_OPCODE_XZ(3, 4):                 /* CALL cc[y] nn; Size: 3; Flags: None*/
                return Z80_STAGE_M1; // +1 byte
            case Z80_OPCODE_XZ(3,5):
                if (q[0]){
                    if (p[0] == 0)                          /* CALL nn; Size: 3; Flags: None*/
                        return Z80_STAGE_M1; //+1 byte
                    else{
                        assert(0); //Unimplemented
                        return Z80_STAGE_RESET;
                    }
                }
                else{
                    assert(0); //Unimplemented
                    return Z80_STAGE_RESET;
                }
            case Z80_OPCODE_XZ(3, 6):            /*alu + 8bit immediate; Size: 2; Flags: ALL*/
                //Select ALU operation by 'y'
                switch (y[0]){
                case Z80_ALUOP_CP:                               /*CP n; Size: 2; Flags: All*/
                    Z80_F  = 0;
                    Z80_F |= Z80_FLAG_ADD; //Flag is set, always
                    Z80_F |= Z80_SETFLAG_SIGN(Z80_A - z80.opcode[1]);
                    Z80_F |= Z80_SETFLAG_ZERO(Z80_A - z80.opcode[1]);
                    Z80_F |= Z80_SETFLAG_HC(Z80_A, Z80_A - z80.opcode[1]);
                    Z80_F |= Z80_SETFLAG_OVERFLOW(Z80_A, Z80_A - z80.opcode[1]);
                    Z80_F |= Z80_SETFLAG_BORROW(Z80_A, Z80_A - z80.opcode[1]);
                    return Z80_STAGE_RESET;
                default:
                    assert(0); //Unimplemented
                    return Z80_STAGE_RESET;
                }
            default:
                assert(0); /*Unimplemented*/
                return Z80_STAGE_RESET;
            }
        }
        assert(0); /*Should never get here */ return Z80_STAGE_RESET;

    case 3: //Third opcode byte
        //Test prefixes
        if      ((z80.opcode[0] == 0xDD) && (z80.opcode[1] == 0xCB)){ assert(0); }
        else if ((z80.opcode[0] == 0xFD) && (z80.opcode[1] == 0xCB)){ assert(0); }
        else if (z80.opcode[0] == 0xDD){ assert(0); }
        else if (z80.opcode[0] == 0xFD){ assert(0); }
        else if (z80.opcode[0] == 0xED){ assert(0); }
        else if (z80.opcode[0] == 0xCB){ assert(0); }
        else{
            //3-byte. Unprefixed
            switch (z80.opcode[0] & (Z80_OPCODE_X_MASK | Z80_OPCODE_Z_MASK)){
            case Z80_OPCODE_XZ(0,1):
                if (!q[0]){                           /* LD rp[n], mm; Size: 3; Flags: None */
                    //mm is stored in the opcode's bytes 1,2.
                    const uint16_t immediate = *((uint16_t*)(z80.opcode + 1));
                    *z80_rp[p[0]] = immediate; ///<-- @bug Endianness!
                    return Z80_STAGE_RESET;
                }
                else{
                    assert(0); //Unimplemented stuff
                    return Z80_STAGE_RESET;
                }
            case Z80_OPCODE_XZ(3, 2):                  /* JP cc[y], nn; Size: 3; Flags: None*/
                //Test condition
                if ((Z80_F & (z80_cc[y[0]])) == (z80_cc_stat[y[0]])){
                    const uint16_t new_pc = *((uint16_t*)(z80.opcode + 1));
                    Z80_PC = new_pc; ///<-- @bug Endianness!
                    return Z80_STAGE_RESET;
                }
                else{
                    return Z80_STAGE_RESET;
                }
            case Z80_OPCODE_XZ(3,3):
                switch (y[0]){
                case 0:                                       /* JP nn; Size: 3; Flags: None*/
                {
                    ///New PC stored in opcode's last bytes. @bug Endianness.
                    const uint16_t new_pc = *((uint16_t*)(z80.opcode + 1));
                    Z80_PC = new_pc;
                    return Z80_STAGE_RESET;
                }
                default:
                    assert(0); //Unimplemented stuff
                    return Z80_STAGE_RESET;
                }
            case Z80_OPCODE_XZ(3, 4):                 /* CALL cc[y] nn; Size: 3; Flags: None*/
                //Check condition
                if ((Z80_F & z80_cc[y[0]]) == (z80_cc_stat[y[0]])){
                    //Push PC onto the stack
                    if (z80.write_index == 0){
                        //Push PC to the stack (M3 write of current PC)
                        *((uint16_t*)(z80.write_buffer)) = Z80_PC; ///<-- @bug Endianness!
                        z80.write_address = Z80_SP - 2;
                        return Z80_STAGE_M3;
                    }
                    else if (z80.write_index == 1){
                        //Write the second byte
                        ++z80.write_address;
                        return Z80_STAGE_M3;
                    }
                    else{
                        //Update SP
                        Z80_SP -= 2;
                        //Update PC
                        const uint16_t new_pc = *((uint16_t*)(z80.opcode + 1)); ///<-- @bug Endiannes!
                        Z80_PC = new_pc;
                        return Z80_STAGE_RESET;
                    }
                }
                else{
                    return Z80_STAGE_RESET;
                }

            case Z80_OPCODE_XZ(3,5):
                if (!q[0]){
                    assert(0); //unimplemented
                    return Z80_STAGE_RESET;
                }
                else{
                    if (p[0] == 0){                         /* CALL nn; Size: 3; Flags: None*/
                        if (z80.write_index == 0){
                            //Push PC to the stack (M3 write of current PC)
                            *((uint16_t*)(z80.write_buffer)) = Z80_PC; ///<-- @bug Endianness!
                            z80.write_address = Z80_SP - 2;
                            return Z80_STAGE_M3;
                        }
                        else if (z80.write_index == 1){
                            //Write the second byte
                            ++z80.write_address;
                            return Z80_STAGE_M3;
                        }
                        else{
                            //Update SP
                            Z80_SP -= 2;
                            //Update PC
                            const uint16_t new_pc = *((uint16_t*)(z80.opcode + 1)); ///<-- @bug Endiannes!
                            Z80_PC = new_pc;
                            return Z80_STAGE_RESET;
                        }
                        assert(0); //Will never get here
                    }
                }
            }
            assert(0); /*Unimplemented*/ return Z80_STAGE_RESET;
        }
        assert(0); /*Should never get here */ return Z80_STAGE_RESET;

    case 4: //Fourth opcode byte
        assert(0); /*Should never get here */ return Z80_STAGE_RESET;
    }
    //Opcode_index > 4
    assert(0); //Opcode overflow!
    return Z80_STAGE_RESET;
}

///Executes the M3 stage (Memory write).
int z80_stage_m3(uint8_t noexec){
    switch (z80_m3_tick_count){
    case 0:
        //1st rising edge
        //Write address to bus
        z80_address = z80.write_address;

        //Prepare for next tick
        ++z80_m3_tick_count;
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
            if ((z80.write_address & 0xFF) == 0xFD)
                sdsc_write(z80.write_buffer[z80.write_index]);
            else if ((z80.write_address & 0xFF) == 0xFC)
                sdsc_control(z80.write_buffer[z80.write_index]);
        }
        //Update index
        ++z80.write_index;

        //Prepare for next tick (If not an IO cycle, skip next 2 cases)
        if (!z80.write_is_io)
            z80_m3_tick_count += 3;
        else
            ++z80_m3_tick_count;
        return Z80_STAGE_M3; //<-- Stage not finished
    case 2:
        //Forced IO wait cycle
        //Do nothing

        //Prepare for next tick
        ++z80_m3_tick_count;
        return Z80_STAGE_M3; //<-- Stage not finished
    case 3:
        //Forced IO wait cycle
        //Do nothing

        //Prepare for next tick
        ++z80_m3_tick_count;
        return Z80_STAGE_M3; //<-- Stage not finished

    case 4:
        //2nd rising edge
        //nothing

        //Prepare for next tick
        ++z80_m3_tick_count;
        return Z80_STAGE_M3; //<-- Stage not finished
    case 5:
        //2nd falling edge
        //Sample #wait
        if (z80_n_wait == 0){
            z80_m3_tick_count -= 2;
        }
        else{
            //Push #Write down
            z80_n_wr = 0;
        }

        //Prepare for next tick
        ++z80_m3_tick_count;
        return Z80_STAGE_M3; //<-- Stage not finished
    case 6:
        //3rd rising edge
        //nothing

        //Prepare for next tick
        ++z80_m3_tick_count;
        return Z80_STAGE_M3; //<-- Stage not finished
    case 7:
        //3rd falling edge
        //#MREQ/#IOREQ up
        z80_n_mreq = 1;
        z80_n_ioreq = 1;
        //#WR up
        z80_n_wr = 1;

        //Prepare for another M3
        z80_m3_tick_count = 0;
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
    switch (z80_m2_tick_count){
    case 0:
        //1st rising edge
        //Write address to address bus.
        z80_address = z80.read_address;

        //Prepare for next tick
        ++z80_m2_tick_count;
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
            z80_m2_tick_count += 3;
        else
            ++z80_m2_tick_count;
        return Z80_STAGE_M2; //<-- Stage hasn't finished yet
    case 2:
        //Forced wait cycle for IO requests

        //Prepare for next tick
        ++z80_m2_tick_count;
        return Z80_STAGE_M2; //<-- Stage hasn't finished yet
    case 3:
        //Forced wait cycle for IO requests

        //Prepare for next tick
        ++z80_m2_tick_count;
        return Z80_STAGE_M2; //<-- Stage hasn't finished yet
    case 4:
        //2nd rising edge
        // Nothing

        //Prepare for next tick
        ++z80_m2_tick_count;
        return Z80_STAGE_M2; //<-- Stage hasn't finished yet
    case 5:
        //2nd falling edge
        //Sample #WAIT. If down, decrement tick in 2. force one wait if IO.
        if ((z80_n_wait == 0) || z80.read_is_io){
            z80_m2_tick_count -= 2;
            z80.read_is_io = 0;
        }

        //Prepare for next tick
        ++z80_m2_tick_count;
        return Z80_STAGE_M2; //<-- Stage hasn't finished yet
    case 6:
        //3rd rising edge
        // Nothing

        //Prepare for next tick
        ++z80_m2_tick_count;
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
        z80_m2_tick_count = 0;
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
    switch (z80_m1_tick_count){
    case 0:
        //On T1 up (first Rising edge)
        //    -M1 is pulled down
        z80_n_m1 = 0;
        //    -RFSH pulled up. @check
        z80_n_rfsh = 1;
        //    -R register is incremented (7-bit). 8th bit is kept
        Z80_R = (Z80_R & (1 << 7)) | ((Z80_R + 1) % (1 << 7));
        
        //Prepare for next tick
        ++z80_m1_tick_count;
        return Z80_STAGE_M1; //<-- Stage hasn't finished
    case 1:
        //On T1 dn (first falling edge) 
        //    -PC is written to the address bus
        z80_address = Z80_PC;
        //    -MREQ and RD are pulled down
        z80_n_mreq = 0;
        z80_n_rd = 0;

        //Prepare for next tick
        ++z80_m1_tick_count;
        return Z80_STAGE_M1; //<-- Stage hasn't finished
    case 2:
        //On T2 up
        //    <nothing>

        //Prepare for next tick
        ++z80_m1_tick_count;
        return Z80_STAGE_M1; //<-- Stage hasn't finished
    case 3:
        //On T2 dn
        //    - WAIT line is sampled. If down decrease the tick count (T2 up)
        if   (!z80_n_wait){      
            z80_m1_tick_count -= 2;
            return Z80_STAGE_M1; //<-- Prevent tick from increasing
        }

        //Prepare for next tick
        ++z80_m1_tick_count;
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
        ++z80_m1_tick_count;
        return Z80_STAGE_M1; //<-- Stage hasn't finished
    case 5:
        //On T3 dn
        //    -MREQ pulled down (Refresh cycle)
        z80_n_mreq = 0;
        //    -Load I to the high address bus @check
        Z80_ADDRH = Z80_I;

        //Prepare for next tick
        ++z80_m1_tick_count;
        return Z80_STAGE_M1; //<-- Stage hasn't finished
    case 6:
        //On T4 up
        //    <nothing>

        //Prepare for next tick
        ++z80_m1_tick_count;
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

        z80_m1_tick_count = 0; //We might want to do another fetch

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
    default:
        assert(0); //<-- Should never get here
    }
}
