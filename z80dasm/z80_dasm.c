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
    strcpy(rv.opcode_str, "##UNK##");
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

static z80d_opcode unknown_decode(const uint8_t * opcode) {
    z80d_opcode rv = defaultOpcode();
    sprintf(rv.opcode_str, ".db %02Xh", opcode[0]);
    return rv;
}

z80d_opcode z80d_decode_op(const uint8_t* opcode, uint16_t pc_addr) {
    z80d_opcode rv;

    if (opcode[0] == 0xDD) {
        if (opcode[1] == 0xCB) {
            assert(op_ddcb[opcode[3]].f);
            rv = op_ddcb[opcode[3]].f(opcode);
        }
        else {
            if (op_dd[opcode[1]].f)
                rv =  op_dd[opcode[1]].f(opcode);
            else
                rv =  unknown_decode(opcode);
        }
    }
    else if (opcode[0] == 0xFD) {
        if (opcode[1] == 0xCB) {
            assert(op_fdcb[opcode[3]].f);
            rv =  op_fdcb[opcode[3]].f(opcode);
        }
        else {
            if (op_fd[opcode[1]].f)
                rv =  op_fd[opcode[1]].f(opcode);
            else
                rv =  unknown_decode(opcode);
        }
    }
    else if (opcode[0] == 0xCB) {
        assert(op_cb[opcode[1]].f);
        rv =  op_cb[opcode[1]].f(opcode);
    }
    else if (opcode[0] == 0xED) {
        assert(op_ed[opcode[1]].f);
        rv =  op_ed[opcode[1]].f(opcode);
    }
    else {
        assert(op_unpref[opcode[0]].f);
        rv =  op_unpref[opcode[0]].f(opcode);
    }

    rv.address = pc_addr;
    rv.address_next = rv.size + pc_addr;
    //Fix all the relative jumps address_jump using the immediate val.
    if (rv.flags & Z80D_TYPE_REL_JUMP) {
        rv.address_jump = pc_addr + rv.immediate + rv.size;
    }


    return rv;
}

int z80d_decode(const uint8_t* opcode, unsigned int size, char* result) {
    z80d_opcode op = z80d_decode_op(opcode, 0);
    strncpy(result, op.opcode_str, size);
    return op.size;
}

/* ----------------------------------------- */
/* --- Implementation of opcode decoders --- */
/* ----------------------------------------- */

z80d_opcode zd_ADC_HL_rp(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t q = ((opcode[1]) >> 4) & 0x03;
    sprintf(rv.opcode_str, "ADC HL, %s", z80d_rp[q]);
    return rv;
}

z80d_opcode zd_ADD_HL_rp(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    const uint8_t q = ((opcode[0]) >> 4) & 0x03;
    sprintf(rv.opcode_str, "ADD HL, %s", z80d_rp[q]);
    return rv;
}

z80d_opcode zd_ADD_IXY_rp(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t q = ((opcode[1]) >> 4) & 0x03;
    if(opcode[0] == 0xDD)
        sprintf(rv.opcode_str, "ADD IX, %s", z80d_rp[q]);
    else
        sprintf(rv.opcode_str, "ADD IY, %s", z80d_rp[q]);
    return rv;
}

z80d_opcode zd_alu_IXYp(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint8_t y = (opcode[1] >> 3) & 0x07;
    const int8_t  d = (int8_t)opcode[2];
    if(opcode[0] == 0xDD)
        sprintf(rv.opcode_str, "%s A, (IX%+d)", z80d_alu[y], d);
    else
        sprintf(rv.opcode_str, "%s A, (IY%+d)", z80d_alu[y], d);
    return rv;
}

z80d_opcode zd_alu_n(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t y = (opcode[0] >> 3) & 0x07;
    sprintf(rv.opcode_str, "%s A, 0x%02X", z80d_alu[y], opcode[1]);
    return rv;
}

z80d_opcode zd_alu_r(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    const uint8_t y = (opcode[0] >> 3) & 0x07;
    const uint8_t z = (opcode[0]) & 0x07;
    sprintf(rv.opcode_str, "%s A, %s", z80d_alu[y], z80d_r[z]);
    return rv;
}

