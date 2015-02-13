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

#include "z80_decoder.h"
#include "z80_macros.h"
#include "z80_register_lut.h"
#include "z80_opcodes.h"
#include <assert.h>

/*
Most information taken from z80.info

Decoding Z80 instructions:
http://www.z80.info/decoding.htm
*/

//Forward declaration of decoder functions
int z80_instruction_decode();

///Removes prefix byte from opcode
void z80_instruction_unprefix(){
    z80.opcode[0] = z80.opcode[1];
    z80.opcode[1] = z80.opcode[2];
    z80.opcode[2] = z80.opcode[3];
    --z80.opcode_index;
}

///Decodes DDCB/FDCB prefix opcodes
int z80_decode_DDCB_FDCB(){

    //Relevant sub-byte divisions, for each of the 4 bytes (max) in an opcode.
    Z80_OPCODE_SUBDIV;

    //DDCB/FDCB are always 4-byte long
    if (z80.opcode_index < 4)
        return Z80_STAGE_M1;

    //Decode
    assert(0);
    return Z80_STAGE_RESET;
}

///Decodes DD/FD prefix
int z80_decode_DD_FD(){

    //Relevant sub-byte divisions, for each of the 4 bytes (max) in an opcode.
    Z80_OPCODE_SUBDIV;

    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///Decodes the CB-prefixed opcodes.
int z80_decode_CB(){

    //Relevant sub-byte divisions, for each of the 4 bytes (max) in an opcode.
    Z80_OPCODE_SUBDIV;

    //If only the prefix has been decoded, fetch one more byte
    if (z80.opcode_index == 1)
        return Z80_STAGE_M1;

    //Decode X==[1,3]
    switch (z80.opcode[1] & Z80_OPCODE_X_MASK){
    case Z80_OPCODE_X(1): return z80_op_BIT_y_r();                      /*BIT y, r (size: 2)*/
    case Z80_OPCODE_X(2): return z80_op_RES_y_r();                      /*RES y, r (size: 2)*/
    case Z80_OPCODE_X(3): return z80_op_SET_y_r();                      /*SET y, r (size: 2)*/
    }

    //Decode X==0 (select by y)
    switch (z80.opcode[1] & Z80_OPCODE_Y_MASK){
    case Z80_OPCODE_Y(0): return z80_op_RLC_r();
    case Z80_OPCODE_Y(1): return z80_op_RRC_r();
    case Z80_OPCODE_Y(2): return z80_op_RL_r();
    case Z80_OPCODE_Y(3): return z80_op_RR_r();
    case Z80_OPCODE_Y(4): return z80_op_SLA_r();
    case Z80_OPCODE_Y(5): return z80_op_SRA_r();
    case Z80_OPCODE_Y(6): return z80_op_SLL_r();
    case Z80_OPCODE_Y(7): return z80_op_SRL_r();
    default:
        assert(0); ///<-- Will never get here
        return Z80_STAGE_RESET;
    }
}


///Decodes ED prefixed opcodes
int z80_decode_ED(){
    Z80_OPCODE_SUBDIV;

    //If only the prefix was decoded, ask for another byte
    if (z80.opcode_index == 1)
        return Z80_STAGE_M1;

    //If X==0 or X==3, reset. (NONI/NOP)
    if ((x[1] == 0) || (x[1] == 3))
        return Z80_STAGE_RESET;

    //Otherwise, decode.
    //Note that the decoder cases are *NOT* sorted in ascending order unlike
    //the base decoder function
    switch (z80.opcode_index){
    case 2:
        switch (z80.opcode[1]){
        case Z80_OPCODE_XYZ(1, 0, 0): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 1, 0): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 2, 0): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 3, 0): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 4, 0): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 5, 0): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 7, 0): return z80_op_IN_r_Cp();         /*IN r, (C) (size: 2)*/
        case Z80_OPCODE_XYZ(1, 6, 0): return z80_op_IN_Cp();              /*IN (C) (Size: 2)*/

        case Z80_OPCODE_XYZ(1, 0, 1): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 1, 1): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 2, 1): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 3, 1): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 4, 1): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 5, 1): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 7, 1): return z80_op_OUT_Cp_r();       /*OUT (C), r (size: 2)*/
        case Z80_OPCODE_XYZ(1, 6, 1): return z80_op_OUT_Cp_0();       /*OUT (C), 0 (Size: 2)*/

        case Z80_OPCODE_XPQZ(1, 0, 0, 2): /*fall-through*/
        case Z80_OPCODE_XPQZ(1, 1, 0, 2): /*fall-through*/
        case Z80_OPCODE_XPQZ(1, 2, 0, 2): /*fall-through*/
        case Z80_OPCODE_XPQZ(1, 3, 0, 2): return z80_op_SBC_HL_rp();  /*SBC HL, rp (size: 2)*/
        case Z80_OPCODE_XPQZ(1, 0, 1, 2): /*fall-through*/
        case Z80_OPCODE_XPQZ(1, 1, 1, 2): /*fall-through*/
        case Z80_OPCODE_XPQZ(1, 2, 1, 2): /*fall-through*/
        case Z80_OPCODE_XPQZ(1, 3, 1, 2): return z80_op_ADC_HL_rp();  /*ADC HL, rp (size: 2)*/

        case Z80_OPCODE_XPQZ(1, 0, 0, 3): /*fall-through*/
        case Z80_OPCODE_XPQZ(1, 1, 0, 3): /*fall-through*/
        case Z80_OPCODE_XPQZ(1, 2, 0, 3): /*fall-through*/
        case Z80_OPCODE_XPQZ(1, 3, 0, 3): return Z80_STAGE_M1;       /*LD (nn), rp (Size: 4)*/
        case Z80_OPCODE_XPQZ(1, 0, 1, 3): /*fall-through*/
        case Z80_OPCODE_XPQZ(1, 1, 1, 3): /*fall-through*/
        case Z80_OPCODE_XPQZ(1, 2, 1, 3): /*fall-through*/
        case Z80_OPCODE_XPQZ(1, 3, 1, 3): return Z80_STAGE_M1;       /*LD rp, (nn) (size: 4)*/

        case Z80_OPCODE_XYZ(1, 0, 4): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 1, 4): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 2, 4): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 3, 4): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 4, 4): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 5, 4): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 6, 4): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 7, 4): return z80_op_NEG();                   /*NEG (Size: 2)*/

        case Z80_OPCODE_XYZ(1, 0, 5): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 2, 5): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 3, 5): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 4, 5): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 5, 5): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 6, 5): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 7, 5): return z80_op_RETN();                 /*RETN (size: 2)*/
        case Z80_OPCODE_XYZ(1, 1, 5): return z80_op_RETI();                 /*RETI (size: 2)*/

        case Z80_OPCODE_XYZ(1, 0, 6): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 1, 6): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 2, 6): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 3, 6): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 4, 6): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 5, 6): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 6, 6): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 7, 6): return z80_op_IM();                   /*IM y (size: 2)*/

        case Z80_OPCODE_XYZ(1, 0, 7): return z80_op_LD_I_A();            /*LD I, A (size: 2)*/
        case Z80_OPCODE_XYZ(1, 1, 7): return z80_op_LD_R_A();            /*LD R, A (size: 2)*/
        case Z80_OPCODE_XYZ(1, 2, 7): return z80_op_LD_A_I();            /*LD A, I (size: 2)*/
        case Z80_OPCODE_XYZ(1, 3, 7): return z80_op_LD_A_R();            /*LD A, R (size: 2)*/
        case Z80_OPCODE_XYZ(1, 4, 7): return z80_op_RRD();                   /*RRD (size: 2)*/
        case Z80_OPCODE_XYZ(1, 5, 7): return z80_op_RLD();                   /*RLD (size: 2)*/
        case Z80_OPCODE_XYZ(1, 6, 7): /*fall-through*/
        case Z80_OPCODE_XYZ(1, 7, 7): return z80_op_NOP();                   /*NOP (size: 2)*/

        default:
            assert(0); ///<-- Should never get here
            return Z80_STAGE_RESET;
        }
    case 3:
        assert(0); ///<-- There are no 0xED prefixed opcodes of length 3
        return Z80_STAGE_RESET;
    case 4:
        switch (z80.opcode[1]){
        case Z80_OPCODE_XPQZ(1, 0, 0, 3): /*fall-through*/
        case Z80_OPCODE_XPQZ(1, 1, 0, 3): /*fall-through*/
        case Z80_OPCODE_XPQZ(1, 2, 0, 3): /*fall-through*/
        case Z80_OPCODE_XPQZ(1, 3, 0, 3): return z80_op_LD_nnp_rp(); /*LD (nn), rp (Size: 4)*/
        case Z80_OPCODE_XPQZ(1, 0, 1, 3): /*fall-through*/
        case Z80_OPCODE_XPQZ(1, 1, 1, 3): /*fall-through*/
        case Z80_OPCODE_XPQZ(1, 2, 1, 3): /*fall-through*/
        case Z80_OPCODE_XPQZ(1, 3, 1, 3): return z80_op_LD_rp_nnp(); /*LD rp, (nn) (size: 4)*/

        default:
            assert(0); ///<-- Should never get here
            return Z80_STAGE_RESET;
        }
    default:
        assert(0); ///<-- Opcode overflow
        return Z80_STAGE_RESET;
    }
}

