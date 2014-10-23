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
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "../sdsc/sdsc.h"

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

void z80_dump(){
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
    //Zero the z80 struct.
    memset(&z80, 0x00, sizeof(struct z80_s));
    //Set anything non-zero here
    z80.rSP = 0xFFFF; //<-- Stack pointer starts at 0xFFFF
}

struct z80_decoder_result_s{
    uint8_t request_fetch; ///<-- After reading a byte, the decoder requests more data
    uint8_t request_memory_read; ///<-- Decoded instruction requires a byte/word from memory
    uint8_t request_memory_write; ///<-- Decoded instruction requires a byte/word write
    uint16_t address; ///<-- Which address to read/write
    uint16_t data;    ///<-- Data to write
    uint8_t  is_16bit; ///<-- True on 16-bit read/write
};
typedef struct z80_decoder_result_s z80_decoder_result;

// --- Register lookup tables ---

////Register pairs lookup table
const uint16_t* z80_rp[8] = { &Z80_BC, &Z80_DE, &Z80_HL, &Z80_SP, &Z80_BCp, &Z80_DEp, &Z80_HLp, &Z80_SP };
////Register pairs lookup table (AF option)
const uint16_t* z80_rp2[8] = { &Z80_BC, &Z80_DE, &Z80_HL, &Z80_AF, &Z80_BCp, &Z80_DEp, &Z80_HLp, &Z80_AFp };
///Register lookup table
const uint8_t* z80_r[16] = { &Z80_B, &Z80_C, &Z80_D, &Z80_E, &Z80_H, &Z80_L, 0, &Z80_Ap, &Z80_Bp, &Z80_Cp, &Z80_Dp, &Z80_Ep, &Z80_Hp, &Z80_Lp, 0, &Z80_Ap };
///Condition Flag  mask lookup table (NZ,Z,NC,C,PO,PE,P,M)
///@bug cc is unimplemented (all zero)
const uint8_t z80_cc[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
///Interrupt mode lookup table
const int8_t z80_im[8] = { 0, -1, 1, 2, 0, -1, 1, 2 };

///@bug missing tables: alu, rot, bli


/**
* @brief clears the z80 state to prepare for a new opcode.
*/
void z80_reset_pipeline(){
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

    //We are going to use the method described in z80.info/decoding.html
    switch (z80.opcode_index - 1){

        //First opcode byte
    case 0:
        if (z80.opcode[0] == 0xCB){ /*Need another byte*/ } //0xCB prefixed opcodes
        else if (z80.opcode[0] == 0xED){ /*Need another byte*/ } //0xED prefixed opcodes
        else if (z80.opcode[0] == 0xDD){ /*Need another byte HL-->IX*/ } //0xDD prefixed opcodes
        else if (z80.opcode[0] == 0xFD){ /*Need another byte HL-->IY*/ } //0xFD prefixed opcodes
        else{                           //Unprefixed opcodes
            //Select by 'x' (2bit: 4 cases)
            switch (x[0]){
                // -------------
                // --- X = 0 ---
                // -------------
            case 0:
                //Select by 'z' (3bit: 8 cases)
                switch (z[0]){
                case 0:
                    //Select by 'y'
                    if (z[0] == 0) { /*NOP*/
                        //No flags were harmed
                        return Z80_STAGE_RESET;
                    }
                    else if (z[0] == 1){  /*EX (AF, AFp)*/
                        const uint16_t tmp_af = Z80_AF;
                        Z80_AF = Z80_AFp;
                        Z80_AFp = tmp_af;
                        //No flags were harmed
                        return Z80_STAGE_RESET;
                    }
                    else if (z[0] == 2) assert(0); /*DJNZ (d)*/    //Needs one extra byte
                    else if (z[0] == 3) assert(0); /*JR(d)*/       //Needs one extra byte
                    else                assert(0); /*JR (y-4, d)*/ //Needs one extra byte
                    break;
                case 1:
                    //Select by q 
                    if (q[0]){ assert(0); /*LD (rp[p], nn)*/ } //Needs two extra bytes
                    else     { /*ADD(HL,rp[p])*/ 
                        const uint16_t old_hl = Z80_HL;
                        Z80_HL = Z80_HL + *z80_rp[p[0]];
                        Z80_F = (Z80_F & (Z80_CLRFLAG_CARRY & Z80_CLRFLAG_ADD)) //Clear N/Carry flag (bits 1,0)
                            | Z80_SETFLAG_CARRY(old_hl, Z80_HL); //Set carry flag (bit 0)
                        return Z80_STAGE_RESET;
                    }
                    break;
                case 2:
                    //Select by q
                    if (!(q[0])){
                        if (p[0] == 0) {/*LD((BC),A)*/ 
                            //Write the A register into the BC address
                            
                            //If the write has not been performed, request it
                            if (z80.write_index == 0){
                                z80.write_address = Z80_BC;
                                z80.write_buffer[0] = Z80_A;
                                z80.write_index = 0;
                                return  Z80_STAGE_M3;
                            }
                            //Otherwise, reset pipeline
                            else{
                                assert(z80.write_index == 1);
                                return Z80_STAGE_RESET;
                            }
                        }
                        else if (p[0] == 1) { assert(0); /*LD((DE),A)*/ }
                        else if (p[0] == 2) { assert(0); /*LD((nn),HL)*/ } //Needs two extra bytes
                        else if (p[0] == 3) { assert(0); /*LD((nn),A)*/ }  //Needs two extra bytes
                    }
                    else{
                        if (p[0] == 0) { assert(0); /*LD(A,(BC))*/ }
                        else if (p[0] == 1) { assert(0); /*LD(A,(DE))*/ }
                        else if (p[0] == 2) { assert(0); /*LD(HL,(nn))*/ } //Needs two extra bytes
                        else if (p[0] == 3) { assert(0); /*LD(A,(nn))*/ }  //Needs two extra bytes
                    }// !q
                    break;
                case 3:
                    //Select by q
                    if (!(q[0])){/*INC(rp[p])*/
                        ++z80_rp[p[0]];
                        //No flags were harmed
                        return Z80_STAGE_RESET;
                    }
                    else        { /*DEC(rp[p])*/
                        --z80_rp[p[0]];
                        //No flags were harmed
                        return Z80_STAGE_RESET;
                    }
                    break;
                case 4:
                { /*INC(r[y])*/
                          const uint8_t old_r = *z80_r[y[0]];
                          ++z80_r[y[0]];
                          //Flags

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
                    break;
                case 5:
                { /*DEC(r[y])*/
                          const uint8_t old_r = *z80_r[y[0]];
                          --z80_r[y[0]];
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
                    break;
                case 6:
                { assert(0); /*LD(r[y],n)*/ } //Needs one extra byte
                    break;
                case 7:
                    //Select by 'y'
                    if (y[0] == 0) {/*RLCA*/
                        Z80_A = (Z80_A << 1) | (Z80_A & (1 << 7) ? 1 : 0);
                        //Clear HNC
                        Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_ADD & Z80_CLRFLAG_CARRY))
                            | (Z80_F & 1 ? Z80_FLAG_CARRY : 0); //Set Carry
                        return Z80_STAGE_RESET;
                    }
                    else if (y[0] == 1) {/*RRCA*/
                        Z80_A = (Z80_A >> 1) | (Z80_A & (1 >> 7) ? (1 << 7) : 0);
                        //Clear HNC
                        Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_ADD & Z80_CLRFLAG_CARRY))
                            | (Z80_F & (1 << 7) ? Z80_FLAG_CARRY : 0); //Set Carry
                        return Z80_STAGE_RESET;
                    }
                    else if (y[0] == 2) {/*RLA */
                        const uint8_t next_carry = Z80_A & (1 << 7);
                        Z80_A = (Z80_A << 1) | (Z80_F & Z80_FLAG_CARRY ? 1 : 0);
                        //Clear HNC
                        Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_ADD & Z80_CLRFLAG_CARRY))
                            | (next_carry ? Z80_FLAG_CARRY : 0); //Set Carry
                        return Z80_STAGE_RESET;
                    }
                    else if (y[0] == 3) {/*RRA */
                        const uint8_t next_carry = Z80_A & (1);
                        Z80_A = (Z80_A >> 1) | (Z80_F & Z80_FLAG_CARRY ? (1 << 7) : 0);
                        //Clear HNC
                        Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_ADD & Z80_CLRFLAG_CARRY))
                            | (next_carry ? Z80_FLAG_CARRY : 0); //Set Carry
                        return Z80_STAGE_RESET;
                    }
                    else if (y[0] == 4) { assert(0); /*DAA */ }
                    else if (y[0] == 5) {/*CPL */
                        Z80_A = ~Z80_A;
                        //Set H,N
                        Z80_F = Z80_F | Z80_FLAG_HC | Z80_FLAG_ADD;
                        return Z80_STAGE_RESET;
                    }
                    else if (y[0] == 6) {/*SCF */
                        //Clear H,N. Set C
                        Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_ADD)) | Z80_FLAG_CARRY;
                        return Z80_STAGE_RESET;
                    }
                    else {/*CCF */
                        //Clear N, Swap C
                        Z80_F = (Z80_F ^ Z80_FLAG_CARRY) & Z80_CLRFLAG_ADD;
                        return Z80_STAGE_RESET;
                    }
                    break;
                }//switch (z)
                break;
                // -------------
                // --- X = 1 ---
                // -------------
            case 1:
                //Select by 'z'
                if (z[0] != 6) { assert(0); /*LD(r[y],r[z])*/ }
                else           { assert(0); /*HALT*/ } /// @bug Exception (Replaces LD(HL)(HL)) ?
                break;
                // -------------
                // --- X = 2 ---
                // -------------
            case 2:
            { assert(0); /*ALU_REG_MEM(y,r[z])*/ } //<-- y is the operation, r[z] is the operand
                break;
                // -------------
                // --- X = 3 ---
                // -------------
            case 3:
                //Select by 'z'
                switch (z[0]){
                case 0:
                { assert(0); /*RET(cc[y])*/ }
                    break;
                case 1:
                    if (!(q[0])){ assert(0); /*POP(rp2[p])*/ }
                    else   {
                        if (p[0] == 0) { assert(0); /*RET*/ }
                        else if (p[0] == 1) {/*EXX*/
                            const uint16_t old_bc = Z80_BC;
                            const uint16_t old_de = Z80_DE;
                            const uint16_t old_hl = Z80_HL;
                            Z80_BC = Z80_BCp;
                            Z80_DE = Z80_DEp;
                            Z80_HL = Z80_HLp;
                            Z80_BCp = old_bc;
                            Z80_DEp = old_de;
                            Z80_HLp = old_hl;
                            //No flags were harmed
                            return Z80_STAGE_RESET;
                        }
                        else if (p[0] == 2) { assert(0); /*JP(HL)*/ }
                        else if (p[0] == 3) { assert(0); /*LD(SP,HL)*/ }
                    }
                    break;
                case 2:
                { assert(0); /*JP(cc[y], nn)*/ } //Needs two extra bytes
                    break;
                case 3:
                    if (y[0] == 0) { assert(0);  /*JP nn*/ }      //needs two extra bytes
                    //else if (y == 1) { }              //CB prefix
                    else if (y[0] == 2) { assert(0);  /*OUT((n),A)*/ } //needs one extra byte
                    else if (y[0] == 3) { assert(0);  /*IN(A,(n))*/ }  //needs one extra byte
                    else if (y[0] == 4) { assert(0);  /*EX((SP),HL)*/ }
                    else if (y[0] == 5) {/*EX(DE,HL)*/
                        const uint16_t old_de = Z80_DE;
                        Z80_DE = Z80_HL;
                        Z80_HL = old_de;
                        //No flags were harmed
                        return Z80_STAGE_RESET;
                    }
                    else if (y[0] == 6) {/*DI*/
                        z80.iff[0] = 0;
                        z80.iff[1] = 0;
                        //No flags were harmed
                        return Z80_STAGE_RESET;
                    }
                    else if (y[0] == 7) {/*EI*/
                        z80.iff[0] = 1;
                        z80.iff[1] = 1;
                        //No flags were harmed
                        return Z80_STAGE_RESET;
                    }
                    else { assert(0); /* Should NEVER get here*/ }
                    break;
                case 4:
                { assert(0); /*CALL(cc[y], nn)*/ } //Needs two extra bytes
                    break;
                case 5:
                    if (!(q[0])){ assert(0); /*PUSH(rp2[p])*/ }
                    else   {
                        if (p[0] == 0) { assert(0); /*CALL nn*/ } //Needs two extra bytes
                        else { assert(0); /*Should never get here*/ }
                        //p[0] == 1 --> DD prefix
                        //p[0] == 2 --> ED prefix
                        //p[0] == 3 --> FD prefix
                    }
                    break;
                case 6:
                { assert(0); /*ALU_ACC_INM(y,n)*/ } //Needs one extra byte
                    break;
                case 7:
                    /*RST(y * 8)*/
                    //When in debug mode, RST 0 also dumps the CPU state to stdout
#ifndef NDEBUG
                    if (y == 0){
                        z80_dump();
                    }
#endif
                    assert(0);
                    break;
                }//switch (z)
                break;
            }//switch (x)
        }//if unprefixed opcode
        break;
        //Second opcode byte
    case 1:
        //Test wether this is a prefixed opcode
        if (z80.opcode[0] == 0xCB){  //0xCB prefixed opcodes
            if (x[1] == 0) { assert(0); /*ROT(rot[y], r[z])*/ }
            else if (x[1] == 1) { assert(0); /*BIT(y,r[z])*/ }
            else if (x[1] == 2) { assert(0); /*RES(y,r[z])*/ }
            else if (x[1] == 3) { assert(0); /*SET(y,r[z])*/ }
        }
        else if (z80.opcode[0] == 0xED){ //0xED prefixed opcodes
            //select by 'x'
            if (x[1] == 1) {
                //Select by 'z'
                switch (z[1]){
                case 0:
                    if (y[1] != 6){ assert(0); /*IN(r[y], C)*/ }
                    else          { assert(0); /*IN(C)*/ }
                    break;
                case 1:
                    if (y[1] != 6){ assert(0); /*OUT(r[y], C)*/ }
                    else          { assert(0); /*OUT(C)*/ }
                    break;
                case 2:
                    if (!(q[1])){ assert(0); /*SBC(HL,rp[p])*/ }
                    else        { assert(0); /*ADC(HL,rp[p])*/ }
                    break;
                case 3:
                    if (!(q[1])){ assert(0); /*LD((nn),rp[p])*/ } //Needs two extra bytes
                    else        { assert(0); /*LD(rp[p],(nn))*/ } //Needs two extra bytes
                    break;
                case 4:
                { assert(0); /*NEG*/ }
                    break;
                case 5:
                    if (y[1] != 1) { assert(0); /*RETN*/ }
                    else           { assert(0); /*RETI*/ }
                    break;
                case 6:
                { assert(0); /*IM(im[y])*/ }
                    break;
                case 7:
                    if (y[1] == 0){ assert(0); /*LD(I,A)*/ }
                    else if (y[1] == 1){ assert(0); /*LD(R,A)*/ }
                    else if (y[1] == 2){ assert(0); /*LD(A,I)*/ }
                    else if (y[1] == 3){ assert(0); /*LD(A,R)*/ }
                    else if (y[1] == 4){ assert(0); /*RRD*/ }
                    else if (y[1] == 5){ assert(0); /*RLD*/ }
                    else               { assert(0); /*NOP*/ }
                    break;
                }//Switch(z)
            }
            else if (x[1] == 2) {
                if ((z[1] <= 3) && (y[1] >= 4)){ assert(0); /*BLOCK(y,z)*/ }
                else                           { assert(0); /*NONI + NOP*/ }
            }
            else{ assert(0); /*NONI + NOP*/ }
        }
        else if (z80.opcode[0] == 0xDD){
            if (z80.opcode[1] == 0xDD) z80.opcode_index--; //0xDDDD = 0xDD
            else if (z80.opcode[1] == 0xFD){
                //0xFD replaces a previous 0xDD
                ///@note HL --> IY
                z80.opcode[0] = 0xFD;
                z80.opcode_index--;
                assert(0);
            }
            else if (z80.opcode[1] == 0xED){
                z80.opcode[0] = 0xED;
                z80.opcode_index--;
                ///@bug HL --> HL
                assert(0);
            }
            ///@bug 0xDD prefixed opcodes
            assert(0);
        }
        else if (z80.opcode[0] == 0xFD){
            if (z80.opcode[0] == 0xFD) z80.opcode_index--;
            else if (z80.opcode[1] == 0xDD){
                //0xDD replaces a previous 0xFD
                ///@note HL --> IX
                z80.opcode[0] = 0xDD;
                z80.opcode_index--;
                assert(0);
            }
            else if (z80.opcode[1] == 0xED){
                z80.opcode[0] = 0xED;
                z80.opcode_index--;
                ///@bug HL --> HL
                assert(0);
            }
            ///@bug 0xFD prefixed opcodes
            assert(0);
        }
        else{
            ///@bug Insert 2-byte, unprefixed stuff here.
            assert(0);
        }

        //Third opcode byte
    case 2:
        //Test prefixes
        if (z80.opcode[0] == 0xCB){ assert(0); }
        else if (z80.opcode[0] == 0xED){ assert(0); }
        else if (((z80.opcode[0] == 0xDD) && (z80.opcode[1] == 0xCB)) ||
            ((z80.opcode[0] == 0xFD) && (z80.opcode[1] == 0xCB)))
        {
            //All opcodes at this place need an extra byte
            assert(0);
        }
        else if (z80.opcode[0] == 0xDD) { assert(0); }
        else if (z80.opcode[0] == 0xFD) { assert(0); }
        else{
            ///@bug Insert 3-byte, unprefixed stuff here.
            assert(0);
        } //Prefixes
        break;
        //Fourth opcode byte
    case 3:
        //Test prefixes
        if (z80.opcode[0] == 0xCB){ assert(0); }
        else if (z80.opcode[0] == 0xED){ assert(0); }
        else if (((z80.opcode[0] == 0xDD) && (z80.opcode[1] == 0xCB)) ||
            ((z80.opcode[0] == 0xFD) && (z80.opcode[1] == 0xCB)))
        {
            assert(0);
        }
        else if (z80.opcode[0] == 0xDD) { assert(0); }
        else if (z80.opcode[0] == 0xFD) { assert(0); }
        else{
            ///@bug Insert 4-byte, unprefixed stuff here. (none that I know of).
        } //Prefixes
        break;
    }//switch Opcode index
    return Z80_STAGE_RESET;
}

