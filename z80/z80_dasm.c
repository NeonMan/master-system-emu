#include "z80_dasm.h"
#include "z80_macros.h"
#include <string.h>
#include <stdio.h>

const char* z80d_r[8] = { "B", "C", "D", "E", "H", "L", "(HL)", "A" };
const char* z80d_rp[4] = { "BC", "DE", "HL", "SP" };
const char* z80d_rp2[4] = { "DB", "DE", "HL", "AF" };
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

int z80d_decode_ED(uint8_t* opcode, unsigned int size, char* result){
    const uint8_t x[4] = { opcode[0] >> 6, opcode[1] >> 6, opcode[2] >> 6, opcode[3] >> 6 };
    const uint8_t y[4] = { (opcode[0] >> 3) & 0x7, (opcode[1] >> 3) & 0x7, (opcode[2] >> 3) & 0x7, (opcode[3] >> 3) & 0x7 };
    const uint8_t z[4] = { opcode[0] & 0x7, opcode[1] & 0x7, opcode[2] & 0x7, opcode[3] & 0x7 };
    const uint8_t p[4] = { (opcode[0] >> 4) & 0x3, (opcode[1] >> 4) & 0x3, (opcode[2] >> 4) & 0x3, (opcode[3] >> 4) & 0x3 };
    const uint8_t q[4] = { opcode[0] & (1 << 3), opcode[1] & (1 << 3), opcode[2] & (1 << 3), opcode[3] & (1 << 3) };
    const uint16_t nn = *((uint16_t*)(opcode + 1));
    const uint16_t nn2 = *((uint16_t*)(opcode + 2));

    int rv = 0;
    char tmp_str[100];
    char* tmp_ptr = tmp_str;
    memset(tmp_str, '\0', 100);

    switch (opcode[1] & (Z80_OPCODE_Z_MASK | Z80_OPCODE_X_MASK)){
    case Z80_OPCODE_XZ(0, 0):
    case Z80_OPCODE_XZ(0, 1):
    case Z80_OPCODE_XZ(0, 2):
    case Z80_OPCODE_XZ(0, 3):
    case Z80_OPCODE_XZ(0, 4):
    case Z80_OPCODE_XZ(0, 5):
    case Z80_OPCODE_XZ(0, 6):
    case Z80_OPCODE_XZ(0, 7):

    case Z80_OPCODE_XZ(3, 0):
    case Z80_OPCODE_XZ(3, 1):
    case Z80_OPCODE_XZ(3, 2):
    case Z80_OPCODE_XZ(3, 3):
    case Z80_OPCODE_XZ(3, 4):
    case Z80_OPCODE_XZ(3, 5):
    case Z80_OPCODE_XZ(3, 6):
    case Z80_OPCODE_XZ(3, 7):
        sprintf(tmp_str, "NONI/NOP");
        rv = 2;
        break;

    case Z80_OPCODE_XZ(1, 0):
        if (y[1] != 6){
            sprintf(tmp_str, "IN %s, (C)", z80d_r[y[1]]);
        }
        else{
            sprintf(tmp_str, "IN (C)");
        }
        rv = 2;
        break;

    case Z80_OPCODE_XZ(1, 1):
        if (y[1] != 6){
            sprintf(tmp_str, "OUT %s, (C)", z80d_r[y[1]]);
        }
        else{
            sprintf(tmp_str, "OUT (C)");
        }
        rv = 2;
        break;

    case Z80_OPCODE_XZ(1, 2):
        if (!q[1]){
            sprintf(tmp_str, "SBC HL, %s", z80d_rp[p[1]]);
        }
        else{
            sprintf(tmp_str, "ADC HL, %s", z80d_rp[p[1]]);
        }
        rv = 2;
        break;

    case Z80_OPCODE_XZ(1, 3):
        if (!q[1]){
            sprintf(tmp_str, "LD (0x%04X), %s", nn2, z80d_rp[p[1]]);
        }
        else{
            sprintf(tmp_str, "LD %s, (0x%04X)", z80d_rp[p[1]], nn2);
        }
        rv = 4;
        break;

    case Z80_OPCODE_XZ(1, 4):
        sprintf(tmp_str, "NEG");
        rv = 2;
        break;

    case Z80_OPCODE_XZ(1, 5):
        if (y[1] != 1){
            sprintf(tmp_str, "RETN");
        }
        else{
            sprintf(tmp_str, "RETI");
        }
        rv = 2;
        break;

    case Z80_OPCODE_XZ(1, 6):
        sprintf(tmp_str, "IM %s", z80d_im[y[1]]);
        rv = 2;
        break;

    case Z80_OPCODE_XZ(1, 7):
        switch (y[1]){
        case 0:
            sprintf(tmp_str, "LD I, A");
            rv = 2;
            break;
        case 1:
            sprintf(tmp_str, "LD R, A");
            rv = 2;
            break;
        case 2:
            sprintf(tmp_str, "LD A, I");
            rv = 2;
            break;
        case 3:
            sprintf(tmp_str, "LD A, R");
            rv = 2;
            break;
        case 4:
            sprintf(tmp_str, "RRD");
            rv = 2;
            break;
        case 5:
            sprintf(tmp_str, "RLD");
            rv = 2;
            break;
        case 6:
        case 7:
            sprintf(tmp_str, "NOP");
            rv = 2;
            break;
        }
        break;

    case Z80_OPCODE_XZ(2, 0):
    case Z80_OPCODE_XZ(2, 1):
    case Z80_OPCODE_XZ(2, 2):
    case Z80_OPCODE_XZ(2, 3):
    case Z80_OPCODE_XZ(2, 4):
    case Z80_OPCODE_XZ(2, 5):
    case Z80_OPCODE_XZ(2, 6):
    case Z80_OPCODE_XZ(2, 7):
        sprintf(tmp_str, "%s", *(z80d_bli + z[1] + ((y[1] - 4) * 4)));
        rv = 2;
        break;
    }
    strncpy(result, tmp_str, size - 1);
    return rv;
}

