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

#include "z80_dasm_tables.h"
#include "z80_dasm.h"
#include <string.h>
#include <stdio.h>
#include "debug/sms_debug.h"

static const char* z80d_r[8] = { "B", "C", "D", "E", "H", "L", "(HL)", "A" };
static const char* z80d_r_ix[8] = { "B", "C", "D", "E", "IXH", "IXL", "(IX + d)", "A" };
static const char* z80d_r_iy[8] = { "B", "C", "D", "E", "IYH", "IYL", "(IY + d)", "A" };
static const char* z80d_rp[4] = { "BC", "DE", "HL", "SP" };
static const char* z80d_rp2[4] = { "BC", "DE", "HL", "AF" };
static const char* const z80d_cc[8] = { "NZ", "Z", "NC", "C", "PO", "PE", "P", "M" };
static const char* const z80d_alu[8] = { "ADD A,", "ADC A,", "SUB", "SBC A,", "AND", "XOR", "OR", "CP" };
static const char* const z80d_rot[8] = { "RLC", "RRC", "RL", "RR", "SLA", "SRA", "SLL", "SRL" };
static const char* const z80d_im[8] = { "0", "0/1", "1", "2", "0", "0/1", "1", "2" };
static const char* const z80d_bli[4*4] = {
    "LDI", "CPI", "INI", "OUTI",
    "LDD", "CPD", "IND", "OUTD",
    "LDIR", "CPIR", "INIR", "OTIR",
    "LDDR", "CPDR", "INDR", "OTDR"
};

static z80d_opcode defaultOpcode() {
    z80d_opcode rv;
    rv.address = 0;
    rv.address_jump = 0;
    rv.address_next = 0;
    rv.flags = Z80D_TYPE_NORMAL;
    rv.immediate = 0;
    rv.opcode[0] = 0;
    rv.opcode[1] = 0;
    rv.opcode[2] = 0;
    rv.opcode[3] = 0;
    rv.opcode_str[0] = 0;
    rv.size = 0;

    return rv;
}

static uint8_t opcode_size(const uint8_t* opcode) {
    if (((opcode[0] == 0xFD) && (opcode[1] == 0xCB)) || ((opcode[0] == 0xDD) && (opcode[1] == 0xCB))) {
        //FDCB / DDCB
        return 4;
    }

    if ((opcode[0] == 0xFD) || (opcode[0] == 0xDD)) {
        return op_dd[opcode[1]].size;
    }

    if (opcode[0] == 0xED) {
        return 2;
    }

    if (opcode[0] == 0xCB) {
        return 2;
    }

    return op_unpref[opcode[0]].size;
}

char z80d_byte_to_char(uint8_t b){
    if ((b >= 0x20) && (b <= 0x7E)) //If printable, return as-is
        return b;
    else //Otherwise, a point
        return '.';
}

static z80d_opcode unknown_decode(const uint8_t * opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    sprintf(result, ".db %02Xh", opcode[0]);
    return rv;
}

int z80d_decode(const uint8_t* opcode, unsigned int size, char* result){
    result[0] = '\0';
    if (opcode[0] == 0xDD) {
        if (opcode[1] == 0xCB) {
            assert(op_ddcb[opcode[3]].f);
            return op_ddcb[opcode[3]].f(opcode, result).size;
        }
        else {
            if (op_dd[opcode[1]].f)
                return op_dd[opcode[1]].f(opcode, result).size;
            else
                return unknown_decode(opcode, result).size;
        }
    }
    else if (opcode[0] == 0xFD) {
        if (opcode[1] == 0xCB) {
            assert(op_fdcb[opcode[3]].f);
            return op_fdcb[opcode[3]].f(opcode, result).size;
        }
        else {
            if (op_fd[opcode[1]].f)
                return op_fd[opcode[1]].f(opcode, result).size;
            else
                return unknown_decode(opcode, result).size;
        }
    }
    else if (opcode[0] == 0xCB) {
        assert(op_cb[opcode[1]].f);
        return op_cb[opcode[1]].f(opcode, result).size;
    }
    else if (opcode[0] == 0xED) {
        assert(op_ed[opcode[1]].f);
        return op_ed[opcode[1]].f(opcode, result).size;
    }
    else {
        assert(op_unpref[opcode[0]].f);
        return op_unpref[opcode[0]].f(opcode, result).size;
    }
    return 1;
}

