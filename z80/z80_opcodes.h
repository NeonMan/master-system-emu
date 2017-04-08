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

#ifndef __Z80_OPCODES_H
#define __Z80_OPCODES_H

#include "z80.h"

#ifdef __cplusplus
extern "C" {
#endif

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

/* ------------- */
/* --- Types --- */
/* ------------- */
struct alu_result_s {
    int8_t  result;
    uint8_t flags;
};
typedef struct alu_result_s alu_result_t;

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
int LD_r_r_undoc();

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
int alu8_HLp();
int alu8_n();
int alu8_r();

int DEC_HLp();
int DEC_r();
int INC_HLp();
int INC_r();

/* -------------------------------------- */
/* --- 8bit arithmetic (undocumented) --- */
/* -------------------------------------- */
int INC_rIXY();
int DEC_rIXY();
int alu8_r_undoc();

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
int shift_rot_IXYp();
int shift_rot_HLp();
int shift_rot_r();

int RLA();
int RLCA();
int RLD();
int RRA();
int RRCA();
int RRD();

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
int alu8_IXYp();

int ADD_IXY_rp();
int LD_IXY_nn();
int LD_nnp_IXY();
int INC_IXY();
int INC_IXYp();
int LD_IXY_nnp();
int DEC_IXY();
int DEC_IXYp();
int LD_IXYp_n();
int LD_r_IXYp();
int LD_IXYp_r();
int POP_IXY();
int EX_SPp_IXY();
int PUSH_IXY();
int JP_IXYp();
int LD_SP_IXY();

int SET_b_IXYp();
int RES_b_IXYp();
int BIT_b_IXYp();

/* --- Dummy opcode for testing a debugging purposes --- */
/* --- Calling this will make an assertion failure   --- */
int UNDOC();

/* --- Exposed internals, just for unit testing --- */
alu_result_t alu8_op(uint8_t operation, int8_t op1, int8_t op2, uint8_t flags);
alu_result_t alu8_inc(uint8_t op, uint8_t flags);
alu_result_t alu8_dec(uint8_t op, uint8_t flags);
alu_result_t op_rotate_shift(uint8_t operation, uint8_t op, uint8_t flags);

#ifdef __cplusplus
}
#endif

#endif
