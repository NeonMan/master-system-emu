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

/**
* @file z80_dasm.h
* @brief Provides functions to decode z80 opcodes into human-readable text.
*
*/
#ifndef __Z80_DASM
#define __Z80_DASM

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Contains all the info regarding an opcode at a given address.
struct z80d_opcode_s{
    char     opcode_str[32]; ///<-- Mnemonic representing the opcode
    uint8_t  opcode[4];      ///<-- The opcode bytes
    uint8_t  size;           ///<-- Opcode size
    uint16_t address;        ///<-- Opcode address

    ///Pointer to an opcode at the same address (if bank switching is used)
    struct z80d_opcode_s *next;
};
typedef struct z80d_opcode_s z80d_opcode;

/**
* Decodes an opcode, writing a sring representing the opcode.
* 
* @param opcode A 4-byte array containing the opcode.
* @param size   maximum size of the result string.
* @param result pointer to the string where the result will be written.
* @returns the opcode size in bytes.
*/
int z80d_decode(const uint8_t* opcode, unsigned int size, char* result);

/**
 * Decodes an opcode, return an z80d_opcode struct.
 * 
 * @param opcode A 4-byte array containing the opcode.
 * @param pc_addr The address where the opcode was found (PC).
 * @returns A z80_opcode struct containg all the opcode info.
 */
z80d_opcode z80d_decode_op(const uint8_t* opcode, uint16_t pc_addr);