z80d_opcode z80d_decode_op(const uint8_t* opcode, uint16_t pc_addr){
    z80d_opcode rv;
    rv.address = pc_addr;
    memcpy(rv.opcode, opcode, 4);
    rv.size = z80d_decode(opcode, sizeof(rv.opcode_str), rv.opcode_str);
    return rv;
}

/* ----------------------------------------- */
/* --- Implementation of opcode decoders --- */
/* ----------------------------------------- */

z80d_opcode zd_ADC_HL_rp(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t q = ((opcode[1]) >> 4) & 0x03;
    sprintf(result, "ADC HL, %s", z80d_rp[q]);
    return rv;
}

z80d_opcode zd_ADD_HL_rp(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    const uint8_t q = ((opcode[0]) >> 4) & 0x03;
    sprintf(result, "ADD HL, %s", z80d_rp[q]);
    return rv;
}

z80d_opcode zd_ADD_IXY_rp(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t q = ((opcode[1]) >> 4) & 0x03;
    if(opcode[0] == 0xDD)
        sprintf(result, "ADD IX, %s", z80d_rp[q]);
    else
        sprintf(result, "ADD IY, %s", z80d_rp[q]);
    return rv;
}

z80d_opcode zd_alu_IXYp(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint8_t y = (opcode[1] >> 3) & 0x07;
    const int8_t  d = (int8_t)opcode[2];
    if(opcode[0] == 0xDD)
        sprintf(result, "%s A, (IX%+d)", z80d_alu[y], d);
    else
        sprintf(result, "%s A, (IY%+d)", z80d_alu[y], d);
    return rv;
}

z80d_opcode zd_alu_n(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t y = (opcode[0] >> 3) & 0x07;
    sprintf(result, "%s A, 0x%02X", z80d_alu[y], opcode[1]);
    return rv;
}

z80d_opcode zd_alu_r(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    const uint8_t y = (opcode[0] >> 3) & 0x07;
    const uint8_t z = (opcode[0]) & 0x07;
    sprintf(result, "%s A, %s", z80d_alu[y], z80d_r[z]);
    return rv;
}

z80d_opcode zd_alu_r_undoc(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t y = (opcode[1] >> 3) & 0x07;
    const uint8_t z = (opcode[1]) & 0x07;
    if (opcode[0] == 0xDD) {
        sprintf(result, "%s A, %s", z80d_alu[y], z80d_r_ix[z]);
    }
    else {
        sprintf(result, "%s A, %s", z80d_alu[y], z80d_r_iy[z]);
    }
    return rv;
}

z80d_opcode zd_BIT_b_IXYp(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const uint8_t y = (opcode[3] >> 3) & 0x07;
    const uint8_t z = (opcode[3]) & 0x07;
    const int8_t d = (int8_t) opcode[2];
    if(opcode[0] == 0xDD)
        sprintf(result, "BIT %d, (IX%+d)", y, d);
    else
        sprintf(result, "BIT %d, (IY%+d)", y, d);
    return rv;
}

z80d_opcode zd_BIT_b_r(const uint8_t* opcode, char* result) { 
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t y = (opcode[1] >> 3) & 0x07;
    const uint8_t z = (opcode[1]) & 0x07;
    sprintf(result, "BIT %d, %s", y, z80d_r[z]);
    return rv;
}

z80d_opcode zd_bli(const uint8_t* opcode, char* result) { 
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t y = (opcode[1] >> 3) & 0x07;
    const uint8_t z = (opcode[1]) & 0x07;
    sprintf(result, "%s", z80d_bli[(4*z) + y]);
    return rv;
}

z80d_opcode zd_CALL_cc_nn(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint8_t y = (opcode[0] >> 3) & 0x07;
    const uint16_t addr = opcode[1] + (((uint16_t)opcode[2]) << 8);
    sprintf(result, "CALL %s 0x%04X", z80d_cc[y], addr);
    return rv;
}

z80d_opcode zd_CALL_nn(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint16_t addr = opcode[1] + (((uint16_t)opcode[2]) << 8);
    sprintf(result, "CALL 0x%04X", addr);
    return rv;
}

z80d_opcode zd_CCF(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(result, "CCF");
    return rv;
}

z80d_opcode zd_CPL(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(result, "CPL");
    return rv;
}

z80d_opcode zd_DAA(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(result, "DAA");
    return rv;
}

z80d_opcode zd_DEC_IXY(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    if (opcode[0] == 0xDD)
        strcpy(result, "DEC IX");
    else
        strcpy(result, "DEC IY");
    return rv;
}