z80d_opcode zd_alu_r_undoc(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t y = (opcode[1] >> 3) & 0x07;
    const uint8_t z = (opcode[1]) & 0x07;
    if (opcode[0] == 0xDD) {
        sprintf(rv.opcode_str, "%s A, %s", z80d_alu[y], z80d_r_ix[z]);
    }
    else {
        sprintf(rv.opcode_str, "%s A, %s", z80d_alu[y], z80d_r_iy[z]);
    }
    return rv;
}

z80d_opcode zd_BIT_b_IXYp(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const uint8_t y = (opcode[3] >> 3) & 0x07;
    const uint8_t z = (opcode[3]) & 0x07;
    const int8_t d = (int8_t) opcode[2];
    if(opcode[0] == 0xDD)
        sprintf(rv.opcode_str, "BIT %d, (IX%+d)", y, d);
    else
        sprintf(rv.opcode_str, "BIT %d, (IY%+d)", y, d);
    return rv;
}

z80d_opcode zd_BIT_b_r(const uint8_t* opcode) { 
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t y = (opcode[1] >> 3) & 0x07;
    const uint8_t z = (opcode[1]) & 0x07;
    sprintf(rv.opcode_str, "BIT %d, %s", y, z80d_r[z]);
    return rv;
}

z80d_opcode zd_bli(const uint8_t* opcode) { 
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t y = (opcode[1] >> 3) & 0x07;
    const uint8_t z = (opcode[1]) & 0x07;
    sprintf(rv.opcode_str, "%s", z80d_bli[(4*z) + y]);
    return rv;
}

z80d_opcode zd_CALL_cc_nn(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint8_t y = (opcode[0] >> 3) & 0x07;
    const uint16_t addr = opcode[1] + (((uint16_t)opcode[2]) << 8);
    sprintf(rv.opcode_str, "CALL %s 0x%04X", z80d_cc[y], addr);

    rv.flags = Z80D_TYPE_CONST_JUMP | Z80D_TYPE_NORMAL | Z80D_TYPE_CALL;
    rv.address_jump = addr;
    return rv;
}

z80d_opcode zd_CALL_nn(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint16_t addr = opcode[1] + (((uint16_t)opcode[2]) << 8);
    sprintf(rv.opcode_str, "CALL 0x%04X", addr);

    rv.flags = Z80D_TYPE_CONST_JUMP | Z80D_TYPE_NORMAL | Z80D_TYPE_CALL;
    rv.address_jump = addr;
    return rv;
}

z80d_opcode zd_CCF(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(rv.opcode_str, "CCF");
    return rv;
}

z80d_opcode zd_CPL(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(rv.opcode_str, "CPL");
    return rv;
}

z80d_opcode zd_DAA(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(rv.opcode_str, "DAA");
    return rv;
}

z80d_opcode zd_DEC_IXY(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    if (opcode[0] == 0xDD)
        strcpy(rv.opcode_str, "DEC IX");
    else
        strcpy(rv.opcode_str, "DEC IY");
    return rv;
}

z80d_opcode zd_DEC_IXYp(const uint8_t* opcode) { 
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const int8_t d = (int8_t)opcode[2];
    if(opcode[0] == 0xDD)
        sprintf(rv.opcode_str, "DEC (IX%+d)", d);
    else
        sprintf(rv.opcode_str, "DEC (IY%+d)", d);
    return rv;
}

z80d_opcode zd_DEC_r(const uint8_t* opcode) { 
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    const uint8_t y = (opcode[0] >> 3) & 0x07;
    sprintf(rv.opcode_str, "DEC %s", z80d_r[y]);
    return rv;
}

z80d_opcode zd_DEC_rp(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    const uint8_t p = (opcode[0] >> 4) & 0x03;
    sprintf(rv.opcode_str, "DEC %s", z80d_rp[p]);
    return rv; //1;
}

z80d_opcode zd_DI(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(rv.opcode_str, "DI");
    return rv; //1;
}

