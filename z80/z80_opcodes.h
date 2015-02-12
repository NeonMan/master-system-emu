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

int z80_op_ADC_n();
int z80_op_ADC_r();
int z80_op_ADD_HL_rp();
int z80_op_ADD_n();
int z80_op_ADD_r();
int z80_op_AND_n();
int z80_op_AND_r();
int z80_op_BIT_y_r();
int z80_op_CALL_cc_nn();
int z80_op_CALL_nn();
int z80_op_CCF();
int z80_op_CP_n();
int z80_op_CP_r();
int z80_op_CPL();
int z80_op_DAA();
int z80_op_DEC_r();
int z80_op_DEC_rp();
int z80_op_DI();
int z80_op_DJNZ_d();
int z80_op_EI();
int z80_op_EX();
int z80_op_EX_DE_HL();
int z80_op_EX_SPp_HL();
int z80_op_EXX();
int z80_op_HALT();
int z80_op_IM();
int z80_op_IN_A_np();
int z80_op_INC_HLp();
int z80_op_INC_r();
int z80_op_INC_rp();
int z80_op_JP_nn();
int z80_op_JP_cc_nn();
int z80_op_JP_HL();
int z80_op_JR_cc_d();
int z80_op_JR_d();
int z80_op_LD_A_BCp();
int z80_op_LD_A_DEp();
int z80_op_LD_A_nnp();
int z80_op_LD_BCp_A();
int z80_op_LD_HL_nnp();
int z80_op_LD_DEp_A();
int z80_op_LD_r_n();
int z80_op_LD_r_r();
int z80_op_LD_rp_nn();
int z80_op_LD_SP_HL();
int z80_op_LD_nnp_A();
int z80_op_LD_nnp_HL();
int z80_op_LDIR();
int z80_op_NOP();
int z80_op_OR_n();
int z80_op_OR_r();
int z80_op_OTIR();
int z80_op_OUT_np_A();
int z80_op_POP_rp2();
int z80_op_PUSH_rp2();
int z80_op_RES_y_r();
int z80_op_RET();
int z80_op_RET_cc();
int z80_op_RLA();
int z80_op_RLCA();
int z80_op_RRA();
int z80_op_RRCA();
int z80_op_RST_y();
int z80_op_SBC_HL_rp();
int z80_op_SBC_n();
int z80_op_SBC_r();
int z80_op_SCF();
int z80_op_SRL_r();
int z80_op_SUB_n();
int z80_op_SUB_r();
int z80_op_XOR_n();
int z80_op_XOR_r();

#endif