z80d_opcode zd_DEC_IXYp(const uint8_t* opcode, char* result) { 
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const int8_t d = (int8_t)opcode[2];
    if(opcode[0] == 0xDD)
        sprintf(result, "DEC (IX%+d)", d);
    else
        sprintf(result, "DEC (IY%+d)", d);
    return rv;
}

z80d_opcode zd_DEC_r(const uint8_t* opcode, char* result) { 
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    const uint8_t y = (opcode[0] >> 3) & 0x07;
    sprintf(result, "DEC %s", z80d_r[y]);
    return rv;
}

z80d_opcode zd_DEC_rp(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    const uint8_t p = (opcode[0] >> 4) & 0x03;
    sprintf(result, "DEC %s", z80d_rp[p]);
    return rv; //1;
}

z80d_opcode zd_DI(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(result, "DI");
    return rv; //1;
}

z80d_opcode zd_DJNZ_d(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    int8_t d = (int8_t)opcode[1];
    sprintf(result, "DJNZ %+d", d);
    return rv; //2;
}

z80d_opcode zd_EI(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(result, "EI");
    return rv; //1;
}

z80d_opcode zd_EX(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(result, "EX AF, AF'");
    return rv; //1;
}

z80d_opcode zd_EX_DE_HL(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(result, "EX DE, HL");
    return rv; //1;
}

z80d_opcode zd_EX_SPp_HL(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(result, "EX (SP), HL");
    return rv; //1;
}

z80d_opcode zd_EX_SPp_IXY(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    if (opcode[0] == 0xDD)
        strcpy(result, "EX (SP), IX");
    else
        strcpy(result, "EX (SP), IY");
    return rv; //2;
}

z80d_opcode zd_EXX(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(result, "EXX");
    return rv; //1;
}

z80d_opcode zd_HALT(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(result, "HALT");
    return rv; //1;
}

z80d_opcode zd_IM(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t y = (opcode[1] >> 3) & 0x07;
    sprintf(result, "IM %s", z80d_im[y]);
    return rv; //2;
}

z80d_opcode zd_IN_A_np(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t n = opcode[1];
    sprintf(result, "IN A, (0x%02X)", n);
    return rv; //2;
}

z80d_opcode zd_IN_r_Cp(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t y = (opcode[1] >> 3) & 0x07;
    sprintf(result, "IN %s, (C)", z80d_r[y]);
    return rv; //2;
}

z80d_opcode zd_INC_IXY(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    if (opcode[0] == 0xDD)
        strcpy(result, "INC IX");
    else
        strcpy(result, "INC IY");
    return rv; //2;
}

z80d_opcode zd_INC_IXYp(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const int8_t d = (int8_t)opcode[2];
    if (opcode[0] == 0xDD)
        sprintf(result, "INC (IX%+d)", d);
    else
        sprintf(result, "INC (IY%+d)", d);
    return rv; //3;
}

z80d_opcode zd_INC_r(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    const uint8_t y = (opcode[0] >> 3) & 0x07;
    sprintf(result, "INC %s", z80d_r[y]);
    return rv; //1;
}

z80d_opcode zd_INC_rp(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    const uint8_t p = (opcode[0] >> 4) & 0x03;
    sprintf(result, "INC %s", z80d_rp[p]);
    return rv; //1;
}

z80d_opcode zd_JP_cc_nn(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint8_t y = (opcode[0] >> 3) & 0x07;
    const uint16_t addr = opcode[1] + (((uint16_t)opcode[2]) << 8);
    sprintf(result, "JP %s, 0x%04X", z80d_cc[y], addr);
    return rv; //3;
}

z80d_opcode zd_JP_HLp(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(result, "JP HL");
    return rv; //1;
}

z80d_opcode zd_JP_IXYp(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    if (opcode[0] == 0xDD)
        strcpy(result, ";Check!\nJP IX");
    else
        strcpy(result, ";Check!\nJP IY");
    return rv; //2;
}

z80d_opcode zd_JP_nn(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint16_t addr = opcode[1] + (((uint16_t)opcode[2]) << 8);
    sprintf(result, "JP 0x%04X", addr);
    return rv; //3;
}

z80d_opcode zd_JR_cc_d(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const int8_t d = (int8_t) opcode[1];
    const uint8_t y = (opcode[0] >> 3) & 0x07;
    sprintf(result, "JR %s, %+d", z80d_cc[y - 4], d);
    return rv; //2;
}

z80d_opcode zd_JR_d(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const int8_t d = (int8_t)opcode[1];
    sprintf(result, "JR %+d", d);
    return rv; //2;
}

