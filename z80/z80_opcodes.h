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

/* -------------------------------------------- */
/* --- Unprefixed and 0xCB prefixed opcodes --- */
/* -------------------------------------------- */

int z80_op_ADC_HL_rp();
int z80_op_ADC_n();
int z80_op_ADC_r(uint8_t prefixed);
int z80_op_ADD_HL_rp(uint8_t prefixed);
int z80_op_ADD_n();
int z80_op_ADD_r(uint8_t prefixed);
int z80_op_AND_n();
int z80_op_AND_r(uint8_t prefixed);
int z80_op_BIT_y_r();
int z80_op_CALL_cc_nn();
int z80_op_CALL_nn();
int z80_op_CCF();
int z80_op_CPD();
int z80_op_CPDR();
int z80_op_CPI();
int z80_op_CPIR();
int z80_op_CPL();
int z80_op_CP_n();
int z80_op_CP_r(uint8_t prefixed);
int z80_op_DAA();
int z80_op_DEC_r(uint8_t prefixed);
int z80_op_DEC_rp(uint8_t prefixed);
int z80_op_DI();
int z80_op_DJNZ_d();
int z80_op_EI();
int z80_op_EX();
int z80_op_EXX();
int z80_op_EX_DE_HL();
int z80_op_EX_SPp_HL();
int z80_op_HALT();
int z80_op_IM();
int z80_op_INC_HLp(uint8_t prefixed); ///<-- @note merge into INC r
int z80_op_INC_r(uint8_t prefixed);
int z80_op_INC_rp(uint8_t prefixed);
int z80_op_IND();
int z80_op_INDR();
int z80_op_INI();
int z80_op_INIR();
int z80_op_IN_A_np();
int z80_op_IN_Cp();
int z80_op_IN_r_Cp(); 
int z80_op_JP_cc_nn();
int z80_op_JP_HL();
int z80_op_JP_nn();
int z80_op_JR_cc_d();
int z80_op_JR_d();
int z80_op_LDD();
int z80_op_LDDR();
int z80_op_LDI();
int z80_op_LDIR();
int z80_op_LD_A_BCp();
int z80_op_LD_A_DEp();
int z80_op_LD_A_I();
int z80_op_LD_A_nnp();
int z80_op_LD_A_R();
int z80_op_LD_BCp_A();
int z80_op_LD_DEp_A();
int z80_op_LD_HL_nnp();
int z80_op_LD_I_A();
int z80_op_LD_nnp_A();
int z80_op_LD_nnp_HL();
int z80_op_LD_nnp_rp();
int z80_op_LD_rp_nn(uint8_t prefixed);
int z80_op_LD_rp_nnp();
int z80_op_LD_R_A();
int z80_op_LD_r_n(uint8_t prefixed);
int z80_op_LD_r_r(uint8_t prefixed);
int z80_op_LD_SP_HL();
int z80_op_NEG();
int z80_op_NOP();
int z80_op_OR_n();
int z80_op_OR_r(uint8_t prefixed);
int z80_op_OTDR();
int z80_op_OTIR();
int z80_op_OUTD();
int z80_op_OUTI();
int z80_op_OUT_Cp_0();
int z80_op_OUT_Cp_r();
int z80_op_OUT_np_A();
int z80_op_POP_rp2(uint8_t prefixed);
int z80_op_PUSH_rp2(uint8_t prefixed);
int z80_op_RES_y_r();
int z80_op_RET();
int z80_op_RETI();
int z80_op_RETN();
int z80_op_RET_cc();
int z80_op_RLA();
int z80_op_RLCA();
int z80_op_RLC_r(uint8_t prefixed);
int z80_op_RLD();
int z80_op_RL_r(uint8_t prefixed);
int z80_op_RRA();
int z80_op_RRCA();
int z80_op_RRC_r(uint8_t prefixed);
int z80_op_RRD();
int z80_op_RR_r(uint8_t prefixed);
int z80_op_RST_y();
int z80_op_SBC_HL_rp(); /*Can't be prefixed*/
int z80_op_SBC_n();
int z80_op_SBC_r(uint8_t prefixed);
int z80_op_SCF();
int z80_op_SET_y_r();
int z80_op_SLA_r(uint8_t prefixed);
int z80_op_SLL_r(uint8_t prefixed);
int z80_op_SRA_r(uint8_t prefixed);
int z80_op_SRL_r(uint8_t prefixed);
int z80_op_SUB_n();
int z80_op_SUB_r(uint8_t prefixed);
int z80_op_XOR_n();
int z80_op_XOR_r(uint8_t prefixed);

/* ---------------------------------- */
/* --- 0xDD/0xFD prefixed opcodes --- */
/* ---------------------------------- */

// Empty

/* -------------------------------------- */
/* --- 0xDDCB/0xFDCB prefixed opcodes --- */
/* -------------------------------------- */

// Empty

#endif
