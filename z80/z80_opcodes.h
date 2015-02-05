#ifndef __Z80_OPCODES_H
#define __Z80_OPCODES_H

#include "z80.h"

int z80_op_ADD_HL_rp();
int z80_op_ADD_n();
int z80_op_CCF();
int z80_op_CPL();
int z80_op_DEC_r();
int z80_op_DEC_rp();
int z80_op_EX();
int z80_op_INC_HLp();
int z80_op_INC_r();
int z80_op_INC_rp();
int z80_op_LD_A_DEp();
int z80_op_LD_BCp_A();
int z80_op_LD_DEp_A();
int z80_op_LD_r_HLp();
int z80_op_LD_r_r();
int z80_op_NOP();
int z80_op_RLA();
int z80_op_RLCA();
int z80_op_RRA();
int z80_op_RRCA();
int z80_op_SCF();

#endif