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
    uint8_t data_latch; ///<-- When data bus is sampled, it is stored here.

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

//Address H/L
#define Z80_ADDRH (*((uint8_t*)(&z80_address)))
#define Z80_ADDRL (*((uint8_t*)((&z80_address) + 1)))

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
#define Z80_BC  (*((uint16_t*) z80.rBC))
#define Z80_BCp (*((uint16_t*) z80.rBC + 2))

//DE
#define Z80_D  z80.rDE[1]
#define Z80_E  z80.rDE[0]
#define Z80_Dp z80.rDE[3]
#define Z80_Ep z80.rDE[2]
#define Z80_DE  (*((uint16_t*) z80.rDE))
#define Z80_DEp (*((uint16_t*) z80.rDE + 2))

//HL
#define Z80_H  z80.rHL[1]
#define Z80_L  z80.rHL[0]
#define Z80_Hp z80.rHL[3]
#define Z80_Lp z80.rHL[2]
#define Z80_HL  (*((uint16_t*) z80.rHL))
#define Z80_HLp (*((uint16_t*) z80.rHL + 2))

//WZ
#define Z80_W  z80.rWZ[1]
#define Z80_Z  z80.rWZ[0]
#define Z80_Wp z80.rWZ[3]
#define Z80_Zp z80.rWZ[2]
#define Z80_WZ  (*((uint16_t*) z80.rWZ))
#define Z80_WZp (*((uint16_t*) z80.rWZ + 2))

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


