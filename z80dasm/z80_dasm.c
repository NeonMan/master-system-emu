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

#include "z80_dasm_tables.h"
#include "z80_dasm.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

const char* z80d_r[8] = { "B", "C", "D", "E", "H", "L", "(HL)", "A" };
const char* z80d_rp[4] = { "BC", "DE", "HL", "SP" };
const char* z80d_rp2[4] = { "BC", "DE", "HL", "AF" };
const char* const z80d_cc[8] = { "NZ", "Z", "NC", "C", "PO", "PE", "P", "M" };
const char* const z80d_alu[8] = { "ADD A,", "ADC A,", "SUB", "SBC A,", "AND", "XOR", "OR", "CP" };
const char* const z80d_rot[8] = { "RLC", "RRC", "RL", "RR", "SLA", "SRA", "SLL", "SRL" };
const char* const z80d_im[8] = { "0", "0/1", "1", "2", "0", "0/1", "1", "2" };
const char* const z80d_bli[4*4] = {
    "LDI", "CPI", "INI", "OUTI",
    "LDD", "CPD", "IND", "OUTD",
    "LDIR", "CPIR", "INIR", "OTIR",
    "LDDR", "CPDR", "INDR", "OTDR"
};

char z80d_byte_to_char(uint8_t b){
    if ((b >= 0x20) && (b <= 0x7E)) //If printable, return as-is
        return b;
    else //Otherwise, a point
        return '.';
}

int z80d_decode(const uint8_t* opcode, unsigned int size, char* result){
    if (opcode[0] == 0xDD) {
        if (opcode[1] == 0xCB) {
            assert(op_ddcb[opcode[3]].f);
            return op_ddcb[opcode[3]].f(opcode, result);
        }
        else {
            assert(op_dd[opcode[1]].f);
            op_dd[opcode[1]].f(opcode, result);
        }
        return 1;
    }
    else if (opcode[0] == 0xFD) {
        if (opcode[1] == 0xCB) {
            assert(op_fdcb[opcode[3]].f);
            return op_fdcb[opcode[3]].f(opcode, result);
        }
        else {
            assert(op_fd[opcode[1]].f);
            op_fd[opcode[1]].f(opcode, result);
        }
        return 1;
    }
    else if (opcode[0] == 0xCB) {
        assert(op_cb[opcode[1]].f);
        return op_cb[opcode[1]].f(opcode, result);
    }
    else if (opcode[0] == 0xED) {
        assert(op_ed[opcode[1]].f);
        return op_ed[opcode[1]].f(opcode, result);
    }
    else {
        assert(op_unpref[opcode[0]].f);
        return op_unpref[opcode[0]].f(opcode, result);
    }
}

z80d_opcode z80d_decode_op(const uint8_t* opcode, uint16_t pc_addr){
    z80d_opcode rv;
    rv.address = pc_addr;
    rv.next = 0;
    memcpy(rv.opcode, opcode, 4);
    rv.size = z80d_decode(opcode, sizeof(rv.opcode_str), rv.opcode_str);
    return rv;
}


/* --- Implementation of opcode decoders --- */

int zd_unimplemented(const uint8_t* opcode, char* result) {
    sprintf(result, "!!UNIMP");
    return 1;
}

int zd_ADC_HL_rp(const uint8_t* opcode, char* result) {
    const uint8_t q = ((opcode[1]) >> 4) & 0x03;
    sprintf(result, "ADC HL, %s", z80d_rp[q]);
    return 2;
}

int zd_ADD_HL_rp(const uint8_t* opcode, char* result) {
    const uint8_t q = ((opcode[0]) >> 4) & 0x03;
    sprintf(result, "ADD HL, %s", z80d_rp[q]);
    return 1;
}

int zd_ADD_IXY_rp(const uint8_t* opcode, char* result) {
    const uint8_t q = ((opcode[1]) >> 4) & 0x03;
    if(opcode[0] == 0xDD)
        sprintf(result, "ADC IX, %s", z80d_rp[q]);
    else
        sprintf(result, "ADC IY, %s", z80d_rp[q]);
    return 2;
}

int zd_alu_IXYp(const uint8_t* opcode, char* result) {
    const uint8_t y = (opcode[1] >> 3) & 0x07;
    const int8_t  d = (int8_t)opcode[2];
    if(opcode[0] == 0xDD)
        sprintf(result, "%s A, (IX%+d)", z80d_alu[y], d);
    else
        sprintf(result, "%s A, (IY%+d)", z80d_alu[y], d);
    return 3;
}