z80d_opcode zd_LD_A_BCp(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(result, "LD A, (BC)");
    return rv; //1;
}

z80d_opcode zd_LD_A_DEp(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(result, "LD A, (DE)");
    return rv; //1;
}

z80d_opcode zd_LD_A_I(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    strcpy(result, "LD A, I");
    return rv; //2;
}

z80d_opcode zd_LD_A_nnp(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint16_t addr = opcode[1] + (((uint16_t)opcode[2]) << 8);
    sprintf(result, "LD A, (0x%04X)", addr);
    return rv; //3;
}

z80d_opcode zd_LD_A_R(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    strcpy(result, "LD A, R");
    return rv; //2;
}

z80d_opcode zd_LD_BCp_A(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(result, "LD (BC), A");
    return rv; //1;
}

z80d_opcode zd_LD_DEp_A(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(result, "LD (DE), A");
    return rv; //1;
}

z80d_opcode zd_LD_HL_nnp(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint16_t addr = opcode[1] + (((uint16_t)opcode[2]) << 8);
    sprintf(result, "LD HL, (0x%04X)", addr);
    return rv; //3;
}

z80d_opcode zd_LD_I_A(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    strcpy(result, "LD I, A");
    return rv; //2;
}

z80d_opcode zd_LD_IXY_nn(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const uint16_t addr = opcode[2] + (((uint16_t)opcode[3]) << 8);
    if (opcode[0] == 0xDD)
        sprintf(result, "LD IX, 0x%04X", addr);
    else
        sprintf(result, "LD IY, 0x%04X", addr);
    return rv; //4;
}

z80d_opcode zd_LD_IXY_nnp(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const uint16_t addr = opcode[2] + (((uint16_t)opcode[3]) << 8);
    if(opcode[0] == 0xDD)
        sprintf(result, "LD IX, (0x%04X)", addr);
    else
        sprintf(result, "LD IY, (0x%04X)", addr);
    return rv; //4;
}

z80d_opcode zd_LD_IXYH_n(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint8_t n = opcode[2];
    if(opcode[0] == 0xDD)
        sprintf(result, "LD IXH, 0x%02X", n);
    else
        sprintf(result, "LD IYH, 0x%02X", n);
    return rv; //3;
}

z80d_opcode zd_LD_IXYL_n(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint8_t n = opcode[2];
    if (opcode[0] == 0xDD)
        sprintf(result, "LD IXL, 0x%02X", n);
    else
        sprintf(result, "LD IYL, 0x%02X", n);
    return rv; //3;
}

z80d_opcode zd_LD_IXYp_n(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const uint8_t n = opcode[2];
    const int8_t d = (int8_t)opcode[3];
    if (opcode[0] == 0xDD)
        sprintf(result, "LD (IX%+d), 0x%02X", d, n);
    else
        sprintf(result, "LD (IY%+d), 0x%02X", d, n);
    return rv; //4;
}

z80d_opcode zd_LD_IXYp_r(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint8_t z = (opcode[1]) & 0x07;
    const int8_t  d = (int8_t)opcode[2];
    sprintf(result, "LD (IX%+d), %s", d, z80d_r[z]);
    return rv; //3;
}

z80d_opcode zd_LD_nnp_A(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint16_t addr = opcode[1] + (((uint16_t)opcode[2]) << 8);
    sprintf(result, "LD (0x%04X), A", addr);
    return rv; //3;
}

z80d_opcode zd_LD_nnp_HL(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint16_t addr = opcode[1] + (((uint16_t)opcode[2]) << 8);
    sprintf(result, "LD (0x%04X), HL", addr);
    return rv; //3;
}

z80d_opcode zd_LD_nnp_IXY(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const uint16_t addr = opcode[2] + (((uint16_t)opcode[3]) << 8);
    if(opcode[0] == 0xDD)
        sprintf(result, "LD (0x%04X), IX", addr);
    else
        sprintf(result, "LD (0x%04X), IY", addr);
    return rv; //4;
}

z80d_opcode zd_LD_nnp_rp(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const uint8_t p = (opcode[1] >> 4) & 0x03;
    const uint16_t addr = opcode[2] + (((uint16_t)opcode[3])<<8);
    sprintf(result, "LD (0x%04X), %s", addr, z80d_rp[p]);
    return rv; //4;
}

z80d_opcode zd_LD_R_A(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    strcpy(result, "LD R, A");
    return rv; //2;
}