/* --- Disasm functions --- */
int zd_ADC_HL_rp(const uint8_t* opcode, char* result);
int zd_ADD_HL_rp(const uint8_t* opcode, char* result);
int zd_ADD_IXY_rp(const uint8_t* opcode, char* result);
int zd_ADD_IXY_rp(const uint8_t* opcode, char* result);
int zd_alu_IXYp(const uint8_t* opcode, char* result);
int zd_alu_IXYp(const uint8_t* opcode, char* result);
int zd_alu_n(const uint8_t* opcode, char* result);
int zd_alu_r(const uint8_t* opcode, char* result);
int zd_BIT_b_IXYp(const uint8_t* opcode, char* result);
int zd_BIT_b_IXYp(const uint8_t* opcode, char* result);
int zd_BIT_b_r(const uint8_t* opcode, char* result);
int zd_BIT_b_r(const uint8_t* opcode, char* result);
int zd_BIT_b_r(const uint8_t* opcode, char* result);
int zd_bli(const uint8_t* opcode, char* result);
int zd_CALL_cc_nn(const uint8_t* opcode, char* result);
int zd_CALL_nn(const uint8_t* opcode, char* result);
int zd_CCF(const uint8_t* opcode, char* result);
int zd_CPL(const uint8_t* opcode, char* result);
int zd_DAA(const uint8_t* opcode, char* result);
int zd_DEC_IXY(const uint8_t* opcode, char* result);
int zd_DEC_IXY(const uint8_t* opcode, char* result);
int zd_DEC_IXYp(const uint8_t* opcode, char* result);
int zd_DEC_IXYp(const uint8_t* opcode, char* result);
int zd_DEC_r(const uint8_t* opcode, char* result);
int zd_DEC_rp(const uint8_t* opcode, char* result);
int zd_DI(const uint8_t* opcode, char* result);
int zd_DJNZ_d(const uint8_t* opcode, char* result);
int zd_EI(const uint8_t* opcode, char* result);
int zd_EX(const uint8_t* opcode, char* result);
int zd_EX_DE_HL(const uint8_t* opcode, char* result);
int zd_EX_SPp_HL(const uint8_t* opcode, char* result);
int zd_EX_SPp_IXY(const uint8_t* opcode, char* result);
int zd_EX_SPp_IXY(const uint8_t* opcode, char* result);
int zd_EXX(const uint8_t* opcode, char* result);
int zd_HALT(const uint8_t* opcode, char* result);
int zd_HALT(const uint8_t* opcode, char* result);
int zd_HALT(const uint8_t* opcode, char* result);
int zd_IM(const uint8_t* opcode, char* result);
int zd_IM(const uint8_t* opcode, char* result);
int zd_IM(const uint8_t* opcode, char* result);
int zd_IN_A_np(const uint8_t* opcode, char* result);
int zd_IN_r_Cp(const uint8_t* opcode, char* result);
int zd_INC_IXY(const uint8_t* opcode, char* result);
int zd_INC_IXY(const uint8_t* opcode, char* result);
int zd_INC_IXYp(const uint8_t* opcode, char* result);
int zd_INC_IXYp(const uint8_t* opcode, char* result);
int zd_INC_r(const uint8_t* opcode, char* result);
int zd_INC_rp(const uint8_t* opcode, char* result);
int zd_JP_cc_nn(const uint8_t* opcode, char* result);
int zd_JP_HLp(const uint8_t* opcode, char* result);
int zd_JP_IXYp(const uint8_t* opcode, char* result);
int zd_JP_IXYp(const uint8_t* opcode, char* result);
int zd_JP_nn(const uint8_t* opcode, char* result);
int zd_JR_cc_d(const uint8_t* opcode, char* result);
int zd_JR_cc_d(const uint8_t* opcode, char* result);
int zd_JR_cc_d(const uint8_t* opcode, char* result);
int zd_JR_cc_d(const uint8_t* opcode, char* result);
int zd_JR_d(const uint8_t* opcode, char* result);
int zd_LD_A_BCp(const uint8_t* opcode, char* result);
int zd_LD_A_DEp(const uint8_t* opcode, char* result);
int zd_LD_A_I(const uint8_t* opcode, char* result);
int zd_LD_A_nnp(const uint8_t* opcode, char* result);
int zd_LD_A_R(const uint8_t* opcode, char* result);
int zd_LD_BCp_A(const uint8_t* opcode, char* result);
int zd_LD_DEp_A(const uint8_t* opcode, char* result);
int zd_LD_HL_nnp(const uint8_t* opcode, char* result);
int zd_LD_I_A(const uint8_t* opcode, char* result);
int zd_LD_IXY_nn(const uint8_t* opcode, char* result);
int zd_LD_IXY_nn(const uint8_t* opcode, char* result);
int zd_LD_IXY_nnp(const uint8_t* opcode, char* result);
int zd_LD_IXY_nnp(const uint8_t* opcode, char* result);
int zd_LD_IXYH_n(const uint8_t* opcode, char* result);
int zd_LD_IXYH_n(const uint8_t* opcode, char* result);
int zd_LD_IXYL_n(const uint8_t* opcode, char* result);
int zd_LD_IXYL_n(const uint8_t* opcode, char* result);
int zd_LD_IXYp_n(const uint8_t* opcode, char* result);
int zd_LD_IXYp_n(const uint8_t* opcode, char* result);
int zd_LD_IXYp_r(const uint8_t* opcode, char* result);
int zd_LD_IXYp_r(const uint8_t* opcode, char* result);
int zd_LD_nnp_A(const uint8_t* opcode, char* result);
int zd_LD_nnp_HL(const uint8_t* opcode, char* result);
int zd_LD_nnp_IXY(const uint8_t* opcode, char* result);
int zd_LD_nnp_IXY(const uint8_t* opcode, char* result);
int zd_LD_nnp_rp(const uint8_t* opcode, char* result);
int zd_LD_R_A(const uint8_t* opcode, char* result);
int zd_LD_r_IXYp(const uint8_t* opcode, char* result);
int zd_LD_r_IXYp(const uint8_t* opcode, char* result);
int zd_LD_r_n(const uint8_t* opcode, char* result);
int zd_LD_r_r(const uint8_t* opcode, char* result);
int zd_LD_RES(const uint8_t* opcode, char* result);
int zd_LD_RES(const uint8_t* opcode, char* result);
int zd_LD_ROT(const uint8_t* opcode, char* result);
int zd_LD_ROT(const uint8_t* opcode, char* result);
int zd_LD_rp_nn(const uint8_t* opcode, char* result);
int zd_LD_rp_nnp(const uint8_t* opcode, char* result);
int zd_LD_SET(const uint8_t* opcode, char* result);
int zd_LD_SET(const uint8_t* opcode, char* result);
int zd_LD_SP_HL(const uint8_t* opcode, char* result);
int zd_LD_SP_IXY(const uint8_t* opcode, char* result);
int zd_LD_SP_IXY(const uint8_t* opcode, char* result);
int zd_NEG(const uint8_t* opcode, char* result);
int zd_NOP(const uint8_t* opcode, char* result);
int zd_NOP(const uint8_t* opcode, char* result);
int zd_NOP(const uint8_t* opcode, char* result);
int zd_NOP(const uint8_t* opcode, char* result);
int zd_OUT_Cp_r(const uint8_t* opcode, char* result);
int zd_OUT_np_A(const uint8_t* opcode, char* result);
int zd_POP_IXY(const uint8_t* opcode, char* result);
int zd_POP_IXY(const uint8_t* opcode, char* result);
int zd_POP_rp2(const uint8_t* opcode, char* result);
int zd_PUSH_IXY(const uint8_t* opcode, char* result);
int zd_PUSH_IXY(const uint8_t* opcode, char* result);
int zd_PUSH_rp2(const uint8_t* opcode, char* result);
int zd_RES_b_IXYp(const uint8_t* opcode, char* result);
int zd_RES_b_IXYp(const uint8_t* opcode, char* result);
int zd_RES_b_r(const uint8_t* opcode, char* result);
int zd_RET(const uint8_t* opcode, char* result);
int zd_RET_cc(const uint8_t* opcode, char* result);
int zd_RETI(const uint8_t* opcode, char* result);
int zd_RETN(const uint8_t* opcode, char* result);
int zd_RLA(const uint8_t* opcode, char* result);
int zd_RLCA(const uint8_t* opcode, char* result);
int zd_RLD(const uint8_t* opcode, char* result);
int zd_rot(const uint8_t* opcode, char* result);
int zd_rot_IXYp(const uint8_t* opcode, char* result);
int zd_rot_IXYp(const uint8_t* opcode, char* result);
int zd_RRA(const uint8_t* opcode, char* result);
int zd_RRCA(const uint8_t* opcode, char* result);
int zd_RRD(const uint8_t* opcode, char* result);
int zd_RST_y(const uint8_t* opcode, char* result);
int zd_SBC_HL_rp(const uint8_t* opcode, char* result);
int zd_SCF(const uint8_t* opcode, char* result);
int zd_SET_b_IXYp(const uint8_t* opcode, char* result);
int zd_SET_b_IXYp(const uint8_t* opcode, char* result);
int zd_SET_b_r(const uint8_t* opcode, char* result);



#ifdef __cplusplus
}
#endif

#endif