int zd_alu_n(const uint8_t* opcode, char* result) {
    const uint8_t y = (opcode[0] >> 3) & 0x07;
    sprintf(result, "%s A, 0x%02X", z80d_alu[y], opcode[1]);
    return 2;
}

int zd_alu_r(const uint8_t* opcode, char* result) {
    const uint8_t y = (opcode[0] >> 3) & 0x07;
    const uint8_t z = (opcode[0]) & 0x07;
    sprintf(result, "%s A, %s", z80d_alu[y], z80d_r[z]);
    return 1;
}

int zd_BIT_b_IXYp(const uint8_t* opcode, char* result) {
    const uint8_t y = (opcode[3] >> 3) & 0x07;
    const uint8_t z = (opcode[3]) & 0x07;
    const int8_t d = (int8_t) opcode[2];
    if(opcode[0] == 0xDD)
        sprintf(result, "BIT %d, (IX%+d)", y, d);
    else
        sprintf(result, "BIT %d, (IY%+d)", y, d);
    return 4;
}

int zd_BIT_b_r(const uint8_t* opcode, char* result) { 
    const uint8_t y = (opcode[1] >> 3) & 0x07;
    const uint8_t z = (opcode[1]) & 0x07;
    sprintf(result, "BIT %d, %s", y, z80d_r[z]);
    return 2;
}

int zd_bli(const uint8_t* opcode, char* result) { 
    const uint8_t y = (opcode[1] >> 3) & 0x07;
    const uint8_t z = (opcode[1]) & 0x07;
    sprintf(result, "%s", z80d_bli[(4*z) + y]);
    return 2;
}

int zd_CALL_cc_nn(const uint8_t* opcode, char* result) {
    const uint8_t y = (opcode[0] >> 3) & 0x07;
    const uint16_t addr = opcode[1] + (((uint16_t)opcode[2]) << 8);
    sprintf(result, "CALL %s 0x%04X", z80d_cc[y], addr);
    return 3;
}

int zd_CALL_nn(const uint8_t* opcode, char* result) {
    const uint16_t addr = opcode[1] + (((uint16_t)opcode[2]) << 8);
    sprintf(result, "CALL 0x%04X", addr);
    return 3;
}

int zd_CCF(const uint8_t* opcode, char* result) {
    strcpy(result, "CCF");
    return 1;
}

int zd_CPL(const uint8_t* opcode, char* result) {
    strcpy(result, "CPL");
    return 1;
}

int zd_DAA(const uint8_t* opcode, char* result) {
    strcpy(result, "DAA");
    return 1;
}

int zd_DEC_IXY(const uint8_t* opcode, char* result) {
    if (opcode[0] == 0xDD)
        strcpy(result, "DEC IX");
    else
        strcpy(result, "DEC IY");
    return 2;
}

int zd_DEC_IXYp(const uint8_t* opcode, char* result) { 
    const int8_t d = (int8_t)opcode[2];
    if(opcode[0] == 0xDD)
        sprintf(result, "DEC (IX%+d)", d);
    else
        sprintf(result, "DEC (IY%+d)", d);
    return 3;
}

int zd_DEC_r(const uint8_t* opcode, char* result) { 
    const uint8_t y = (opcode[0] >> 3) & 0x07;
    sprintf(result, "DEC %s", z80d_r[y]);
    return 1;
}

int zd_DEC_rp(const uint8_t* opcode, char* result) {
    const uint8_t p = (opcode[0] >> 4) & 0x03;
    sprintf(result, "DEC %s", z80d_rp[p]);
    return 1;
}

int zd_DI(const uint8_t* opcode, char* result) {
    strcpy(result, "DI");
    return 1;
}

int zd_DJNZ_d(const uint8_t* opcode, char* result) {
    int8_t d = (int8_t)opcode[1];
    sprintf(result, "DJNZ %+d", d);
    return 2;
}

int zd_EI(const uint8_t* opcode, char* result) {
    strcpy(result, "EI");
    return 1;
}

int zd_EX(const uint8_t* opcode, char* result) {
    strcpy(result, "EX AF, AF'");
    return 1;
}

int zd_EX_DE_HL(const uint8_t* opcode, char* result) {
    strcpy(result, "EX DE, HL");
    return 1;
}