/**
* @brief decode/execute a Z80 opcode.
*
* Decode a z80 opcode, if the opcode is completly read, execute it.
* If a memory read/write is needed, signal it by returning the apropriate
* Z80_STAGE value.
*
* @return Signal wether we need a fetch/read/write or reset.
*/
int z80_instruction_decode(){

    //Relevant sub-byte divisions, for each of the 4 bytes (max) in an opcode.
    Z80_OPCODE_SUBDIV;

    //Select by X/Z at the same time
    switch (z80.opcode_index){

    case 1: //First opcode byte
        //Select by first byte
        //All cases are sorted in ascending order.
        //To ease debugging, XYZ and XPQZ macros are used
        switch (z80.opcode[0]){
        //X==0
        case Z80_OPCODE_XYZ(0, 0, 0): return z80_op_NOP();                   /*NOP (Size: 1)*/
        case Z80_OPCODE_XYZ(0, 0, 1): return Z80_STAGE_M1;             /*LD rp, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(0, 0, 0, 2): return z80_op_LD_BCp_A();   /*LD (BC), A (size: 1)*/
        case Z80_OPCODE_XYZ(0, 0, 3): return z80_op_INC_rp();             /*INC rp (size: 1)*/
        case Z80_OPCODE_XYZ(0, 0, 4): return z80_op_INC_r();               /*INC r (Size: 1)*/
        case Z80_OPCODE_XYZ(0, 0, 5): return z80_op_DEC_r();               /*DEC r (size: 1)*/
        case Z80_OPCODE_XYZ(0, 0, 6): return Z80_STAGE_M1;               /*LD r, n (size: 2)*/
        case Z80_OPCODE_XYZ(0, 0, 7): return z80_op_RLCA();                 /*RLCA (size: 1)*/

        case Z80_OPCODE_XYZ(0, 1, 0): return z80_op_EX();             /*EX AF, AF' (Size: 1)*/
        case Z80_OPCODE_XYZ(0, 1, 1): return z80_op_ADD_HL_rp();      /*ADD HL, rp (size: 1)*/
        case Z80_OPCODE_XPQZ(0, 0, 1, 2): return z80_op_LD_A_BCp();   /*LD A, (BC) (size: 1)*/
        case Z80_OPCODE_XYZ(0, 1, 3): return z80_op_DEC_rp();             /*DEC rp (size: 1)*/
        case Z80_OPCODE_XYZ(0, 1, 4): return z80_op_INC_r();               /*INC r (Size: 1)*/
        case Z80_OPCODE_XYZ(0, 1, 5): return z80_op_DEC_r();               /*DEC r (size: 1)*/
        case Z80_OPCODE_XYZ(0, 1, 6): return Z80_STAGE_M1;               /*LD r, n (size: 2)*/
        case Z80_OPCODE_XYZ(0, 1, 7): return z80_op_RRCA();                 /*RRCA (size: 1)*/

        case Z80_OPCODE_XYZ(0, 2, 0): return Z80_STAGE_M1;                /*DJNZ d (size: 2)*/
        case Z80_OPCODE_XYZ(0, 2, 1): return Z80_STAGE_M1;             /*LD rp, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(0, 1, 0, 2): return z80_op_LD_DEp_A();   /*LD (DE), A (size: 1)*/
        case Z80_OPCODE_XYZ(0, 2, 3): return z80_op_INC_rp();             /*INC rp (size: 1)*/
        case Z80_OPCODE_XYZ(0, 2, 4): return z80_op_INC_r();               /*INC r (Size: 1)*/
        case Z80_OPCODE_XYZ(0, 2, 5): return z80_op_DEC_r();               /*DEC r (size: 1)*/
        case Z80_OPCODE_XYZ(0, 2, 6): return Z80_STAGE_M1;               /*LD r, n (size: 2)*/
        case Z80_OPCODE_XYZ(0, 2, 7): return z80_op_RLA();                   /*RLA (size: 1)*/

        case Z80_OPCODE_XYZ(0, 3, 0): return Z80_STAGE_M1;                  /*JR d (size: 2)*/
        case Z80_OPCODE_XYZ(0, 3, 1): return z80_op_ADD_HL_rp();      /*ADD HL, rp (size: 1)*/
        case Z80_OPCODE_XPQZ(0, 1, 1, 2): return z80_op_LD_A_DEp();    /*LD A,(DE) (size: 1)*/
        case Z80_OPCODE_XYZ(0, 3, 3): return z80_op_DEC_rp();             /*DEC rp (size: 1)*/
        case Z80_OPCODE_XYZ(0, 3, 4): return z80_op_INC_r();               /*INC r (Size: 1)*/
        case Z80_OPCODE_XYZ(0, 3, 5): return z80_op_DEC_r();               /*DEC r (size: 1)*/
        case Z80_OPCODE_XYZ(0, 3, 6): return Z80_STAGE_M1;               /*LD r, n (size: 2)*/
        case Z80_OPCODE_XYZ(0, 3, 7): return z80_op_RRA();                   /*RRA (size: 1)*/

        case Z80_OPCODE_XYZ(0, 4, 0): return Z80_STAGE_M1;               /*JR cc, d (size 2)*/
        case Z80_OPCODE_XYZ(0, 4, 1): return Z80_STAGE_M1;             /*LD rp, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(0, 2, 0, 2): return Z80_STAGE_M1;       /*LD (nn), HL (size: 3)*/
        case Z80_OPCODE_XYZ(0, 4, 3): return z80_op_INC_rp();             /*INC rp (size: 1)*/
        case Z80_OPCODE_XYZ(0, 4, 4): return z80_op_INC_r();               /*INC r (Size: 1)*/
        case Z80_OPCODE_XYZ(0, 4, 5): return z80_op_DEC_r();               /*DEC r (size: 1)*/
        case Z80_OPCODE_XYZ(0, 4, 6): return Z80_STAGE_M1;               /*LD r, n (size: 2)*/
        case Z80_OPCODE_XYZ(0, 4, 7): return z80_op_DAA();                   /*DAA (size: 1)*/

        case Z80_OPCODE_XYZ(0, 5, 0): return Z80_STAGE_M1;               /*JR cc, d (size 2)*/
        case Z80_OPCODE_XYZ(0, 5, 1): return z80_op_ADD_HL_rp();      /*ADD HL, rp (size: 1)*/
        case Z80_OPCODE_XPQZ(0, 2, 1, 2): return Z80_STAGE_M1;       /*LD HL, (nn) (size: 3)*/
        case Z80_OPCODE_XYZ(0, 5, 3): return z80_op_DEC_rp();             /*DEC rp (size: 1)*/
        case Z80_OPCODE_XYZ(0, 5, 4): return z80_op_INC_r();               /*INC r (Size: 1)*/
        case Z80_OPCODE_XYZ(0, 5, 5): return z80_op_DEC_r();               /*DEC r (size: 1)*/
        case Z80_OPCODE_XYZ(0, 5, 6): return Z80_STAGE_M1;               /*LD r, n (size: 2)*/
        case Z80_OPCODE_XYZ(0, 5, 7): return z80_op_CPL();                   /*CPL (size: 1)*/

        case Z80_OPCODE_XYZ(0, 6, 0): return Z80_STAGE_M1;               /*JR cc, d (size 2)*/
        case Z80_OPCODE_XYZ(0, 6, 1): return Z80_STAGE_M1;             /*LD rp, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(0, 3, 0, 2): return Z80_STAGE_M1;        /*LD (nn), A (size: 3)*/
        case Z80_OPCODE_XYZ(0, 6, 3): return z80_op_INC_rp();             /*INC rp (size: 1)*/
        case Z80_OPCODE_XYZ(0, 6, 4): return z80_op_INC_r();               /*INC r (Size: 1)*/
        case Z80_OPCODE_XYZ(0, 6, 5): return z80_op_DEC_r();               /*DEC r (size: 1)*/
        case Z80_OPCODE_XYZ(0, 6, 6): return Z80_STAGE_M1;               /*LD r, n (size: 2)*/
        case Z80_OPCODE_XYZ(0, 6, 7): return z80_op_SCF();                   /*SCF (size: 1)*/

        case Z80_OPCODE_XYZ(0, 7, 0): return Z80_STAGE_M1;               /*JR cc, d (size 2)*/
        case Z80_OPCODE_XYZ(0, 7, 1): return z80_op_ADD_HL_rp();      /*ADD HL, rp (size: 1)*/
        case Z80_OPCODE_XPQZ(0, 3, 1, 2): return Z80_STAGE_M1;        /*LD A, (nn) (size: 3)*/
        case Z80_OPCODE_XYZ(0, 7, 3): return z80_op_DEC_rp();             /*DEC rp (size: 1)*/
        case Z80_OPCODE_XYZ(0, 7, 4): return z80_op_INC_r();               /*INC r (Size: 1)*/
        case Z80_OPCODE_XYZ(0, 7, 5): return z80_op_DEC_r();               /*DEC r (size: 1)*/
        case Z80_OPCODE_XYZ(0, 7, 6): return Z80_STAGE_M1;               /*LD r, n (size: 2)*/
        case Z80_OPCODE_XYZ(0, 7, 7): return z80_op_CCF();                   /*CCF (size: 1)*/

        //X==1
        case Z80_OPCODE_XYZ(1, 0, 0): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 0, 1): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 0, 2): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 0, 3): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 0, 4): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 0, 5): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 0, 6): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 0, 7): /*Fall-through*/

        case Z80_OPCODE_XYZ(1, 1, 0): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 1, 1): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 1, 2): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 1, 3): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 1, 4): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 1, 5): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 1, 6): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 1, 7): /*Fall-through*/

        case Z80_OPCODE_XYZ(1, 2, 0): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 2, 1): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 2, 2): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 2, 3): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 2, 4): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 2, 5): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 2, 6): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 2, 7): /*Fall-through*/

        case Z80_OPCODE_XYZ(1, 3, 0): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 3, 1): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 3, 2): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 3, 3): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 3, 4): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 3, 5): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 3, 6): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 3, 7): /*Fall-through*/

        case Z80_OPCODE_XYZ(1, 4, 0): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 4, 1): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 4, 2): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 4, 3): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 4, 4): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 4, 5): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 4, 6): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 4, 7): /*Fall-through*/

        case Z80_OPCODE_XYZ(1, 5, 0): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 5, 1): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 5, 2): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 5, 3): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 5, 4): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 5, 5): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 5, 6): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 5, 7): /*Fall-through*/

        case Z80_OPCODE_XYZ(1, 6, 0): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 6, 1): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 6, 2): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 6, 3): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 6, 4): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 6, 5): return z80_op_LD_r_r();            /*LD r, r (size: 1)*/
        case Z80_OPCODE_XYZ(1, 6, 6): return z80_op_HALT();                 /*HALT (size: 1)*/ /*Replaces LD (HL), (HL)*/
        case Z80_OPCODE_XYZ(1, 6, 7): /*Fall-through*/

        case Z80_OPCODE_XYZ(1, 7, 0): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 7, 1): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 7, 2): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 7, 3): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 7, 4): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 7, 5): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 7, 6): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 7, 7): return z80_op_LD_r_r();            /*LD r, r (size: 1)*/

        //X==2
        case Z80_OPCODE_XYZ(2, 0, 0): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 0, 1): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 0, 2): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 0, 3): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 0, 4): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 0, 5): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 0, 6): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 0, 7): return z80_op_ADD_r();               /*ADD r (size: 1)*/

        case Z80_OPCODE_XYZ(2, 1, 0): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 1, 1): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 1, 2): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 1, 3): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 1, 4): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 1, 5): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 1, 6): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 1, 7): return z80_op_ADC_r();               /*ADC r (size: 1)*/

        case Z80_OPCODE_XYZ(2, 2, 0): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 2, 1): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 2, 2): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 2, 3): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 2, 4): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 2, 5): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 2, 6): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 2, 7): return z80_op_SUB_r();               /*SUB r (size: 1)*/

        case Z80_OPCODE_XYZ(2, 3, 0): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 3, 1): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 3, 2): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 3, 3): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 3, 4): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 3, 5): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 3, 6): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 3, 7): return z80_op_SBC_r();               /*SBC r (size: 1)*/

        case Z80_OPCODE_XYZ(2, 4, 0): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 4, 1): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 4, 2): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 4, 3): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 4, 4): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 4, 5): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 4, 6): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 4, 7): return z80_op_AND_r();               /*AND r (size: 1)*/

        case Z80_OPCODE_XYZ(2, 5, 0): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 5, 1): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 5, 2): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 5, 3): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 5, 4): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 5, 5): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 5, 6): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 5, 7): return z80_op_XOR_r();               /*XOR r (size: 1)*/

        case Z80_OPCODE_XYZ(2, 6, 0): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 6, 1): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 6, 2): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 6, 3): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 6, 4): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 6, 5): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 6, 6): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 6, 7): return z80_op_OR_r();                 /*OR r (size: 1)*/

        case Z80_OPCODE_XYZ(2, 7, 0): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 7, 1): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 7, 2): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 7, 3): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 7, 4): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 7, 5): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 7, 6): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 7, 7): return z80_op_CP_r();                 /*CP r (size: 1)*/

        //X==3
        case Z80_OPCODE_XYZ(3, 0, 0): return z80_op_RET_cc();             /*RET cc (size: 1)*/
        case Z80_OPCODE_XPQZ(3, 0, 0, 1): return z80_op_POP_rp2();       /*POP rp2 (size: 1)*/
        case Z80_OPCODE_XYZ(3, 0, 2): return Z80_STAGE_M1;             /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 0, 3): return Z80_STAGE_M1;                 /*JP nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 0, 4): return Z80_STAGE_M1;           /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 0, 0, 5): return z80_op_PUSH_rp2();     /*PUSH rp2 (size: 1)*/
        case Z80_OPCODE_XPQZ(3, 0, 0, 6): return Z80_STAGE_M1;             /*ADD n (size: 2)*/
        case Z80_OPCODE_XYZ(3, 0, 7): return z80_op_RST_y();               /*RST y (size: 1)*/

        case Z80_OPCODE_XYZ(3, 1, 0): return z80_op_RET_cc();             /*RET cc (size: 1)*/
        case Z80_OPCODE_XPQZ(3, 0, 1, 1): return z80_op_RET();               /*RET (size: 1)*/
        case Z80_OPCODE_XYZ(3, 1, 2): return Z80_STAGE_M1;             /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 1, 3): return z80_decode_CB();                  /*0xCB Prefix*/
        case Z80_OPCODE_XYZ(3, 1, 4): return Z80_STAGE_M1;           /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 0, 1, 5): return Z80_STAGE_M1;           /*CALL nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 0, 1, 6): return Z80_STAGE_M1;             /*ADC n (size: 2)*/
        case Z80_OPCODE_XYZ(3, 1, 7): return z80_op_RST_y();               /*RST y (size: 1)*/

        case Z80_OPCODE_XYZ(3, 2, 0): return z80_op_RET_cc();             /*RET cc (size: 1)*/
        case Z80_OPCODE_XPQZ(3, 1, 0, 1): return z80_op_POP_rp2();       /*POP rp2 (size: 1)*/
        case Z80_OPCODE_XYZ(3, 2, 2): return Z80_STAGE_M1;             /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 2, 3): return Z80_STAGE_M1;            /*OUT (n), A (size: 2)*/
        case Z80_OPCODE_XYZ(3, 2, 4): return Z80_STAGE_M1;           /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 1, 0, 5): return z80_op_PUSH_rp2();     /*PUSH rp2 (size: 1)*/
        case Z80_OPCODE_XPQZ(3, 1, 0, 6): return Z80_STAGE_M1;             /*SUB n (size: 2)*/
        case Z80_OPCODE_XYZ(3, 2, 7): return z80_op_RST_y();               /*RST y (size: 1)*/

        case Z80_OPCODE_XYZ(3, 3, 0): return z80_op_RET_cc();             /*RET cc (size: 1)*/
        case Z80_OPCODE_XPQZ(3, 1, 1, 1): return z80_op_EXX();               /*EXX (size: 1)*/
        case Z80_OPCODE_XYZ(3, 3, 2): return Z80_STAGE_M1;             /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 3, 3): return Z80_STAGE_M1;             /*IN A, (n) (size: 2)*/
        case Z80_OPCODE_XYZ(3, 3, 4): return Z80_STAGE_M1;           /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 1, 1, 5): return z80_decode_DD_FD();             /*DD prefix*/
        case Z80_OPCODE_XPQZ(3, 1, 1, 6): return Z80_STAGE_M1;             /*SBC n (size: 2)*/
        case Z80_OPCODE_XYZ(3, 3, 7): return z80_op_RST_y();               /*RST y (size: 1)*/

        case Z80_OPCODE_XYZ(3, 4, 0): return z80_op_RET_cc();             /*RET cc (size: 1)*/
        case Z80_OPCODE_XPQZ(3, 2, 0, 1): return z80_op_POP_rp2();       /*POP rp2 (size: 1)*/
        case Z80_OPCODE_XYZ(3, 4, 2): return Z80_STAGE_M1;             /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 4, 3): return z80_op_EX_SPp_HL();     /*EX (SP), HL (size: 1)*/
        case Z80_OPCODE_XYZ(3, 4, 4): return Z80_STAGE_M1;           /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 2, 0, 5): return z80_op_PUSH_rp2();     /*PUSH rp2 (size: 1)*/
        case Z80_OPCODE_XPQZ(3, 2, 0, 6): return Z80_STAGE_M1;             /*AND n (size: 2)*/
        case Z80_OPCODE_XYZ(3, 4, 7): return z80_op_RST_y();               /*RST y (size: 1)*/

        case Z80_OPCODE_XYZ(3, 5, 0): return z80_op_RET_cc();             /*RET cc (size: 1)*/
        case Z80_OPCODE_XPQZ(3, 2, 1, 1): return z80_op_JP_HL();           /*JP HL (size: 1)*/
        case Z80_OPCODE_XYZ(3, 5, 2): return Z80_STAGE_M1;             /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 5, 3): return z80_op_EX_DE_HL();        /*EX DE, HL (size: 1)*/
        case Z80_OPCODE_XYZ(3, 5, 4): return Z80_STAGE_M1;           /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 2, 1, 5): return z80_decode_ED();                /*ED prefix*/
        case Z80_OPCODE_XPQZ(3, 2, 1, 6): return Z80_STAGE_M1;             /*XOR n (size: 2)*/
        case Z80_OPCODE_XYZ(3, 5, 7): return z80_op_RST_y();               /*RST y (size: 1)*/

        case Z80_OPCODE_XYZ(3, 6, 0): return z80_op_RET_cc();             /*RET cc (size: 1)*/
        case Z80_OPCODE_XPQZ(3, 3, 0, 1): return z80_op_POP_rp2();       /*POP rp2 (size: 1)*/
        case Z80_OPCODE_XYZ(3, 6, 2): return Z80_STAGE_M1;             /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 6, 3): return z80_op_DI();                     /*DI (size: 1)*/
        case Z80_OPCODE_XYZ(3, 6, 4): return Z80_STAGE_M1;           /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 3, 0, 5): return z80_op_PUSH_rp2();     /*PUSH rp2 (size: 1)*/
        case Z80_OPCODE_XPQZ(3, 3, 0, 6): return Z80_STAGE_M1;             /*OR  n (size: 2)*/
        case Z80_OPCODE_XYZ(3, 6, 7): return z80_op_RST_y();               /*RST y (size: 1)*/

        case Z80_OPCODE_XYZ(3, 7, 0): return z80_op_RET_cc();             /*RET cc (size: 1)*/
        case Z80_OPCODE_XPQZ(3, 3, 1, 1): return z80_op_LD_SP_HL();    /*LD SP, HL (size: 1)*/
        case Z80_OPCODE_XYZ(3, 7, 2): return Z80_STAGE_M1;             /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 7, 3): return z80_op_EI();                     /*EI (size: 1)*/
        case Z80_OPCODE_XYZ(3, 7, 4): return Z80_STAGE_M1;           /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 3, 1, 5): return z80_decode_DD_FD();             /*FD prefix*/
        case Z80_OPCODE_XPQZ(3, 3, 1, 6): return Z80_STAGE_M1;             /*CP  n (size: 2)*/
        case Z80_OPCODE_XYZ(3, 7, 7): return z80_op_RST_y();               /*RST y (size: 1)*/

        default:
            assert(0); //Should never get here.
            return Z80_STAGE_RESET;
        }

    case 2: //Second opcode byte
        //Test prefixes
        switch (z80.opcode[0]){
        case 0xCB: // --- 0xCB prefixed opcodes
            return z80_decode_CB();
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
            return z80_decode_DD_FD();
        case 0xED:
            return z80_decode_ED();
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
            return z80_decode_DD_FD();
        }
        //Unprefixed opcodes below
        switch (z80.opcode[0]){

        case Z80_OPCODE_XYZ(0, 0, 1): return Z80_STAGE_M1;             /*LD rp, nn (size: 3)*/
        case Z80_OPCODE_XYZ(0, 0, 6): return z80_op_LD_r_n();            /*LD r, n (size: 2)*/
        case Z80_OPCODE_XYZ(0, 1, 6): return z80_op_LD_r_n();            /*LD r, n (size: 2)*/
        case Z80_OPCODE_XYZ(0, 2, 0): return z80_op_DJNZ_d();             /*DJNZ d (size: 2)*/
        case Z80_OPCODE_XYZ(0, 2, 1): return Z80_STAGE_M1;             /*LD rp, nn (size: 3)*/
        case Z80_OPCODE_XYZ(0, 2, 6): return z80_op_LD_r_n();            /*LD r, n (size: 2)*/
        case Z80_OPCODE_XYZ(0, 3, 0): return z80_op_JR_d();                 /*JR d (size: 2)*/
        case Z80_OPCODE_XYZ(0, 3, 6): return z80_op_LD_r_n();            /*LD r, n (size: 2)*/
        case Z80_OPCODE_XYZ(0, 4, 0): return z80_op_JR_cc_d();           /*JR cc, d (size 2)*/
        case Z80_OPCODE_XYZ(0, 4, 1): return Z80_STAGE_M1;             /*LD rp, nn (size: 3)*/
        case Z80_OPCODE_XYZ(0, 4, 6): return z80_op_LD_r_n();            /*LD r, n (size: 2)*/
        case Z80_OPCODE_XPQZ(0, 2, 0, 2): return Z80_STAGE_M1;       /*LD (nn), HL (size: 3)*/
        case Z80_OPCODE_XYZ(0, 5, 0): return z80_op_JR_cc_d();           /*JR cc, d (size 2)*/
        case Z80_OPCODE_XPQZ(0, 2, 1, 2): return Z80_STAGE_M1;       /*LD HL, (nn) (size: 3)*/
        case Z80_OPCODE_XYZ(0, 5, 6): return z80_op_LD_r_n();            /*LD r, n (size: 2)*/
        case Z80_OPCODE_XYZ(0, 6, 0): return z80_op_JR_cc_d();           /*JR cc, d (size 2)*/
        case Z80_OPCODE_XYZ(0, 6, 1): return Z80_STAGE_M1;             /*LD rp, nn (size: 3)*/
        case Z80_OPCODE_XYZ(0, 6, 6): return z80_op_LD_r_n();            /*LD r, n (size: 2)*/
        case Z80_OPCODE_XPQZ(0, 3, 0, 2): return Z80_STAGE_M1;        /*LD (nn), A (size: 3)*/
        case Z80_OPCODE_XYZ(0, 7, 0): return z80_op_JR_cc_d();           /*JR cc, d (size 2)*/
        case Z80_OPCODE_XYZ(0, 7, 6): return z80_op_LD_r_n();            /*LD r, n (size: 2)*/
        case Z80_OPCODE_XPQZ(0, 3, 1, 2): return Z80_STAGE_M1;        /*LD A, (nn) (size: 3)*/
        case Z80_OPCODE_XYZ(3, 0, 2): return Z80_STAGE_M1;             /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 0, 3): return Z80_STAGE_M1;                 /*JP nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 0, 4): return Z80_STAGE_M1;           /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 0, 0, 6): return z80_op_AND_n();           /*ADD n (size: 2)*/
        case Z80_OPCODE_XYZ(3, 1, 2): return Z80_STAGE_M1;             /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 1, 4): return Z80_STAGE_M1;           /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 0, 1, 5): return Z80_STAGE_M1;           /*CALL nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 0, 1, 6): return z80_op_ADC_n();           /*ADC n (size: 2)*/
        case Z80_OPCODE_XYZ(3, 2, 2): return Z80_STAGE_M1;             /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 2, 3): return z80_op_OUT_np_A();       /*OUT (n), A (size: 2)*/
        case Z80_OPCODE_XYZ(3, 2, 4): return Z80_STAGE_M1;           /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 1, 0, 6): return z80_op_SUB_n();           /*SUB n (size: 2)*/
        case Z80_OPCODE_XYZ(3, 3, 2): return Z80_STAGE_M1;             /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 3, 3): return z80_op_IN_A_np();         /*IN A, (n) (size: 2)*/
        case Z80_OPCODE_XYZ(3, 3, 4): return Z80_STAGE_M1;           /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 1, 1, 6): return z80_op_SBC_n();           /*SBC n (size: 2)*/
        case Z80_OPCODE_XYZ(3, 4, 2): return Z80_STAGE_M1;             /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 4, 4): return Z80_STAGE_M1;           /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 2, 0, 6): return z80_op_AND_n();           /*AND n (size: 2)*/
        case Z80_OPCODE_XYZ(3, 5, 2): return Z80_STAGE_M1;             /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 5, 4): return Z80_STAGE_M1;           /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 2, 1, 6): return z80_op_XOR_n();           /*XOR n (size: 2)*/
        case Z80_OPCODE_XYZ(3, 6, 2): return Z80_STAGE_M1;             /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 6, 4): return Z80_STAGE_M1;           /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 3, 0, 6): return z80_op_OR_n();            /*OR  n (size: 2)*/
        case Z80_OPCODE_XYZ(3, 7, 2): return Z80_STAGE_M1;             /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 7, 4): return Z80_STAGE_M1;           /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 3, 1, 6): return z80_op_CP_n();            /*CP  n (size: 2)*/

        default:
            assert(0); //Should never get here.
            return Z80_STAGE_RESET;
        }

    case 3: //Third opcode byte
        //Test prefixes
        if ((z80.opcode[0] == 0xDD) && (z80.opcode[1] == 0xCB))
            return z80_decode_DDCB_FDCB();
        else if ((z80.opcode[0] == 0xFD) && (z80.opcode[1] == 0xCB))
            return z80_decode_DDCB_FDCB();
        else if (z80.opcode[0] == 0xED)
            return z80_decode_ED();
        /* 0xCB prefixed opcodes are at most 2-bytes long*/

        //Unprefixed opcodes, third byte
        switch (z80.opcode[0]){
        case Z80_OPCODE_XYZ(0, 0, 1): /*fall-through*/                 /*LD rp, nn (size: 3)*/
        case Z80_OPCODE_XYZ(0, 2, 1): /*fall-through*/                 /*LD rp, nn (size: 3)*/
        case Z80_OPCODE_XYZ(0, 4, 1): return z80_op_LD_rp_nn();        /*LD rp, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(0, 2, 0, 2): return z80_op_LD_nnp_HL(); /*LD (nn), HL (size: 3)*/
        case Z80_OPCODE_XPQZ(0, 2, 1, 2): return z80_op_LD_HL_nnp(); /*LD HL, (nn) (size: 3)*/
        case Z80_OPCODE_XYZ(0, 6, 1): return z80_op_LD_rp_nn();        /*LD rp, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(0, 3, 0, 2): return z80_op_LD_nnp_A();   /*LD (nn), A (size: 3)*/
        case Z80_OPCODE_XPQZ(0, 3, 1, 2): return z80_op_LD_A_nnp();   /*LD A, (nn) (size: 3)*/
        case Z80_OPCODE_XYZ(3, 0, 2): return z80_op_JP_cc_nn();        /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 0, 3): return z80_op_JP_nn();               /*JP nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 0, 4): return z80_op_CALL_cc_nn();    /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 1, 2): return z80_op_JP_cc_nn();        /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 1, 4): return z80_op_CALL_cc_nn();    /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 0, 1, 5): return z80_op_CALL_nn();       /*CALL nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 2, 2): return z80_op_JP_cc_nn();        /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 2, 4): return z80_op_CALL_cc_nn();    /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 3, 2): return z80_op_JP_cc_nn();        /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 3, 4): return z80_op_CALL_cc_nn();    /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 4, 2): return z80_op_JP_cc_nn();        /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 4, 4): return z80_op_CALL_cc_nn();    /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 5, 2): return z80_op_JP_cc_nn();        /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 5, 4): return z80_op_CALL_cc_nn();    /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 6, 2): return z80_op_JP_cc_nn();        /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 6, 4): return z80_op_CALL_cc_nn();    /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 7, 2): return z80_op_JP_cc_nn();        /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 7, 4): return z80_op_CALL_cc_nn();    /*CALL cc, nn (size: 3)*/

        default:
            assert(0); //Should never get here.
            return Z80_STAGE_RESET;
        }

    case 4: //Fourth opcode byte
        //Test prefixes
        if ((z80.opcode[0] == 0xDD) && (z80.opcode[1] == 0xCB))
            return z80_decode_DDCB_FDCB();
        else if ((z80.opcode[0] == 0xFD) && (z80.opcode[1] == 0xCB))
            return z80_decode_DDCB_FDCB();
        else if (z80.opcode[0] == 0xED)
            return z80_decode_ED();
        else{
            //Should never get here
            assert(0);
            return Z80_STAGE_RESET;
        }
    }
    //Opcode_index > 4
    assert(0); //Opcode overflow!
    return Z80_STAGE_RESET;
}
