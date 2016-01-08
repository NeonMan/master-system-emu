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

#ifndef __Z80_OPCODES_H
#define __Z80_OPCODES_H

#include "z80.h"

/* -------------------- */
/* --- Handy macros --- */
/* -------------------- */

#define Z80_8BIT_READ(ADDRESS, IO) {\
    if(z80.read_index == 0){\
        z80.read_is_io = (IO);\
        z80.read_address = (ADDRESS);\
        return Z80_STAGE_M2;\
    }\
}

#define Z80_16BIT_READ(ADDRESS, IO) {\
    if(z80.read_index == 0){\
        z80.read_is_io = (IO);\
        z80.read_address = (ADDRESS);\
        return Z80_STAGE_M2;\
    }else if(z80.read_index == 1){\
        ++z80.read_address;\
        return Z80_STAGE_M2;\
    }\
}

#define Z80_8BIT_WRITE(ADDRESS, IO, B0) {\
    if(z80.write_index==0){\
        z80.write_is_io = (IO);\
        z80.write_address = (ADDRESS);\
        z80.write_buffer[0] = (B0);\
        return Z80_STAGE_M3;\
        }\
}

#define Z80_16BIT_WRITE(ADDRESS, IO, B0, B1) {\
    if(z80.write_index==0){\
        z80.write_is_io = (IO);\
        z80.write_address = (ADDRESS);\
        z80.write_buffer[0] = (B0);\
        z80.write_buffer[1] = (B1);\
        return Z80_STAGE_M3;\
    }\
    else if(z80.write_index == 1){\
        ++z80.write_address;\
        return Z80_STAGE_M3;\
    }\
}

/* ------------------ */
/* --- 8-bit Load --- */
/* ------------------ */
int LD_A_BCp();
int LD_A_DEp();
int LD_A_I();
int LD_A_nnp();
int LD_A_R();
int LD_BCp_A();
int LD_DEp_A();
int LD_HL_nnp();
int LD_HLp_n();
int LD_HLp_r();
int LD_I_A();
int LD_nnp_A();
int LD_R_A();
int LD_r_HLp();
int LD_r_n();
int LD_r_r();

/* ------------------------------- */
/* --- Undocumented 8-bit Load --- */
/* ------------------------------- */
int LD_IXYH_n();
int LD_IXYL_n();

/* ------------------- */
/* --- 16-bit Load --- */
/* ------------------- */
int LD_nnp_HL();
int LD_nnp_rp();
int LD_rp_nn();
int LD_rp_nnp();
int LD_SP_HL();
int POP_rp2();
int PUSH_rp2();

/* ------------------------------------------- */
/* --- Exchange, block transfer and search --- */
/* ------------------------------------------- */
int CPD();
int CPDR();
int CPI();
int CPIR();
int EX();
int EX_DE_HL();
int EX_SPp_HL();
int EXX();
int LDD();
int LDDR();
int LDI();
int LDIR();

/* ------------------------ */
/* --- 8-bit arithmetic --- */
/* ------------------------ */
int ADC_HLp();
int ADC_n();
int ADC_r();
int ADD_HLp();
int ADD_n();
int ADD_r();
int AND_HLp();
int AND_n();
int AND_r();
int CP_HLp();
int CP_n();
int CP_r();
int DEC_HLp();
int DEC_r();
int INC_HLp();
int INC_r();
int OR_HLp();
int OR_n();
int OR_r();
int SBC_HLp();
int SBC_n();
int SBC_r();
int SUB_HLp();
int SUB_n();
int SUB_r();
int XOR_HLp();
int XOR_n();
int XOR_r();

/* ---------------------------------------------- */
/* --- General purpose arithmetic and control --- */
/* ---------------------------------------------- */
int CCF();
int CPL();
int DAA();
int DI();
int EI();
int HALT();
int IM();
int NEG();
int NOP();
int SCF();

/* ------------------------- */
/* --- 16-bit Arithmetic --- */
/* ------------------------- */
int ADC_HL_rp();
int ADD_HL_rp();
int AND_HL_rp();
int CP_HL_rp();
int DEC_rp();
int INC_rp();
int SBC_HL_rp();

/* ------------------------ */
/* --- Rotate and shift --- */
/* ------------------------ */
int RL_HLp();
int RL_r();
int RLA();
int RLC_HLp();
int RLC_r();
int RLCA();
int RLD();
int RR_HLp();
int RR_r();
int RRA();
int RRC_HLp();
int RRC_r();
int RRCA();
int RRD();
int SLA_HLp();
int SLA_r();
int SLL_HLp();
int SLL_r();
int SRA_HLp();
int SRA_r();
int SRL_HLp();
int SRL_r();

/* ------------------------------- */
/* --- Bit set, reset and test --- */
/* ------------------------------- */
int BIT_b_HLp();
int BIT_b_r();
int RES_b_HLp();
int RES_b_r();
int SET_b_HLp();
int SET_b_r();

/* ------------ */
/* --- Jump --- */
/* ------------ */
int DJNZ_d();
int JP_cc_nn();
int JP_HLp();
int JP_nn();
int JR_cc_d();
int JR_d();

/* ----------------------- */
/* --- Call and return --- */
/* ----------------------- */
int CALL_cc_nn();
int CALL_nn();
int RET();
int RET_cc();
int RETI();
int RETN();
int RST_y();

/* -------------------- */
/* --- Input/Output --- */
/* -------------------- */
int IN_A_np();
int IN_Cp();
int IN_r_Cp();
int IND();
int INDR();
int INI();
int INIR();
int OTDR();
int OTIR();
int OUT_Cp_0();
int OUT_Cp_r();
int OUT_np_A();
int OUTD();
int OUTI();

/* --------------------------------------------- */
/* --- (IX+n)/(IY+n)/IX/IY verion of opcodes --- */
/* --------------------------------------------- */

int ADD_IXY_rp();
int LD_IXY_nn();
int LD_nnp_IXY();
int INC_IXY();
int INC_IXYp();
int ADD_IXY_rp();
int LD_IXY_nnp();
int DEC_IXY();
int DEC_IXYp();
int LD_IXYp_n();
int LD_r_IXYp();
int LD_IXYp_r();
int ADD_IXYp();
int ADC_IXYp();
int SUB_IXYp();
int SBC_IXYp();
int AND_IXYp();
int XOR_IXYp();
int OR_IXYp();
int CP_IXYp();
int POP_IXY();
int EX_SPp_IXY();
int PUSH_IXY();
int JP_IXYp();
int LD_SP_IXY();
int SLL_IXYp();
int RRC_IXYp();
int RL_IXYp();
int SLA_IXYp();
int RR_IXYp();
int SRA_IXYp();
int SRL_IXYp();
int RR_IXYp();
int SRA_IXYp();
int SRL_IXYp();
int RLC_IXYp();

int SET_b_IXYp();
int RES_b_IXYp();
int BIT_b_IXYp();

/* --- Dummy opcode for testing a debugging purposes --- */
/* --- Calling this will make an assertion failure   --- */
int UNDOC();

#endif