z80d_opcode zd_DJNZ_d(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    int8_t d = (int8_t)opcode[1];
    sprintf(rv.opcode_str, "DJNZ %+d", d);

    rv.flags = Z80D_TYPE_REL_JUMP | Z80D_TYPE_NORMAL;
    rv.immediate = d;
    return rv; //2;
}

z80d_opcode zd_EI(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(rv.opcode_str, "EI");
    return rv; //1;
}

z80d_opcode zd_EX(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(rv.opcode_str, "EX AF, AF'");
    return rv; //1;
}

z80d_opcode zd_EX_DE_HL(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(rv.opcode_str, "EX DE, HL");
    return rv; //1;
}

z80d_opcode zd_EX_SPp_HL(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(rv.opcode_str, "EX (SP), HL");
    return rv; //1;
}

z80d_opcode zd_EX_SPp_IXY(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    if (opcode[0] == 0xDD)
        strcpy(rv.opcode_str, "EX (SP), IX");
    else
        strcpy(rv.opcode_str, "EX (SP), IY");
    return rv; //2;
}

z80d_opcode zd_EXX(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(rv.opcode_str, "EXX");
    return rv; //1;
}

z80d_opcode zd_HALT(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(rv.opcode_str, "HALT");
    return rv; //1;
}

z80d_opcode zd_IM(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t y = (opcode[1] >> 3) & 0x07;
    sprintf(rv.opcode_str, "IM %s", z80d_im[y]);
    return rv; //2;
}

z80d_opcode zd_IN_A_np(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t n = opcode[1];
    sprintf(rv.opcode_str, "IN A, (0x%02X)", n);
    return rv; //2;
}

z80d_opcode zd_IN_r_Cp(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t y = (opcode[1] >> 3) & 0x07;
    sprintf(rv.opcode_str, "IN %s, (C)", z80d_r[y]);
    return rv; //2;
}

z80d_opcode zd_INC_IXY(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    if (opcode[0] == 0xDD)
        strcpy(rv.opcode_str, "INC IX");
    else
        strcpy(rv.opcode_str, "INC IY");
    return rv; //2;
}

z80d_opcode zd_INC_IXYp(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const int8_t d = (int8_t)opcode[2];
    if (opcode[0] == 0xDD)
        sprintf(rv.opcode_str, "INC (IX%+d)", d);
    else
        sprintf(rv.opcode_str, "INC (IY%+d)", d);
    return rv; //3;
}

z80d_opcode zd_INC_r(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    const uint8_t y = (opcode[0] >> 3) & 0x07;
    sprintf(rv.opcode_str, "INC %s", z80d_r[y]);
    return rv; //1;
}

z80d_opcode zd_INC_rp(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    const uint8_t p = (opcode[0] >> 4) & 0x03;
    sprintf(rv.opcode_str, "INC %s", z80d_rp[p]);
    return rv; //1;
}

z80d_opcode zd_JP_cc_nn(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint8_t y = (opcode[0] >> 3) & 0x07;
    const uint16_t addr = opcode[1] + (((uint16_t)opcode[2]) << 8);
    sprintf(rv.opcode_str, "JP %s, 0x%04X", z80d_cc[y], addr);

    rv.flags = Z80D_TYPE_CONST_JUMP | Z80D_TYPE_NORMAL;
    rv.address_jump = addr;
    return rv; //3;
}

z80d_opcode zd_JP_HLp(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    rv.flags = Z80D_TYPE_INDIRECT_JUMP;
    strcpy(rv.opcode_str, "JP HL");
    return rv; //1;
}

z80d_opcode zd_JP_IXYp(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    if (opcode[0] == 0xDD)
        strcpy(rv.opcode_str, ";Check!\nJP IX");
    else
        strcpy(rv.opcode_str, ";Check!\nJP IY");

    rv.flags = Z80D_TYPE_INDIRECT_JUMP;
    return rv; //2;
}

z80d_opcode zd_JP_nn(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint16_t addr = opcode[1] + (((uint16_t)opcode[2]) << 8);
    sprintf(rv.opcode_str, "JP 0x%04X", addr);

    rv.flags = Z80D_TYPE_CONST_JUMP;
    rv.address_jump = addr;
    return rv; //3;
}