z80d_opcode zd_LD_r_IXYp(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint8_t y = (opcode[1] >> 3) & 0x07;
    const int8_t  d = (int8_t)opcode[2];
	if (opcode[0] == 0xDD) {
		sprintf(result, "LD %s, (IX%+d)", z80d_r[y], d);
	}
	else {
		sprintf(result, "LD %s, (IY%+d)", z80d_r[y], d);
	}
    return rv; //3;
}

z80d_opcode zd_LD_r_n(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t n = opcode[1];
    const uint8_t y = (opcode[0] >> 3) & 0x07;
    sprintf(result, "LD %s, 0x%02X", z80d_r[y], n);
    return rv; //2;
}

z80d_opcode zd_LD_r_r(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    const uint8_t y = (opcode[0] >> 3) & 0x07;
    const uint8_t z = opcode[0] & 0x07;
    sprintf(result, "LD %s, %s", z80d_r[y], z80d_r[z]);
    return rv; //1;
}

z80d_opcode zd_LD_r_r_undoc(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t y = (opcode[1] >> 3) & 0x07;
    const uint8_t z = opcode[1] & 0x07;
    if (opcode[0] == 0xDD) {
        sprintf(result, "LD %s, %s", z80d_r_ix[y], z80d_r_ix[z]);
    }
    else {
        sprintf(result, "LD %s, %s", z80d_r_iy[y], z80d_r_iy[z]);
    }
    return rv; //1;
}

z80d_opcode zd_LD_rp_nn(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    uint8_t p = (opcode[0] >> 4) & 0x03;
    uint16_t nn = opcode[1] + (((uint16_t)opcode[2]) << 8);
    sprintf(result, "LD %s, 0x%04X", z80d_rp[p], nn);
    return rv; //3;
}

z80d_opcode zd_LD_rp_nnp(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const uint8_t p = (opcode[1] >> 4) & 0x03;
    const uint16_t addr = opcode[2] + (((uint16_t)opcode[3])<<8);
    sprintf(result, "LD %s, (0x%04X)", z80d_rp[p], addr);
    return rv; //4;
}

z80d_opcode zd_LD_SP_HL(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(result, "LD SP, HL");
    return rv; //1;
}

z80d_opcode zd_LD_SP_IXY(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    if(opcode[0] == 0xDD)
        strcpy(result, "LD SP, IX");
    else
        strcpy(result, "LD SP, IY");
    return rv; //2;
}

z80d_opcode zd_NEG(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    strcpy(result, "NEG");
    return rv; //2;
}

z80d_opcode zd_NOP(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(result, "NOP");
    return rv; //1;
}

z80d_opcode zd_OUT_Cp_r(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t y = (opcode[1] >> 3) & 0x07;
    sprintf(result, "OUT (C), %s", z80d_r[y]);
    return rv; //2;
}

z80d_opcode zd_OUT_np_A(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t n = opcode[1];
    sprintf(result, "OUT (0x%02X), A", n);
    return rv; //2;
}

z80d_opcode zd_POP_IXY(const uint8_t* opcode, char* result) { 
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    if (opcode[0] == 0xDD)
        strcpy(result, "POP IX");
    else
        strcpy(result, "POP IY");
    return rv; //2;
}

z80d_opcode zd_POP_rp2(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    const uint8_t p = (opcode[0] >> 4) & 0x03;
    sprintf(result, "POP %s", z80d_rp2[p]);
    return rv; //1;
}

z80d_opcode zd_PUSH_IXY(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    if (opcode[0] == 0xDD)
        strcpy(result, "PUSH IX");
    else
        strcpy(result, "PUSH IY");
    return rv; //2;
}

z80d_opcode zd_PUSH_rp2(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    const uint8_t p = (opcode[0] >> 4) & 0x03;
    sprintf(result, "PUSH %s", z80d_rp2[p]);
    return rv; //1;
}

z80d_opcode zd_RES_b_IXYp(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const int8_t d = (int8_t)opcode[2];
    const uint8_t y = (opcode[3] >> 3) & 0x07;
	if (opcode[0] == 0xDD) {
		sprintf(result, "RES %d, (IX%+d)", y, d);
	}
	else {
		sprintf(result, "RES %d, (IY%+d)", y, d);
	}
    return rv; //4;
}

z80d_opcode zd_RES_b_r(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t y = (opcode[1] >> 3) & 0x07;
    const uint8_t z = (opcode[1]) & 0x07;
    sprintf(result, "RES %d, %s", y, z80d_r[z]);
    return rv; //2;
}

