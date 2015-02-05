#ifndef __Z80_OPCODES_H
#define __Z80_OPCODES_H

#include "z80.h"

int z80_op_ADD_HL_rp();
int z80_op_ADD_n();
int z80_op_LD_A_DE();
int z80_op_LD_BC_A();
int z80_op_LD_DE_A();
int z80_op_NOP();
int z80_op_EX();

#endif