///Executes the M3 stage (Memory write).
int z80_stage_m3(){
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
        assert((z80.write_index >= 0) && (z80.write_index < 2));
        z80_data = z80.write_buffer[z80.write_index];
        --z80.write_index;
        ++z80.write_address; //Increment address, for 16-bit writes

        //Prepare for next tick (If not an IO cycle, skip next 2 cases)
        if (z80.write_is_io)
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
        if ((z80_n_wait == 0)){
            z80_m3_tick_count -= 2;
            //If this is a SDSC write, covertly call its functions.
            if (z80.write_is_io)
            if ((z80.write_address & 0xFF) == 0xFD)
                sdsc_write(z80.write_buffer[z80.write_index]);
            else if ((z80.write_address & 0xFF) == 0xFC)
                sdsc_control(z80.write_buffer[z80.write_index]);
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

        //Call instruction decoder, return whatever it wants
        return z80_instruction_decode();
    }
    assert(0); //Should never get here.
    return Z80_STAGE_RESET;
}

///Executes the M2 stage (Memory read)
int z80_stage_m2(){
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
        ++z80.read_address; //Increment address, for 16-bit reads

        //Reset #RD, #MREQ, #IOREQ
        z80_n_mreq = 1;
        z80_n_ioreq = 1;
        z80_n_rd = 1;

        //Call instruction decoder, retun whatever it wants.
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
        if   (z80_n_wait) break; //<-- Continue
        else{                    //<-- GoTo T2 up
            --z80_m1_tick_count;
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
    case 0:
        z80_reset_pipeline();
        ++(z80.stage);
        //Fall-through
    case 1:
        z80.stage = z80_stage_m1();
        break;
    case 2:
        z80.stage = z80_stage_m2();
        break;
    case 3:
        z80.stage = z80_stage_m3();
        break;
    default:
        assert(0);
    }
}