z80d_opcode zd_RET(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(result, "RET");
    return rv; //1;
}

z80d_opcode zd_RET_cc(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    const uint8_t y = (opcode[0] >> 3) & 0x07;
    sprintf(result, "RET %s", z80d_cc[y]);
    return rv; //1;
}

z80d_opcode zd_RETI(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    strcpy(result, "RETI");
    return rv; //2;
}

z80d_opcode zd_RETN(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    strcpy(result, "RETN");
    return rv; //2;
}

z80d_opcode zd_RLA(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(result, "RLA");
    return rv; //1;
}

z80d_opcode zd_RLCA(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(result, "RLCA");
    return rv; //1;
}

z80d_opcode zd_RLD(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(result, "RLD");
    return rv; //1;
}

z80d_opcode zd_rot(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t y = (opcode[1] >> 3) & 0x07;
    const uint8_t z = opcode[1] & 0x07;
    sprintf(result, "%s %s", z80d_rot[y], z80d_r[z]);
    return rv; //2;
}

z80d_opcode zd_rot_IXYp(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const uint8_t d = opcode[2];
    const uint8_t y = (opcode[3] >> 3) & 0x07;
	if (opcode[0] == 0xDD) {
		sprintf(result, "%s (IX%+d)", z80d_rot[y], d);
	}
	else {
		sprintf(result, "%s (IY%+d)", z80d_rot[y], d);
	}
    return rv; //4;
}

z80d_opcode zd_RRA(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    strcpy(result, "RRA");
    return rv; //2;
}

z80d_opcode zd_RRCA(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(result, "RRCA");
    return rv; //1;
}

z80d_opcode zd_RRD(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    strcpy(result, "RRD");
    return rv; //2;
}

z80d_opcode zd_RST_y(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    const uint8_t y = (opcode[0] >> 3) & 7;
    sprintf(result, "RST 0x%04X", y * 8);
    return rv; //1;
}

z80d_opcode zd_SBC_HL_rp(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t p = (opcode[1] >> 4) & 0x03;
    sprintf(result, "SBC HL, %s", z80d_rp[p]);
    return rv; //2;
}

z80d_opcode zd_SCF(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(result, "SCF");
    return rv; //1;
}

z80d_opcode zd_SET_b_IXYp(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const int8_t d = (int8_t)opcode[2];
    const uint8_t y = (opcode[3] >> 3) & 0x07;
	if (opcode[0] == 0xDD) {
		sprintf(result, "SET %d, (IX%+d)", y, d);
	}
	else {
		sprintf(result, "SET %d, (IY%+d)", y, d);
	}
    return rv; //4;
}

z80d_opcode zd_SET_b_r(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t y = (opcode[1] >> 3) & 0x07;
    const uint8_t z = (opcode[1]) & 0x07;
    sprintf(result, "SET %d, %s", y, z80d_r[z]);
    return rv; //2;
}

z80d_opcode zd_LD_RES(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const int8_t d = (int8_t)opcode[2];
    const uint8_t y = (opcode[3] >> 3) & 0x07;
    const uint8_t z = (opcode[3]) & 0x07;
    if(opcode[0] == 0xDD)
        sprintf(result, "LD %s, RES %d, (IX%+d)", z80d_r[z], y, d);
    else
        sprintf(result, "LD %s, RES %d, (IY%+d)", z80d_r[z], y, d);
    return rv; //4;
}
z80d_opcode zd_LD_ROT(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const int8_t d = (int8_t)opcode[2];
    const uint8_t y = (opcode[3] >> 3) & 0x07;
    const uint8_t z = (opcode[3]) & 0x07;
    if (opcode[0] == 0xDD)
        sprintf(result, "LD %s, %s (IX%+d)", z80d_r[z], z80d_rot[y], d);
    else
        sprintf(result, "LD %s, %s (IY%+d)", z80d_r[z], z80d_rot[y], d);
    return rv; //4;
}
z80d_opcode zd_LD_SET(const uint8_t* opcode, char* result) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const int8_t d = (int8_t)opcode[2];
    const uint8_t y = (opcode[3] >> 3) & 0x07;
    const uint8_t z = (opcode[3]) & 0x07;
    if (opcode[0] == 0xDD)
        sprintf(result, "LD %s, SET %d, (IX%+d)", z80d_r[z], y, d);
    else
        sprintf(result, "LD %s, SET %d, (IY%+d)", z80d_r[z], y, d);
    return rv; //4;
}