int zd_EX_SPp_HL(const uint8_t* opcode, char* result) {
    strcpy(result, "EX (SP), HL");
    return 1;
}

int zd_EX_SPp_IXY(const uint8_t* opcode, char* result) {
    if (opcode[0] == 0xDD)
        strcpy(result, "EX (SP), IX");
    else
        strcpy(result, "EX (SP), IY");
    return 2;
}

int zd_EXX(const uint8_t* opcode, char* result) {
    strcpy(result, "EXX");
    return 1;
}

int zd_HALT(const uint8_t* opcode, char* result) {
    strcpy(result, "HALT");
    return 1;
}

int zd_IM(const uint8_t* opcode, char* result) {
    const uint8_t y = (opcode[1] >> 3) & 0x07;
    sprintf(result, "IM %s", z80d_im[y]);
    return 2;
}

int zd_IN_A_np(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_IN_r_Cp(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_INC_IXY(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_INC_IXYp(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_INC_r(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_INC_rp(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_JP_cc_nn(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_JP_HLp(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_JP_IXYp(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_JP_nn(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_JR_cc_d(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_JR_d(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }

int zd_LD_A_BCp(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_A_DEp(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_A_I(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_A_nnp(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_A_R(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_BCp_A(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_DEp_A(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_HL_nnp(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_I_A(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_IXY_nn(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_IXY_nnp(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_IXYH_n(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_IXYL_n(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_IXYp_n(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_IXYp_r(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_nnp_A(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_nnp_HL(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_nnp_IXY(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_nnp_rp(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_R_A(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_r_IXYp(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_r_n(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_r_r(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_RES(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_ROT(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_rp_nn(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_rp_nnp(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_SET(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_SP_HL(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_LD_SP_IXY(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }

int zd_NEG(const uint8_t* opcode, char* result) {
    strcpy(result, "NEG");
    return 2;
}

int zd_NOP(const uint8_t* opcode, char* result) {
    strcpy(result, "NOP");
    return 1;
}

int zd_OUT_Cp_r(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_OUT_np_A(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_POP_IXY(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_POP_rp2(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_PUSH_IXY(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_PUSH_rp2(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_RES_b_IXYp(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_RES_b_r(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }

int zd_RET(const uint8_t* opcode, char* result) {
    strcpy(result, "RET");
    return 1;
}

int zd_RET_cc(const uint8_t* opcode, char* result) {
    const uint8_t y = (opcode[0] >> 3) & 0x07;
    sprintf("RET %s", z80d_cc[y]);
    return 1;
}

int zd_RETI(const uint8_t* opcode, char* result) {
    strcpy(result, "RETI");
    return 2;
}

int zd_RETN(const uint8_t* opcode, char* result) {
    strcpy(result, "RETN");
    return 2;
}

int zd_RLA(const uint8_t* opcode, char* result) {
    strcpy(result, "RLA");
    return 1;
}

int zd_RLCA(const uint8_t* opcode, char* result) {
    strcpy(result, "RLCA");
    return 1;
}

int zd_RLD(const uint8_t* opcode, char* result) {
    strcpy(result, "RLD");
    return 1;
}

int zd_rot(const uint8_t* opcode, char* result) {
    const uint8_t y = (opcode[1] >> 3) & 0x07;
    const uint8_t z = opcode[1] & 0x07;
    sprintf(result, "%s %s", z80d_rot[y], z80d_r[z]);
    return 2;
}

int zd_rot_IXYp(const uint8_t* opcode, char* result) {
    const uint8_t d = opcode[2];
    const uint8_t y = (opcode[3] >> 3) & 0x07;
    sprintf(result, "%s (IX%+d)", z80d_rot[y], d);
    return 4;
}

int zd_RRA(const uint8_t* opcode, char* result) {
    strcpy(result, "RRA");
    return 2;
}

int zd_RRCA(const uint8_t* opcode, char* result) {
    strcpy(result, "RRCA");
    return 2;
}

int zd_RRD(const uint8_t* opcode, char* result) {
    strcpy(result, "RRD");
    return 2;
}

int zd_RST_y(const uint8_t* opcode, char* result) {
    const uint8_t y = (opcode[0] >> 3) & 7;
    sprintf(result, "RST 0x%04X", y * 8);
    return 1;
}

int zd_SBC_HL_rp(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_SCF(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_SET_b_IXYp(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
int zd_SET_b_r(const uint8_t* opcode, char* result) { return zd_unimplemented(opcode, result); }