///Executes the M2 stage (Instruction decode/execute)
__inline void z80_stage_m2(){
    switch (z80.opcode_index){
    case 0:
        //No instruction
        break;
    case 1:
        //1 byte
        break;
    case 2:
        //2 bytes
        break;
    case 3:
        //3 bytes
        break;
    case 4:
        //4 bytes
        break;
    default:
        z80_panic(Z80_STAGE_M2, 0);
    }
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
        //    -R register is incremented (7-bit). 8th bit is kept
        Z80_R = (Z80_R & (1 << 7)) | ((Z80_R + 1) % (1 << 7));
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
        //    -MREQ pulled down (Refresh cycle)
        z80_n_mreq = 0;
        //    -Load I to the high address bus @check
        Z80_ADDRH = Z80_I;
        break;

    case 5:
        //On T4 up
        //    <nothing>
        break;

    case 6:
        //On T4 dn
        //    -MREQ pulled up
        z80_n_mreq = 1;

        /// @bug Identify where on the M1 stage is the (partial) opcode latched
        /// here we are presuming it is done at the end of M1 stage.
        z80.opcode[z80.opcode_index] = z80.data_latch;
        ++z80.opcode_index;
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

int AAA_z80_instruction_decode(){

    //Relevant sub-byte divisions, for each of the 4 bytes (max) in an opcode.
    const uint8_t x[4] = { z80.opcode[0] >> 6,         z80.opcode[1] >> 6,         z80.opcode[2] >> 6,         z80.opcode[3] >> 6 };
    const uint8_t y[4] = { (z80.opcode[0] >> 3) & 0x7, (z80.opcode[1] >> 3) & 0x7, (z80.opcode[2] >> 3) & 0x7, (z80.opcode[3] >> 3) & 0x7 };
    const uint8_t z[4] = { z80.opcode[0] & 0x7,        z80.opcode[1] & 0x7,        z80.opcode[2] & 0x7,        z80.opcode[3] & 0x7 };
    const uint8_t p[4] = { (z80.opcode[0] >> 4) & 0x3, (z80.opcode[1] >> 4) & 0x3, (z80.opcode[2] >> 4) & 0x3, (z80.opcode[3] >> 4) & 0x3 };
    const uint8_t q[4] = { z80.opcode[0] & (1 << 3),   z80.opcode[1] & (1 << 3),   z80.opcode[2] & (1 << 3),   z80.opcode[3] & (1 << 3) };

    //We are going to use the method described in z80.info/decoding.html
    switch (z80.opcode_index){

    //First opcode byte
    case 1:
        if     (z80.opcode[0] == 0xCB){ /*Need another byte*/ } //0xCB prefixed opcodes
        else if(z80.opcode[0] == 0xED){ /*Need another byte*/ } //0xED prefixed opcodes
        else if(z80.opcode[0] == 0xDD){ /*Need another byte HL-->IX*/ } //0xDD prefixed opcodes
        else if(z80.opcode[0] == 0xFD){ /*Need another byte HL-->IY*/ } //0xFD prefixed opcodes
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
                    if      (z[0] == 0){ /*OP_NOP*/ }
                    else if (z[0] == 1){ /*OP_EX (AF, AFp)*/ }
                    else if (z[0] == 2){ /*DJNZ (d)*/ }    //Needs one extra byte
                    else if (z[0] == 3){ /*JR (d)*/ }      //Needs one extra byte
                    else               { /*JR (y-4, d)*/ } //Needs one extra byte
                    break;
                case 1:
                    //Select by q 
                    if (q[0]){ /*LD (rp[p], nn)*/ } //Needs two extra bytes
                    else     { /*ADD(HL,rp[p])*/ }
                    break;
                case 2:
                    //Select by q
                    if (!(q[0])){
                        if      (p[0] == 0) { /*LD((BC),A)*/ }
                        else if (p[0] == 1) { /*LD((DE),A)*/ }
                        else if (p[0] == 2) { /*LD((nn),HL)*/ } //Needs two extra bytes
                        else if (p[0] == 3) { /*LD((nn),A)*/ }  //Needs two extra bytes
                    }
                    else{
                        if      (p[0] == 0) { /*LD(A,(BC))*/ }
                        else if (p[0] == 1) { /*LD(A,(DE))*/ }
                        else if (p[0] == 2) { /*LD(HL,(nn))*/ } //Needs two extra bytes
                        else if (p[0] == 3) { /*LD(A,(nn))*/ }  //Needs two extra bytes
                    }// !q
                    break;
                case 3:
                    //Select by q
                    if (!(q[0])){ /*INC(rp[p])*/ }
                    else        { /*DEC(rp[p])*/ }
                    break;
                case 4:
                    { /*INC(r[y])*/ }
                    break;
                case 5:
                    { /*DEC(r[y])*/ }
                    break;
                case 6:
                    { /*LD(r[y],n)*/ } //Needs one extra byte
                    break;
                case 7:
                    //Select by 'y'
                    if      (y[0] == 0) {/*RLCA*/ }
                    else if (y[0] == 0) {/*RLCA*/ }
                    else if (y[0] == 1) {/*RRCA*/ }
                    else if (y[0] == 2) {/*RLA */ }
                    else if (y[0] == 3) {/*RRA */ }
                    else if (y[0] == 4) {/*DAA */ }
                    else if (y[0] == 5) {/*CPL */ }
                    else if (y[0] == 6) {/*SCF */ }
                    else                {/*CCF */ }
                    break;
                }//switch (z)
                break;
            // -------------
            // --- X = 1 ---
            // -------------
            case 1:
                //Select by 'z'
                if (z[0] != 6) { /*LD(r[y],r[z])*/ }
                else           { /*HALT*/ } /// @bug Exception (Replaces LD(HL)(HL)) ?
                break;
            // -------------
            // --- X = 2 ---
            // -------------
            case 2:
                { /*ALU_REG_MEM(y,r[z])*/ } //<-- y is the operation, r[z] is the operand
                break;
            // -------------
            // --- X = 3 ---
            // -------------
            case 3:
                //Select by 'z'
                switch (z[0]){
                case 0:
                    { /*RET(cc[y])*/ }
                    break;
                case 1:
                    if (!(q[0])){ /*POP(rp2[p])*/ }
                    else   {
                        if      (p[0] == 0) { /*RET*/ }
                        else if (p[0] == 1) { /*EXX*/ }
                        else if (p[0] == 2) { /*JP(HL)*/ }
                        else if (p[0] == 3) { /*LD(SP,HL)*/ }
                    }
                    break;
                case 2:
                    { /*JP(cc[y], nn)*/ } //Needs two extra bytes
                    break;
                case 3:
                    if      (y[0] == 0) { /*JP nn*/ }      //needs two extra bytes
                    //else if (y == 1) { }              //CB prefix
                    else if (y[0] == 2) { /*OUT((n),A)*/ } //needs one extra byte
                    else if (y[0] == 3) { /*IN(A,(n))*/ }  //needs one extra byte
                    else if (y[0] == 4) { /*EX((SP),HL)*/ }
                    else if (y[0] == 5) { /*EX(DE,HL)*/ }
                    else if (y[0] == 6) { /*DI*/ }
                    else if (y[0] == 7) { /*EI*/ }
                    break;
                case 4:
                    { /*CALL(cc[y], nn)*/ } //Needs two extra bytes
                    break;
                case 5:
                    if (!(q[0])){ /*PUSH(rp2[p])*/ }
                    else   {
                        if (p[0] == 0) { /*CALL nn*/ } //Needs two extra bytes
                        //p[0] == 1 --> DD prefix
                        //p[0] == 2 --> ED prefix
                        //p[0] == 3 --> FD prefix
                    }
                    break;
                case 6:
                    { /*ALU_ACC_INM(y,n)*/ } //Needs one extra byte
                    break;
                case 7:
                    { /*RST(y * 8)*/ }
                    break;
                }//switch (z)
                break;
            }//switch (x)
        }//if unprefixed opcode
        break;
    //Second opcode byte
    case 2:
        //Test wether this is a prefixed opcode
        if (z80.opcode[0] == 0xCB){  //0xCB prefixed opcodes
            if      (x[1] == 0) { /*ROT(rot[y], r[z])*/ }
            else if (x[1] == 1) { /*BIT(y,r[z])*/ }
            else if (x[1] == 2) { /*RES(y,r[z])*/ }
            else if (x[1] == 3) { /*SET(y,r[z])*/ }
        }
        else if (z80.opcode[0] == 0xED){ //0xED prefixed opcodes
            //select by 'x'
            if (x[1] == 1) {
                //Select by 'z'
                switch (z[1]){
                case 0:
                    if (y[1] != 6){ /*IN(r[y], C)*/ }
                    else          { /*IN(C)*/ }
                    break;
                case 1:
                    if (y[1] != 6){ /*OUT(r[y], C)*/ }
                    else          { /*OUT(C)*/ }
                    break;
                case 2:
                    if (!(q[1])){ /*SBC(HL,rp[p])*/ }
                    else        { /*ADC(HL,rp[p])*/ }
                    break;
                case 3:
                    if (!(q[1])){ /*LD((nn),rp[p])*/ } //Needs two extra bytes
                    else        { /*LD(rp[p],(nn))*/ } //Needs two extra bytes
                    break;
                case 4:
                    { /*NEG*/ }
                    break;
                case 5:
                    if (y[1] != 1) { /*RETN*/ }
                    else           { /*RETI*/ }
                    break;
                case 6:
                    { /*IM(im[y])*/ }
                    break;
                case 7:
                    if      (y[1] == 0){ /*LD(I,A)*/ }
                    else if (y[1] == 1){ /*LD(R,A)*/ }
                    else if (y[1] == 2){ /*LD(A,I)*/ }
                    else if (y[1] == 3){ /*LD(A,R)*/ }
                    else if (y[1] == 4){ /*RRD*/ }
                    else if (y[1] == 5){ /*RLD*/ }
                    else               { /*NOP*/ }
                    break;
                }//Switch(z)
            }
            else if (x[1] == 2) {
                if ((z[1] <= 3) && (y[1] >= 4)){ /*BLOCK(y,z)*/ }
                else                           { /*NONI + NOP*/ }
            }
            else{ /*NONI + NOP*/ }
        }
        else if (z80.opcode[0] == 0xDD){
            if      (z80.opcode[1] == 0xDD) z80.opcode_index--; //0xDDDD = 0xDD
            else if (z80.opcode[1] == 0xFD){
                //0xFD replaces a previous 0xDD
                ///@note HL --> IY
                z80.opcode[0] = 0xFD;
                z80.opcode_index--;
            }
            else if (z80.opcode[1] == 0xED){
                z80.opcode[0] = 0xED;
                z80.opcode_index--;
                ///@bug HL --> HL
            }
            ///@bug 0xDD prefixed opcodes
        } 
        else if (z80.opcode[0] == 0xFD){
            if (z80.opcode[0] == 0xFD) z80.opcode_index--;
            else if (z80.opcode[1] == 0xDD){
                //0xDD replaces a previous 0xFD
                ///@note HL --> IX
                z80.opcode[0] = 0xDD;
                z80.opcode_index--;
            }
            else if (z80.opcode[1] == 0xED){
                z80.opcode[0] = 0xED;
                z80.opcode_index--;
                ///@bug HL --> HL
            }
            ///@bug 0xFD prefixed opcodes
        }
        else{
            ///@bug Insert 2-byte, unprefixed stuff here.
        }

    //Third opcode byte
    case 3:
        //Test prefixes
        if      (z80.opcode[0] == 0xCB){}
        else if (z80.opcode[0] == 0xED){}
        else if (((z80.opcode[0] == 0xDD) && (z80.opcode[1] == 0xCB)) ||
                ((z80.opcode[0] == 0xFD) && (z80.opcode[1] == 0xCB)))
                {
            //All opcodes at this place need an extra byte
        }
        else if (z80.opcode[0] == 0xDD) {}
        else if (z80.opcode[0] == 0xFD) {}
        else{
            ///@bug Insert 3-byte, unprefixed stuff here.
        } //Prefixes
        break;
    //Fourth opcode byte
    case 4:
        //Test prefixes
        if (z80.opcode[0] == 0xCB){}
        else if (z80.opcode[0] == 0xED){}
        else if (((z80.opcode[0] == 0xDD) && (z80.opcode[1] == 0xCB)) ||
            ((z80.opcode[0] == 0xFD) && (z80.opcode[1] == 0xCB)))
        {
        }
        else if (z80.opcode[0] == 0xDD) {}
        else if (z80.opcode[0] == 0xFD) {}
        else{
            ///@bug Insert 4-byte, unprefixed stuff here. (none that I know of).
        } //Prefixes
        break;
    }//switch Opcode index

}

/** Executes a z80 half-clock.
 * 
 */
void z80_tick(){
    z80_stage_m1();
    z80_stage_m2();
}