int z80d_decode(uint8_t* opcode, unsigned int size, char* result){
    const uint8_t x[4] = { opcode[0] >> 6, opcode[1] >> 6, opcode[2] >> 6, opcode[3] >> 6 };
    const uint8_t y[4] = { (opcode[0] >> 3) & 0x7, (opcode[1] >> 3) & 0x7, (opcode[2] >> 3) & 0x7, (opcode[3] >> 3) & 0x7 };
    const uint8_t z[4] = { opcode[0] & 0x7, opcode[1] & 0x7, opcode[2] & 0x7, opcode[3] & 0x7 };
    const uint8_t p[4] = { (opcode[0] >> 4) & 0x3, (opcode[1] >> 4) & 0x3, (opcode[2] >> 4) & 0x3, (opcode[3] >> 4) & 0x3 };
    const uint8_t q[4] = { opcode[0] & (1 << 3), opcode[1] & (1 << 3), opcode[2] & (1 << 3), opcode[3] & (1 << 3) };
    const uint16_t nn = *((uint16_t*)(opcode + 1));
    const uint16_t nn2 = *((uint16_t*)(opcode + 2));

    int rv = 0;
    char tmp_str[100];
    char* tmp_ptr = tmp_str;
    memset(tmp_str, '\0', 100);
    result[0] = '\0';

    switch (opcode[0] & (Z80_OPCODE_Z_MASK | Z80_OPCODE_X_MASK)){
    case Z80_OPCODE_XZ(0, 0):
        switch (y[0]){
        case 0:
            sprintf(tmp_str, "NOP");
            rv = 1;
            break;
        case 1:
            sprintf(tmp_str, "EX AF, AF'");
            rv = 1;
            break;
        case 2:
            sprintf(tmp_str, "DJNZ 0x%02X; (%d)", opcode[1], opcode[1]);
            rv = 2;
            break;
        case 3:
            sprintf(tmp_str, "JR 0x%02X; (%d)", opcode[1], opcode[1]);
            rv = 2;
            break;
        default:
            sprintf(tmp_str, "JR %s, 0x%02X; (%d)", z80d_cc[y[0] - 4], opcode[1], opcode[1]);
            rv = 2;
            break;
        }
        break;

    case Z80_OPCODE_XZ(0, 1):
        if (!q[0]){
            sprintf(tmp_str, "LD %s, 0x%04X", z80d_rp[p[0]], nn); //LD rp, nn
            rv = 3;
            break;
        }
        else{
            sprintf(tmp_str, "ADD HL, %s", z80d_rp[p[0]]); //LD HL, rp
            rv = 1;
            break;
        }
        break;

    case Z80_OPCODE_XZ(0, 2):
        if (!q[0]){
            if (p[0] == 0) { sprintf(tmp_str, "LD (BC), A"); rv = 1; }
            else if (p[0] == 1) { sprintf(tmp_str, "LD (DE), A"); rv = 1; }
            else if (p[0] == 2) { sprintf(tmp_str, "LD (0x%04X), HL", nn); rv = 3; }
            else if (p[0] == 3) { sprintf(tmp_str, "LD (0x%04X), A", nn); rv = 3; }
        }
        else{
            if (p[0] == 0) { sprintf(tmp_str, "LD A, (BC)"); rv = 1; }
            else if (p[0] == 1) { sprintf(tmp_str, "LD A, (DE)"); rv = 1; }
            else if (p[0] == 2) { sprintf(tmp_str, "LD HL, (0x%04X)", nn); rv = 3; }
            else if (p[0] == 3) { sprintf(tmp_str, "LD A, (0x%04X)", nn); rv = 3; }
        }
        break;

    case Z80_OPCODE_XZ(0, 3):
        if (!q[0]){
            sprintf(tmp_str, "INC %s", z80d_rp[p[0]]);
            rv = 1;
        }
        else{
            sprintf(tmp_str, "DEC %s", z80d_rp[p[0]]);
            rv = 1;
        }
        break;

    case Z80_OPCODE_XZ(0, 4):
        sprintf(tmp_str, "INC %s", z80d_r[y[0]]);
        rv = 1;
        break;

    case Z80_OPCODE_XZ(0, 5):
        sprintf(tmp_str, "DEC %s", z80d_r[y[0]]);
        rv = 1;
        break;

    case Z80_OPCODE_XZ(0, 6):
        sprintf(tmp_str, "LD %s, 0x%02X", z80d_r[y[0]], opcode[1]);
        rv = 2;
        break;

    case Z80_OPCODE_XZ(0, 7):
        switch (y[0]){
        case 0:
            sprintf(tmp_str, "RLCA");
            rv = 1;
            break;
        case 1:
            sprintf(tmp_str, "RRCA");
            rv = 1;
            break;
        case 2:
            sprintf(tmp_str, "RLA");
            rv = 1;
            break;
        case 3:
            sprintf(tmp_str, "RRA");
            rv = 1;
            break;
        case 4:
            sprintf(tmp_str, "DAA");
            rv = 1;
            break;
        case 5:
            sprintf(tmp_str, "CPL");
            rv = 1;
            break;
        case 6:
            sprintf(tmp_str, "SCF");
            rv = 1;
            break;
        case 7:
            sprintf(tmp_str, "CCF");
            rv = 1;
            break;
        }
        break;

    case Z80_OPCODE_XZ(1, 0):
    case Z80_OPCODE_XZ(1, 1):
    case Z80_OPCODE_XZ(1, 2):
    case Z80_OPCODE_XZ(1, 3):
    case Z80_OPCODE_XZ(1, 4):
    case Z80_OPCODE_XZ(1, 5):
    case Z80_OPCODE_XZ(1, 7):
        sprintf(tmp_str, "LD %s, %s", z80d_r[y[0]], z80d_r[z[0]]);
        rv = 1;
        break;

    case Z80_OPCODE_XZ(1, 6):
        if (y[0] == 6)
            sprintf(tmp_str, "HALT");
        else
            sprintf(tmp_str, "LD %s, %s", z80d_r[y[0]], z80d_r[z[0]]);
        rv = 1;
        break;

    case Z80_OPCODE_XZ(2, 0):
    case Z80_OPCODE_XZ(2, 1):
    case Z80_OPCODE_XZ(2, 2):
    case Z80_OPCODE_XZ(2, 3):
    case Z80_OPCODE_XZ(2, 4):
    case Z80_OPCODE_XZ(2, 5):
    case Z80_OPCODE_XZ(2, 6):
    case Z80_OPCODE_XZ(2, 7):
        sprintf(tmp_str, "%s %s", z80d_alu[y[0]], z80d_r[z[0]]); //ALU
        rv = 1;
        break;

    case Z80_OPCODE_XZ(3, 0):
        sprintf(tmp_str, "RET %s", z80d_cc[y[0]]);
        rv = 1;
        break;

    case Z80_OPCODE_XZ(3, 1):
        if (!q[0]){
            sprintf(tmp_str, "POP %s", z80d_rp2[p[0]]);
            rv = 1;
        }
        else{
            if      (p[0] == 0) sprintf(tmp_str, "RET");
            else if (p[0] == 1) sprintf(tmp_str, "EXX");
            else if (p[0] == 2) sprintf(tmp_str, "JP HL");
            else if (p[0] == 3) sprintf(tmp_str, "LD SP, HL");
            rv = 1;
        }
        break;

    case Z80_OPCODE_XZ(3, 2):
        sprintf(tmp_str, "JP %s, 0x04X", z80d_cc[y[0]], nn);
        rv = 3;
        break;

    case Z80_OPCODE_XZ(3, 3):
        switch (y[0]){
        case 0:
            sprintf(tmp_str, "JP 0x%04X", nn);
            rv = 3;
            break;
        case 1:
            sprintf(tmp_str, "!!CB!!");
            break;
        case 2:
            sprintf(tmp_str, "OUT (0x%02X), A", opcode[1]);
            rv = 2;
            break;
        case 3:
            sprintf(tmp_str, "IN A, (0x%02X)", opcode[1]);
            rv = 2;
            break;
        case 4:
            sprintf(tmp_str, "EX (SP), HL");
            rv = 1;
            break;
        case 5:
            sprintf(tmp_str, "EX DE, HL");
            rv = 1;
            break;
        case 6:
            sprintf(tmp_str, "DI");
            rv = 1;
            break;
        case 7:
            sprintf(tmp_str, "EI");
            rv = 1;
            break;
        }
        break;

    case Z80_OPCODE_XZ(3, 4):
        sprintf(tmp_str, "CALL %s, 0x%04X", z80d_cc[y[0]], nn);
        rv = 3;
        break;

    case Z80_OPCODE_XZ(3, 5):
        if (!q[0]){
            sprintf(tmp_str, "PUSH %s", z80d_rp2[p[0]]);
            rv = 1;
        }
        else{
            if (p[0] == 0) { sprintf(tmp_str, "CALL 0x%04X", nn); rv = 3;  }
            else if (p[0] == 1) { sprintf(tmp_str, "!!DD!!"); rv = 1; }
            else if (p[0] == 2) { rv = z80d_decode_ED(opcode, 100, tmp_str); }
            else if (p[0] == 3) { sprintf(tmp_str, "!!FD!!"); rv = 1; }
        }
        break;

    case Z80_OPCODE_XZ(3, 6):
        sprintf(tmp_str, "%s %02X; (%d)", z80d_alu[y[0]], opcode[1], opcode[1]);
        rv = 2;
        break;

    case Z80_OPCODE_XZ(3, 7):
        sprintf(tmp_str, "RST 0x02X", y[0]);
        rv = 1;
        break;

    default:
        sprintf(tmp_str, "!!!");
        rv = 1;
    }
    strncpy(result, tmp_str, size - 1);
    return 0;
}