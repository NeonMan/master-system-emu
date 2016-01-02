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
#ifndef __Z80_DASM_TABLES
#define __Z80_DASM_TABLES

// --------------------------------------------------
// ---          DO NOT ALTER THIS FILE!!          ---
// --------------------------------------------------
// --- Disasm tables are automatically generated  ---
// --- by 'z80_make_decoder_tables.py'            ---
// --------------------------------------------------

#include "z80_dasm.h"
#include <stdint.h>

struct opcode_dasm_s{
  int (*f)(const uint8_t *, char*);
  signed char size;
};
typedef struct opcode_dasm_s opcode_dasm_t;

static const opcode_dasm_t op_unpref[256] = {
/*                    0                  1                  2                  3                  4                  5                  6                  7*/
/*00*/       {zd_NOP,1},   {zd_LD_rp_nn,3},   {zd_LD_BCp_A,1},     {zd_INC_rp,1},      {zd_INC_r,1},      {zd_DEC_r,1},     {zd_LD_r_n,2},       {zd_RLCA,1}, 
/*08*/        {zd_EX,1},  {zd_ADD_HL_rp,1},   {zd_LD_A_BCp,1},     {zd_DEC_rp,1},      {zd_INC_r,1},      {zd_DEC_r,1},     {zd_LD_r_n,2},       {zd_RRCA,1}, 
/*10*/    {zd_DJNZ_d,2},   {zd_LD_rp_nn,3},   {zd_LD_DEp_A,1},     {zd_INC_rp,1},      {zd_INC_r,1},      {zd_DEC_r,1},     {zd_LD_r_n,2},        {zd_RLA,1}, 
/*18*/      {zd_JR_d,2},  {zd_ADD_HL_rp,1},   {zd_LD_A_DEp,1},     {zd_DEC_rp,1},      {zd_INC_r,1},      {zd_DEC_r,1},     {zd_LD_r_n,2},        {zd_RRA,1}, 
/*20*/   {zd_JR_cc_d,2},   {zd_LD_rp_nn,3},  {zd_LD_nnp_HL,3},     {zd_INC_rp,1},      {zd_INC_r,1},      {zd_DEC_r,1},     {zd_LD_r_n,2},        {zd_DAA,1}, 
/*28*/   {zd_JR_cc_d,2},  {zd_ADD_HL_rp,1},  {zd_LD_HL_nnp,3},     {zd_DEC_rp,1},      {zd_INC_r,1},      {zd_DEC_r,1},     {zd_LD_r_n,2},        {zd_CPL,1}, 
/*30*/   {zd_JR_cc_d,2},   {zd_LD_rp_nn,3},   {zd_LD_nnp_A,3},     {zd_INC_rp,1},      {zd_INC_r,1},      {zd_DEC_r,1},     {zd_LD_r_n,2},        {zd_SCF,1}, 
/*38*/   {zd_JR_cc_d,2},  {zd_ADD_HL_rp,1},   {zd_LD_A_nnp,3},     {zd_DEC_rp,1},      {zd_INC_r,1},      {zd_DEC_r,1},     {zd_LD_r_n,2},        {zd_CCF,1}, 
/*40*/    {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1}, 
/*48*/    {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1}, 
/*50*/    {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1}, 
/*58*/    {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1}, 
/*60*/    {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1}, 
/*68*/    {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1}, 
/*70*/    {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},       {zd_HALT,1},     {zd_LD_r_r,1}, 
/*78*/    {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1},     {zd_LD_r_r,1}, 
/*80*/     {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1}, 
/*88*/     {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1}, 
/*90*/     {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1}, 
/*98*/     {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1}, 
/*A0*/     {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1}, 
/*A8*/     {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1}, 
/*B0*/     {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1}, 
/*B8*/     {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1},      {zd_alu_r,1}, 
/*C0*/    {zd_RET_cc,1},    {zd_POP_rp2,1},   {zd_JP_cc_nn,3},      {zd_JP_nn,3}, {zd_CALL_cc_nn,3},   {zd_PUSH_rp2,1},      {zd_alu_n,2},      {zd_RST_y,1}, 
/*C8*/    {zd_RET_cc,1},        {zd_RET,1},   {zd_JP_cc_nn,3},             {0,0}, {zd_CALL_cc_nn,3},    {zd_CALL_nn,3},      {zd_alu_n,2},      {zd_RST_y,1}, 
/*D0*/    {zd_RET_cc,1},    {zd_POP_rp2,1},   {zd_JP_cc_nn,3},   {zd_OUT_np_A,2}, {zd_CALL_cc_nn,3},   {zd_PUSH_rp2,1},      {zd_alu_n,2},      {zd_RST_y,1}, 
/*D8*/    {zd_RET_cc,1},        {zd_EXX,1},   {zd_JP_cc_nn,3},    {zd_IN_A_np,2}, {zd_CALL_cc_nn,3},             {0,0},      {zd_alu_n,2},      {zd_RST_y,1}, 
/*E0*/    {zd_RET_cc,1},    {zd_POP_rp2,1},   {zd_JP_cc_nn,3},  {zd_EX_SPp_HL,1}, {zd_CALL_cc_nn,3},   {zd_PUSH_rp2,1},      {zd_alu_n,2},      {zd_RST_y,1}, 
/*E8*/    {zd_RET_cc,1},     {zd_JP_HLp,1},   {zd_JP_cc_nn,3},   {zd_EX_DE_HL,1}, {zd_CALL_cc_nn,3},             {0,0},      {zd_alu_n,2},      {zd_RST_y,1}, 
/*F0*/    {zd_RET_cc,1},    {zd_POP_rp2,1},   {zd_JP_cc_nn,3},         {zd_DI,1}, {zd_CALL_cc_nn,3},   {zd_PUSH_rp2,1},      {zd_alu_n,2},      {zd_RST_y,1}, 
/*F8*/    {zd_RET_cc,1},   {zd_LD_SP_HL,1},   {zd_JP_cc_nn,3},         {zd_EI,1}, {zd_CALL_cc_nn,3},             {0,0},      {zd_alu_n,2},      {zd_RST_y,1}
};
static const opcode_dasm_t op_cb[256] = {
/*                    0                  1                  2                  3                  4                  5                  6                  7*/
/*00*/       {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2}, 
/*08*/       {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2}, 
/*10*/       {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2}, 
/*18*/       {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2}, 
/*20*/       {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2}, 
/*28*/       {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2}, 
/*30*/       {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2}, 
/*38*/       {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2},        {zd_rot,2}, 
/*40*/   {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2}, 
/*48*/   {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2}, 
/*50*/   {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2}, 
/*58*/   {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2}, 
/*60*/   {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2}, 
/*68*/   {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2}, 
/*70*/   {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2}, 
/*78*/   {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2},    {zd_BIT_b_r,2}, 
/*80*/   {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2}, 
/*88*/   {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2}, 
/*90*/   {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2}, 
/*98*/   {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2}, 
/*A0*/   {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2}, 
/*A8*/   {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2}, 
/*B0*/   {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2}, 
/*B8*/   {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2},    {zd_RES_b_r,2}, 
/*C0*/   {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2}, 
/*C8*/   {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2}, 
/*D0*/   {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2}, 
/*D8*/   {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2}, 
/*E0*/   {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2}, 
/*E8*/   {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2}, 
/*F0*/   {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2}, 
/*F8*/   {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2},    {zd_SET_b_r,2}
};
static const opcode_dasm_t op_ed[256] = {
/*                    0                  1                  2                  3                  4                  5                  6                  7*/
/*00*/       {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2}, 
/*08*/       {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2}, 
/*10*/       {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2}, 
/*18*/       {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2}, 
/*20*/       {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2}, 
/*28*/       {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2}, 
/*30*/       {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2}, 
/*38*/       {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2}, 
/*40*/   {zd_IN_r_Cp,2},   {zd_OUT_Cp_r,2},  {zd_SBC_HL_rp,2},  {zd_LD_nnp_rp,4},        {zd_NEG,2},       {zd_RETN,2},         {zd_IM,2},     {zd_LD_I_A,2}, 
/*48*/   {zd_IN_r_Cp,2},   {zd_OUT_Cp_r,2},  {zd_ADC_HL_rp,2},  {zd_LD_rp_nnp,4},        {zd_NEG,2},       {zd_RETI,2},         {zd_IM,2},     {zd_LD_R_A,2}, 
/*50*/   {zd_IN_r_Cp,2},   {zd_OUT_Cp_r,2},  {zd_SBC_HL_rp,2},  {zd_LD_nnp_rp,4},        {zd_NEG,2},       {zd_RETN,2},         {zd_IM,2},     {zd_LD_A_I,2}, 
/*58*/   {zd_IN_r_Cp,2},   {zd_OUT_Cp_r,2},  {zd_ADC_HL_rp,2},  {zd_LD_rp_nnp,4},        {zd_NEG,2},       {zd_RETI,2},         {zd_IM,2},     {zd_LD_A_R,2}, 
/*60*/   {zd_IN_r_Cp,2},   {zd_OUT_Cp_r,2},  {zd_SBC_HL_rp,2},  {zd_LD_nnp_rp,4},        {zd_NEG,2},       {zd_RETN,2},         {zd_IM,2},        {zd_RRD,2}, 
/*68*/   {zd_IN_r_Cp,2},   {zd_OUT_Cp_r,2},  {zd_ADC_HL_rp,2},  {zd_LD_rp_nnp,4},        {zd_NEG,2},       {zd_RETI,2},         {zd_IM,2},        {zd_RLD,2}, 
/*70*/   {zd_IN_r_Cp,2},   {zd_OUT_Cp_r,2},  {zd_SBC_HL_rp,2},  {zd_LD_nnp_rp,4},        {zd_NEG,2},       {zd_RETN,2},         {zd_IM,2},        {zd_NOP,2}, 
/*78*/   {zd_IN_r_Cp,2},   {zd_OUT_Cp_r,2},  {zd_ADC_HL_rp,2},  {zd_LD_rp_nnp,4},        {zd_NEG,2},       {zd_RETI,2},         {zd_IM,2},        {zd_NOP,2}, 
/*80*/       {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2}, 
/*88*/       {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2}, 
/*90*/       {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2}, 
/*98*/       {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2}, 
/*A0*/       {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2}, 
/*A8*/       {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2}, 
/*B0*/       {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2}, 
/*B8*/       {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2},        {zd_bli,2}, 
/*C0*/       {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2}, 
/*C8*/       {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2}, 
/*D0*/       {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2}, 
/*D8*/       {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2}, 
/*E0*/       {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2}, 
/*E8*/       {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2}, 
/*F0*/       {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2}, 
/*F8*/       {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2},        {zd_NOP,2}
};
static const opcode_dasm_t op_dd[256] = {
/*                    0                  1                  2                  3                  4                  5                  6                  7*/
/*00*/       {zd_NOP,2},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0}, 
/*08*/            {0,0}, {zd_ADD_IXY_rp,2},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0}, 
/*10*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0}, 
/*18*/            {0,0}, {zd_ADD_IXY_rp,2},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0}, 
/*20*/            {0,0},  {zd_LD_IXY_nn,4}, {zd_LD_nnp_IXY,4},    {zd_INC_IXY,2},             {0,0},             {0,0},  {zd_LD_IXYH_n,3},             {0,0}, 
/*28*/            {0,0}, {zd_ADD_IXY_rp,2}, {zd_LD_IXY_nnp,4},    {zd_DEC_IXY,2},             {0,0},             {0,0},  {zd_LD_IXYL_n,3},             {0,0}, 
/*30*/            {0,0},             {0,0},             {0,0},             {0,0},   {zd_INC_IXYp,3},   {zd_DEC_IXYp,3},  {zd_LD_IXYp_n,4},             {0,0}, 
/*38*/            {0,0}, {zd_ADD_IXY_rp,2},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0}, 
/*40*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},  {zd_LD_r_IXYp,3},             {0,0}, 
/*48*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},  {zd_LD_r_IXYp,3},             {0,0}, 
/*50*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},  {zd_LD_r_IXYp,3},             {0,0}, 
/*58*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},  {zd_LD_r_IXYp,3},             {0,0}, 
/*60*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},  {zd_LD_r_IXYp,3},             {0,0}, 
/*68*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},  {zd_LD_r_IXYp,3},             {0,0}, 
/*70*/ {zd_LD_IXYp_r,3},  {zd_LD_IXYp_r,3},  {zd_LD_IXYp_r,3},  {zd_LD_IXYp_r,3},  {zd_LD_IXYp_r,3},  {zd_LD_IXYp_r,3},       {zd_HALT,2},  {zd_LD_IXYp_r,3}, 
/*78*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},  {zd_LD_r_IXYp,3},             {0,0}, 
/*80*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},   {zd_alu_IXYp,3},             {0,0}, 
/*88*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},   {zd_alu_IXYp,3},             {0,0}, 
/*90*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},   {zd_alu_IXYp,3},             {0,0}, 
/*98*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},   {zd_alu_IXYp,3},             {0,0}, 
/*A0*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},   {zd_alu_IXYp,3},             {0,0}, 
/*A8*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},   {zd_alu_IXYp,3},             {0,0}, 
/*B0*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},   {zd_alu_IXYp,3},             {0,0}, 
/*B8*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},   {zd_alu_IXYp,3},             {0,0}, 
/*C0*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0}, 
/*C8*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0}, 
/*D0*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0}, 
/*D8*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0}, 
/*E0*/            {0,0},    {zd_POP_IXY,2},             {0,0}, {zd_EX_SPp_IXY,2},             {0,0},   {zd_PUSH_IXY,2},             {0,0},             {0,0}, 
/*E8*/            {0,0},    {zd_JP_IXYp,2},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0}, 
/*F0*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0}, 
/*F8*/            {0,0},  {zd_LD_SP_IXY,2},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0}
};
static const opcode_dasm_t op_fd[256] = {
/*                    0                  1                  2                  3                  4                  5                  6                  7*/
/*00*/       {zd_NOP,2},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0}, 
/*08*/            {0,0}, {zd_ADD_IXY_rp,2},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0}, 
/*10*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0}, 
/*18*/            {0,0}, {zd_ADD_IXY_rp,2},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0}, 
/*20*/            {0,0},  {zd_LD_IXY_nn,4}, {zd_LD_nnp_IXY,4},    {zd_INC_IXY,2},             {0,0},             {0,0},  {zd_LD_IXYH_n,3},             {0,0}, 
/*28*/            {0,0}, {zd_ADD_IXY_rp,2}, {zd_LD_IXY_nnp,4},    {zd_DEC_IXY,2},             {0,0},             {0,0},  {zd_LD_IXYL_n,3},             {0,0}, 
/*30*/            {0,0},             {0,0},             {0,0},             {0,0},   {zd_INC_IXYp,3},   {zd_DEC_IXYp,3},  {zd_LD_IXYp_n,4},             {0,0}, 
/*38*/            {0,0}, {zd_ADD_IXY_rp,2},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0}, 
/*40*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},  {zd_LD_r_IXYp,3},             {0,0}, 
/*48*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},  {zd_LD_r_IXYp,3},             {0,0}, 
/*50*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},  {zd_LD_r_IXYp,3},             {0,0}, 
/*58*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},  {zd_LD_r_IXYp,3},             {0,0}, 
/*60*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},  {zd_LD_r_IXYp,3},             {0,0}, 
/*68*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},  {zd_LD_r_IXYp,3},             {0,0}, 
/*70*/ {zd_LD_IXYp_r,3},  {zd_LD_IXYp_r,3},  {zd_LD_IXYp_r,3},  {zd_LD_IXYp_r,3},  {zd_LD_IXYp_r,3},  {zd_LD_IXYp_r,3},       {zd_HALT,2},  {zd_LD_IXYp_r,3}, 
/*78*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},  {zd_LD_r_IXYp,3},             {0,0}, 
/*80*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},   {zd_alu_IXYp,3},             {0,0}, 
/*88*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},   {zd_alu_IXYp,3},             {0,0}, 
/*90*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},   {zd_alu_IXYp,3},             {0,0}, 
/*98*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},   {zd_alu_IXYp,3},             {0,0}, 
/*A0*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},   {zd_alu_IXYp,3},             {0,0}, 
/*A8*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},   {zd_alu_IXYp,3},             {0,0}, 
/*B0*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},   {zd_alu_IXYp,3},             {0,0}, 
/*B8*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},   {zd_alu_IXYp,3},             {0,0}, 
/*C0*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0}, 
/*C8*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0}, 
/*D0*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0}, 
/*D8*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0}, 
/*E0*/            {0,0},    {zd_POP_IXY,2},             {0,0}, {zd_EX_SPp_IXY,2},             {0,0},   {zd_PUSH_IXY,2},             {0,0},             {0,0}, 
/*E8*/            {0,0},    {zd_JP_IXYp,2},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0}, 
/*F0*/            {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0}, 
/*F8*/            {0,0},  {zd_LD_SP_IXY,2},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0},             {0,0}
};
static const opcode_dasm_t op_ddcb[256] = {
/*                    0                  1                  2                  3                  4                  5                  6                  7*/
/*00*/    {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},   {zd_rot_IXYp,4},     {zd_LD_ROT,4}, 
/*08*/    {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},   {zd_rot_IXYp,4},     {zd_LD_ROT,4}, 
/*10*/    {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},   {zd_rot_IXYp,4},     {zd_LD_ROT,4}, 
/*18*/    {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},   {zd_rot_IXYp,4},     {zd_LD_ROT,4}, 
/*20*/    {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},   {zd_rot_IXYp,4},     {zd_LD_ROT,4}, 
/*28*/    {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},   {zd_rot_IXYp,4},     {zd_LD_ROT,4}, 
/*30*/    {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},   {zd_rot_IXYp,4},     {zd_LD_ROT,4}, 
/*38*/    {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},   {zd_rot_IXYp,4},     {zd_LD_ROT,4}, 
/*40*/   {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4}, {zd_BIT_b_IXYp,4},    {zd_BIT_b_r,4}, 
/*48*/   {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4}, {zd_BIT_b_IXYp,4},    {zd_BIT_b_r,4}, 
/*50*/   {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4}, {zd_BIT_b_IXYp,4},    {zd_BIT_b_r,4}, 
/*58*/   {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4}, {zd_BIT_b_IXYp,4},    {zd_BIT_b_r,4}, 
/*60*/   {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4}, {zd_BIT_b_IXYp,4},    {zd_BIT_b_r,4}, 
/*68*/   {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4}, {zd_BIT_b_IXYp,4},    {zd_BIT_b_r,4}, 
/*70*/   {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4}, {zd_BIT_b_IXYp,4},    {zd_BIT_b_r,4}, 
/*78*/   {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4}, {zd_BIT_b_IXYp,4},    {zd_BIT_b_r,4}, 
/*80*/    {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4}, {zd_RES_b_IXYp,4},     {zd_LD_RES,4}, 
/*88*/    {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4}, {zd_RES_b_IXYp,4},     {zd_LD_RES,4}, 
/*90*/    {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4}, {zd_RES_b_IXYp,4},     {zd_LD_RES,4}, 
/*98*/    {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4}, {zd_RES_b_IXYp,4},     {zd_LD_RES,4}, 
/*A0*/    {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4}, {zd_RES_b_IXYp,4},     {zd_LD_RES,4}, 
/*A8*/    {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4}, {zd_RES_b_IXYp,4},     {zd_LD_RES,4}, 
/*B0*/    {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4}, {zd_RES_b_IXYp,4},     {zd_LD_RES,4}, 
/*B8*/    {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4}, {zd_RES_b_IXYp,4},     {zd_LD_RES,4}, 
/*C0*/    {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4}, {zd_SET_b_IXYp,4},     {zd_LD_SET,4}, 
/*C8*/    {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4}, {zd_SET_b_IXYp,4},     {zd_LD_SET,4}, 
/*D0*/    {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4}, {zd_SET_b_IXYp,4},     {zd_LD_SET,4}, 
/*D8*/    {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4}, {zd_SET_b_IXYp,4},     {zd_LD_SET,4}, 
/*E0*/    {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4}, {zd_SET_b_IXYp,4},     {zd_LD_SET,4}, 
/*E8*/    {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4}, {zd_SET_b_IXYp,4},     {zd_LD_SET,4}, 
/*F0*/    {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4}, {zd_SET_b_IXYp,4},     {zd_LD_SET,4}, 
/*F8*/    {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4}, {zd_SET_b_IXYp,4},     {zd_LD_SET,4}
};
static const opcode_dasm_t op_fdcb[256] = {
/*                    0                  1                  2                  3                  4                  5                  6                  7*/
/*00*/    {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},   {zd_rot_IXYp,4},     {zd_LD_ROT,4}, 
/*08*/    {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},   {zd_rot_IXYp,4},     {zd_LD_ROT,4}, 
/*10*/    {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},   {zd_rot_IXYp,4},     {zd_LD_ROT,4}, 
/*18*/    {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},   {zd_rot_IXYp,4},     {zd_LD_ROT,4}, 
/*20*/    {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},   {zd_rot_IXYp,4},     {zd_LD_ROT,4}, 
/*28*/    {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},   {zd_rot_IXYp,4},     {zd_LD_ROT,4}, 
/*30*/    {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},   {zd_rot_IXYp,4},     {zd_LD_ROT,4}, 
/*38*/    {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},     {zd_LD_ROT,4},   {zd_rot_IXYp,4},     {zd_LD_ROT,4}, 
/*40*/   {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4}, {zd_BIT_b_IXYp,4},    {zd_BIT_b_r,4}, 
/*48*/   {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4}, {zd_BIT_b_IXYp,4},    {zd_BIT_b_r,4}, 
/*50*/   {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4}, {zd_BIT_b_IXYp,4},    {zd_BIT_b_r,4}, 
/*58*/   {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4}, {zd_BIT_b_IXYp,4},    {zd_BIT_b_r,4}, 
/*60*/   {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4}, {zd_BIT_b_IXYp,4},    {zd_BIT_b_r,4}, 
/*68*/   {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4}, {zd_BIT_b_IXYp,4},    {zd_BIT_b_r,4}, 
/*70*/   {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4}, {zd_BIT_b_IXYp,4},    {zd_BIT_b_r,4}, 
/*78*/   {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4},    {zd_BIT_b_r,4}, {zd_BIT_b_IXYp,4},    {zd_BIT_b_r,4}, 
/*80*/    {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4}, {zd_RES_b_IXYp,4},     {zd_LD_RES,4}, 
/*88*/    {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4}, {zd_RES_b_IXYp,4},     {zd_LD_RES,4}, 
/*90*/    {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4}, {zd_RES_b_IXYp,4},     {zd_LD_RES,4}, 
/*98*/    {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4}, {zd_RES_b_IXYp,4},     {zd_LD_RES,4}, 
/*A0*/    {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4}, {zd_RES_b_IXYp,4},     {zd_LD_RES,4}, 
/*A8*/    {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4}, {zd_RES_b_IXYp,4},     {zd_LD_RES,4}, 
/*B0*/    {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4}, {zd_RES_b_IXYp,4},     {zd_LD_RES,4}, 
/*B8*/    {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4},     {zd_LD_RES,4}, {zd_RES_b_IXYp,4},     {zd_LD_RES,4}, 
/*C0*/    {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4}, {zd_SET_b_IXYp,4},     {zd_LD_SET,4}, 
/*C8*/    {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4}, {zd_SET_b_IXYp,4},     {zd_LD_SET,4}, 
/*D0*/    {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4}, {zd_SET_b_IXYp,4},     {zd_LD_SET,4}, 
/*D8*/    {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4}, {zd_SET_b_IXYp,4},     {zd_LD_SET,4}, 
/*E0*/    {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4}, {zd_SET_b_IXYp,4},     {zd_LD_SET,4}, 
/*E8*/    {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4}, {zd_SET_b_IXYp,4},     {zd_LD_SET,4}, 
/*F0*/    {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4}, {zd_SET_b_IXYp,4},     {zd_LD_SET,4}, 
/*F8*/    {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4},     {zd_LD_SET,4}, {zd_SET_b_IXYp,4},     {zd_LD_SET,4}
};

#endif