z80d_opcode zd_JR_cc_d(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const int8_t d = (int8_t) opcode[1];
    const uint8_t y = (opcode[0] >> 3) & 0x07;
    sprintf(rv.opcode_str, "JR %s, %+d", z80d_cc[y - 4], d);

    rv.flags = Z80D_TYPE_REL_JUMP | Z80D_TYPE_NORMAL;
    rv.immediate = d;
    return rv; //2;
}

z80d_opcode zd_JR_d(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const int8_t d = (int8_t)opcode[1];
    sprintf(rv.opcode_str, "JR %+d", d);

    rv.flags = Z80D_TYPE_REL_JUMP;
    rv.immediate = d;
    return rv; //2;
}

z80d_opcode zd_LD_A_BCp(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(rv.opcode_str, "LD A, (BC)");
    return rv; //1;
}

z80d_opcode zd_LD_A_DEp(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(rv.opcode_str, "LD A, (DE)");
    return rv; //1;
}

z80d_opcode zd_LD_A_I(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    strcpy(rv.opcode_str, "LD A, I");
    return rv; //2;
}

z80d_opcode zd_LD_A_nnp(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint16_t addr = opcode[1] + (((uint16_t)opcode[2]) << 8);
    sprintf(rv.opcode_str, "LD A, (0x%04X)", addr);
    return rv; //3;
}

z80d_opcode zd_LD_A_R(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    strcpy(rv.opcode_str, "LD A, R");
    return rv; //2;
}

z80d_opcode zd_LD_BCp_A(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(rv.opcode_str, "LD (BC), A");
    return rv; //1;
}

z80d_opcode zd_LD_DEp_A(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(rv.opcode_str, "LD (DE), A");
    return rv; //1;
}

z80d_opcode zd_LD_HL_nnp(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint16_t addr = opcode[1] + (((uint16_t)opcode[2]) << 8);
    sprintf(rv.opcode_str, "LD HL, (0x%04X)", addr);
    return rv; //3;
}

z80d_opcode zd_LD_I_A(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    strcpy(rv.opcode_str, "LD I, A");
    return rv; //2;
}

z80d_opcode zd_LD_IXY_nn(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const uint16_t addr = opcode[2] + (((uint16_t)opcode[3]) << 8);
    if (opcode[0] == 0xDD)
        sprintf(rv.opcode_str, "LD IX, 0x%04X", addr);
    else
        sprintf(rv.opcode_str, "LD IY, 0x%04X", addr);
    return rv; //4;
}

z80d_opcode zd_LD_IXY_nnp(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const uint16_t addr = opcode[2] + (((uint16_t)opcode[3]) << 8);
    if(opcode[0] == 0xDD)
        sprintf(rv.opcode_str, "LD IX, (0x%04X)", addr);
    else
        sprintf(rv.opcode_str, "LD IY, (0x%04X)", addr);
    return rv; //4;
}

z80d_opcode zd_LD_IXYH_n(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint8_t n = opcode[2];
    if(opcode[0] == 0xDD)
        sprintf(rv.opcode_str, "LD IXH, 0x%02X", n);
    else
        sprintf(rv.opcode_str, "LD IYH, 0x%02X", n);
    return rv; //3;
}

z80d_opcode zd_LD_IXYL_n(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint8_t n = opcode[2];
    if (opcode[0] == 0xDD)
        sprintf(rv.opcode_str, "LD IXL, 0x%02X", n);
    else
        sprintf(rv.opcode_str, "LD IYL, 0x%02X", n);
    return rv; //3;
}

z80d_opcode zd_LD_IXYp_n(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const uint8_t n = opcode[2];
    const int8_t d = (int8_t)opcode[3];
    if (opcode[0] == 0xDD)
        sprintf(rv.opcode_str, "LD (IX%+d), 0x%02X", d, n);
    else
        sprintf(rv.opcode_str, "LD (IY%+d), 0x%02X", d, n);
    return rv; //4;
}

