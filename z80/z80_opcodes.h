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

int ADC_HL_rp();
int ADC_n();
int ADC_r();
int ADC_HLp();
int ADD_HL_rp();
int ADD_n();
int ADD_r();
int ADD_HLp();
int AND_HL_rp();
int AND_n();
int AND_r();
int AND_HLp();
int BIT_b_r();
int BIT_b_HLp();
int CALL_cc_nn();
int CALL_nn();
int CCF();
int CPD();
int CPDR();
int CPI();
int CPIR();
int CPL();
int CP_n();
int CP_r();
int CP_HLp();
int CP_HL_rp();
int DAA();
int DEC_r();
int DEC_HLp();
int DEC_rp();
int DI();
int DJNZ_d();
int EI();
int EX();
int EXX();
int EX_DE_HL();
int EX_SPp_HL();
int HALT();
int IM();
int INC_HLp();
int INC_r();
int INC_rp();
int IND();
int INDR();
int INI();
int INIR();
int IN_A_np();
int IN_Cp();
int IN_r_Cp(); 
int JP_cc_nn();
int JP_HLp();
int JP_nn();
int JR_cc_d();
int JR_d();
int LDD();
int LDDR();
int LDI();
int LDIR();
int LD_A_BCp();
int LD_A_DEp();
int LD_A_I();
int LD_A_nnp();
int LD_A_R();
int LD_BCp_A();
int LD_DEp_A();
int LD_HL_nnp();
int LD_HLp_n();
int LD_I_A();
int LD_nnp_A();
int LD_nnp_HL();
int LD_nnp_rp();
int LD_rp_nn();
int LD_rp_nnp();
int LD_R_A();
int LD_r_n();
int LD_r_r();
int LD_r_HLp();
int LD_HLp_r();
int LD_SP_HL();
int NEG();
int NOP();
int OR_HLp();
int OR_n();
int OR_r();
int OTDR();
int OTIR();
int OUTD();
int OUTI();
int OUT_Cp_0();
int OUT_Cp_r();
int OUT_np_A();
int POP_rp2();
int PUSH_rp2();
int RES_b_r();
int RES_b_HLp();
int RET();
int RETI();
int RETN();
int RET_cc();
int RLA();
int RLCA();
int RLC_r();
int RLD();
int RL_HLp();
int RL_r();
int RRA();
int RRCA();
int RRC_HLp();
int RRC_r();
int RRD();
int RR_HLp();
int RR_r();
int RST_y();
int SBC_HL_rp(); /*Can't be prefixed*/
int SBC_HLp();
int SBC_n();
int SBC_r();
int SCF();
int SET_b_r();
int SET_b_HLp();
int SLA_HLp();
int SLA_r();
int SLL_HLp();
int SLL_r();
int SLLA();
int SRA_HLp();
int SRA_r();
int SRL_HLp();
int SRL_r();
int SUB_HLp();
int SUB_n();
int SUB_r();
int XOR_HLp();
int XOR_n();
int XOR_r();
#endif