z80d_opcode zd_LD_IXYp_r(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint8_t z = (opcode[1]) & 0x07;
    const int8_t  d = (int8_t)opcode[2];
    sprintf(rv.opcode_str, "LD (IX%+d), %s", d, z80d_r[z]);
    return rv; //3;
}

z80d_opcode zd_LD_nnp_A(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint16_t addr = opcode[1] + (((uint16_t)opcode[2]) << 8);
    sprintf(rv.opcode_str, "LD (0x%04X), A", addr);
    return rv; //3;
}

z80d_opcode zd_LD_nnp_HL(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint16_t addr = opcode[1] + (((uint16_t)opcode[2]) << 8);
    sprintf(rv.opcode_str, "LD (0x%04X), HL", addr);
    return rv; //3;
}

z80d_opcode zd_LD_nnp_IXY(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const uint16_t addr = opcode[2] + (((uint16_t)opcode[3]) << 8);
    if(opcode[0] == 0xDD)
        sprintf(rv.opcode_str, "LD (0x%04X), IX", addr);
    else
        sprintf(rv.opcode_str, "LD (0x%04X), IY", addr);
    return rv; //4;
}

z80d_opcode zd_LD_nnp_rp(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const uint8_t p = (opcode[1] >> 4) & 0x03;
    const uint16_t addr = opcode[2] + (((uint16_t)opcode[3])<<8);
    sprintf(rv.opcode_str, "LD (0x%04X), %s", addr, z80d_rp[p]);
    return rv; //4;
}

z80d_opcode zd_LD_R_A(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    strcpy(rv.opcode_str, "LD R, A");
    return rv; //2;
}

z80d_opcode zd_LD_r_IXYp(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    const uint8_t y = (opcode[1] >> 3) & 0x07;
    const int8_t  d = (int8_t)opcode[2];
	if (opcode[0] == 0xDD) {
		sprintf(rv.opcode_str, "LD %s, (IX%+d)", z80d_r[y], d);
	}
	else {
		sprintf(rv.opcode_str, "LD %s, (IY%+d)", z80d_r[y], d);
	}
    return rv; //3;
}

z80d_opcode zd_LD_r_n(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t n = opcode[1];
    const uint8_t y = (opcode[0] >> 3) & 0x07;
    sprintf(rv.opcode_str, "LD %s, 0x%02X", z80d_r[y], n);
    return rv; //2;
}

z80d_opcode zd_LD_r_r(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    const uint8_t y = (opcode[0] >> 3) & 0x07;
    const uint8_t z = opcode[0] & 0x07;
    sprintf(rv.opcode_str, "LD %s, %s", z80d_r[y], z80d_r[z]);
    return rv; //1;
}

z80d_opcode zd_LD_r_r_undoc(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t y = (opcode[1] >> 3) & 0x07;
    const uint8_t z = opcode[1] & 0x07;
    if (opcode[0] == 0xDD) {
        sprintf(rv.opcode_str, "LD %s, %s", z80d_r_ix[y], z80d_r_ix[z]);
    }
    else {
        sprintf(rv.opcode_str, "LD %s, %s", z80d_r_iy[y], z80d_r_iy[z]);
    }
    return rv; //1;
}

z80d_opcode zd_LD_rp_nn(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //3;

    uint8_t p = (opcode[0] >> 4) & 0x03;
    uint16_t nn = opcode[1] + (((uint16_t)opcode[2]) << 8);
    sprintf(rv.opcode_str, "LD %s, 0x%04X", z80d_rp[p], nn);
    return rv; //3;
}

z80d_opcode zd_LD_rp_nnp(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const uint8_t p = (opcode[1] >> 4) & 0x03;
    const uint16_t addr = opcode[2] + (((uint16_t)opcode[3])<<8);
    sprintf(rv.opcode_str, "LD %s, (0x%04X)", z80d_rp[p], addr);
    return rv; //4;
}

z80d_opcode zd_LD_SP_HL(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(rv.opcode_str, "LD SP, HL");
    return rv; //1;
}

z80d_opcode zd_LD_SP_IXY(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    if(opcode[0] == 0xDD)
        strcpy(rv.opcode_str, "LD SP, IX");
    else
        strcpy(rv.opcode_str, "LD SP, IY");
    return rv; //2;
}

z80d_opcode zd_NEG(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    strcpy(rv.opcode_str, "NEG");
    return rv; //2;
}

z80d_opcode zd_NOP(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(rv.opcode_str, "NOP");
    return rv; //1;
}

z80d_opcode zd_OUT_Cp_r(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t y = (opcode[1] >> 3) & 0x07;
    sprintf(rv.opcode_str, "OUT (C), %s", z80d_r[y]);
    return rv; //2;
}

z80d_opcode zd_OUT_np_A(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t n = opcode[1];
    sprintf(rv.opcode_str, "OUT (0x%02X), A", n);
    return rv; //2;
}

z80d_opcode zd_POP_IXY(const uint8_t* opcode) { 
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    if (opcode[0] == 0xDD)
        strcpy(rv.opcode_str, "POP IX");
    else
        strcpy(rv.opcode_str, "POP IY");
    return rv; //2;
}

z80d_opcode zd_POP_rp2(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    const uint8_t p = (opcode[0] >> 4) & 0x03;
    sprintf(rv.opcode_str, "POP %s", z80d_rp2[p]);
    return rv; //1;
}

z80d_opcode zd_PUSH_IXY(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    if (opcode[0] == 0xDD)
        strcpy(rv.opcode_str, "PUSH IX");
    else
        strcpy(rv.opcode_str, "PUSH IY");
    return rv; //2;
}

z80d_opcode zd_PUSH_rp2(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    const uint8_t p = (opcode[0] >> 4) & 0x03;
    sprintf(rv.opcode_str, "PUSH %s", z80d_rp2[p]);
    return rv; //1;
}

z80d_opcode zd_RES_b_IXYp(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const int8_t d = (int8_t)opcode[2];
    const uint8_t y = (opcode[3] >> 3) & 0x07;
	if (opcode[0] == 0xDD) {
		sprintf(rv.opcode_str, "RES %d, (IX%+d)", y, d);
	}
	else {
		sprintf(rv.opcode_str, "RES %d, (IY%+d)", y, d);
	}
    return rv; //4;
}

z80d_opcode zd_RES_b_r(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t y = (opcode[1] >> 3) & 0x07;
    const uint8_t z = (opcode[1]) & 0x07;
    sprintf(rv.opcode_str, "RES %d, %s", y, z80d_r[z]);
    return rv; //2;
}

z80d_opcode zd_RET(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(rv.opcode_str, "RET");
    rv.flags = Z80D_TYPE_INDIRECT_JUMP | Z80D_TYPE_RETURN;
    return rv; //1;
}

z80d_opcode zd_RET_cc(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    const uint8_t y = (opcode[0] >> 3) & 0x07;
    sprintf(rv.opcode_str, "RET %s", z80d_cc[y]);
    rv.flags = Z80D_TYPE_INDIRECT_JUMP | Z80D_TYPE_NORMAL | Z80D_TYPE_RETURN;
    return rv; //1;
}

z80d_opcode zd_RETI(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    strcpy(rv.opcode_str, "RETI");
    rv.flags = Z80D_TYPE_INDIRECT_JUMP | Z80D_TYPE_RETURN;
    return rv; //2;
}

z80d_opcode zd_RETN(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    strcpy(rv.opcode_str, "RETN");
    rv.flags = Z80D_TYPE_INDIRECT_JUMP | Z80D_TYPE_RETURN;
    return rv; //2;
}

z80d_opcode zd_RLA(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(rv.opcode_str, "RLA");
    return rv; //1;
}

z80d_opcode zd_RLCA(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(rv.opcode_str, "RLCA");
    return rv; //1;
}

z80d_opcode zd_RLD(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(rv.opcode_str, "RLD");
    return rv; //1;
}

z80d_opcode zd_rot(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t y = (opcode[1] >> 3) & 0x07;
    const uint8_t z = opcode[1] & 0x07;
    sprintf(rv.opcode_str, "%s %s", z80d_rot[y], z80d_r[z]);
    return rv; //2;
}

z80d_opcode zd_rot_IXYp(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const uint8_t d = opcode[2];
    const uint8_t y = (opcode[3] >> 3) & 0x07;
	if (opcode[0] == 0xDD) {
		sprintf(rv.opcode_str, "%s (IX%+d)", z80d_rot[y], d);
	}
	else {
		sprintf(rv.opcode_str, "%s (IY%+d)", z80d_rot[y], d);
	}
    return rv; //4;
}

z80d_opcode zd_RRA(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    strcpy(rv.opcode_str, "RRA");
    return rv; //2;
}

z80d_opcode zd_RRCA(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(rv.opcode_str, "RRCA");
    return rv; //1;
}

z80d_opcode zd_RRD(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    strcpy(rv.opcode_str, "RRD");
    return rv; //2;
}

z80d_opcode zd_RST_y(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    const uint8_t y = (opcode[0] >> 3) & 7;
    sprintf(rv.opcode_str, "RST 0x%04X", y * 8);
    rv.flags = Z80D_TYPE_CONST_JUMP;
    rv.address_jump = y * 8;
    return rv; //1;
}

z80d_opcode zd_SBC_HL_rp(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t p = (opcode[1] >> 4) & 0x03;
    sprintf(rv.opcode_str, "SBC HL, %s", z80d_rp[p]);
    return rv; //2;
}

z80d_opcode zd_SCF(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //1;

    strcpy(rv.opcode_str, "SCF");
    return rv; //1;
}

z80d_opcode zd_SET_b_IXYp(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const int8_t d = (int8_t)opcode[2];
    const uint8_t y = (opcode[3] >> 3) & 0x07;
	if (opcode[0] == 0xDD) {
		sprintf(rv.opcode_str, "SET %d, (IX%+d)", y, d);
	}
	else {
		sprintf(rv.opcode_str, "SET %d, (IY%+d)", y, d);
	}
    return rv; //4;
}

z80d_opcode zd_SET_b_r(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //2;

    const uint8_t y = (opcode[1] >> 3) & 0x07;
    const uint8_t z = (opcode[1]) & 0x07;
    sprintf(rv.opcode_str, "SET %d, %s", y, z80d_r[z]);
    return rv; //2;
}

z80d_opcode zd_LD_RES(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const int8_t d = (int8_t)opcode[2];
    const uint8_t y = (opcode[3] >> 3) & 0x07;
    const uint8_t z = (opcode[3]) & 0x07;
    if(opcode[0] == 0xDD)
        sprintf(rv.opcode_str, "LD %s, RES %d, (IX%+d)", z80d_r[z], y, d);
    else
        sprintf(rv.opcode_str, "LD %s, RES %d, (IY%+d)", z80d_r[z], y, d);
    return rv; //4;
}
z80d_opcode zd_LD_ROT(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const int8_t d = (int8_t)opcode[2];
    const uint8_t y = (opcode[3] >> 3) & 0x07;
    const uint8_t z = (opcode[3]) & 0x07;
    if (opcode[0] == 0xDD)
        sprintf(rv.opcode_str, "LD %s, %s (IX%+d)", z80d_r[z], z80d_rot[y], d);
    else
        sprintf(rv.opcode_str, "LD %s, %s (IY%+d)", z80d_r[z], z80d_rot[y], d);
    return rv; //4;
}
z80d_opcode zd_LD_SET(const uint8_t* opcode) {
    z80d_opcode rv = defaultOpcode();
    rv.size = opcode_size(opcode); //4;

    const int8_t d = (int8_t)opcode[2];
    const uint8_t y = (opcode[3] >> 3) & 0x07;
    const uint8_t z = (opcode[3]) & 0x07;
    if (opcode[0] == 0xDD)
        sprintf(rv.opcode_str, "LD %s, SET %d, (IX%+d)", z80d_r[z], y, d);
    else
        sprintf(rv.opcode_str, "LD %s, SET %d, (IY%+d)", z80d_r[z], y, d);
    return rv; //4;
}
