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
 * @file z80_macros.h
 * @brief Helper macros and constants for the z80 emulator.
 *
 */
#ifndef __Z80_MACROS_H
#define __Z80_MACROS_H

#ifdef __cplusplus
extern "C" {
#endif

//Data needed for macros
///Parity LUT.
static const uint8_t z80_parity_lut[256] = {
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0
};

//Opcode subdivision macro
#define Z80_OPCODE_SUBDIV     const uint8_t x[4] = { z80.opcode[0] >> 6, z80.opcode[1] >> 6, z80.opcode[2] >> 6, z80.opcode[3] >> 6 };\
const uint8_t y[4] = { (z80.opcode[0] >> 3) & 0x7, (z80.opcode[1] >> 3) & 0x7, (z80.opcode[2] >> 3) & 0x7, (z80.opcode[3] >> 3) & 0x7 };\
const uint8_t z[4] = { z80.opcode[0] & 0x7, z80.opcode[1] & 0x7, z80.opcode[2] & 0x7, z80.opcode[3] & 0x7 };\
const uint8_t p[4] = { (z80.opcode[0] >> 4) & 0x3, (z80.opcode[1] >> 4) & 0x3, (z80.opcode[2] >> 4) & 0x3, (z80.opcode[3] >> 4) & 0x3 };\
const uint8_t q[4] = { z80.opcode[0] & (1 << 3), z80.opcode[1] & (1 << 3), z80.opcode[2] & (1 << 3), z80.opcode[3] & (1 << 3) };
//------------------------

//Stage enumerations
#define Z80_STAGE_RESET 0
#define Z80_STAGE_M1 1
#define Z80_STAGE_M2 2
#define Z80_STAGE_M3 3

//ALU operations
#define Z80_ALUOP_ADD 0
#define Z80_ALUOP_ADC 1
#define Z80_ALUOP_SUB 2
#define Z80_ALUOP_SBC 3
#define Z80_ALUOP_AND 4
#define Z80_ALUOP_XOR 5
#define Z80_ALUOP_OR  6
#define Z80_ALUOP_CP  7

//Sub-byte opcode masks
#define Z80_OPCODE_X(X) ((X&0x03)<<6)
#define Z80_OPCODE_Y(Y) ((Y&0x07)<<3)
#define Z80_OPCODE_Z(Z) ((Z&0x07))
#define Z80_OPCODE_P(P) ((P&0x03)<<4)
#define Z80_OPCODE_Q(Q) ((Q&0x01)<<3)

#define Z80_OPCODE_XYZ(X,Y,Z)    (Z80_OPCODE_X(X) | Z80_OPCODE_Y(Y) | Z80_OPCODE_Z(Z))
#define Z80_OPCODE_XPQZ(X,P,Q,Z) (Z80_OPCODE_XYZ(X,0,Z) | Z80_OPCODE_P(P) | Z80_OPCODE_Q(Q))
#define Z80_OPCODE_XZ(X,Z)       (Z80_OPCODE_XYZ(X,0,Z))
#define Z80_OPCODE_YZ(Y,Z)       (Z80_OPCODE_XYZ(0,Y,Z))

#define Z80_OPCODE_X_MASK (3<<6)
#define Z80_OPCODE_Y_MASK (7<<3)
#define Z80_OPCODE_Z_MASK (7)
#define Z80_OPCODE_P_MASK (3<<4)
#define Z80_OPCODE_Q_MASK (1<<3)

//Register macros. *BEWARE* some macros are endianness sensitive!
//Little endian (x86) is presumed unless noted otherwise.

//Address H/L
#define Z80_ADDRL (*((uint8_t*)(&z80_address)))
#define Z80_ADDRH (*(((uint8_t*)(&z80_address)) + 1))

//I (endianness insensitive)
#define Z80_I  (z80.rI)

//R (endianness insensitive)
#define Z80_R  (z80.rR)

//AF
#define Z80_A  (z80.rAF[1])
#define Z80_Ap (z80.rAF[3])
#define Z80_F  (z80.rAF[0])
#define Z80_Fp (z80.rAF[2])
#define Z80_AF  (*((uint16_t*) z80.rAF))
#define Z80_AFp (*((uint16_t*) (z80.rAF + 2)))

//BC
#define Z80_B  z80.rBC[1]
#define Z80_C  z80.rBC[0]
#define Z80_Bp z80.rBC[3]
#define Z80_Cp z80.rBC[2]
#define Z80_BC  (*((uint16_t*) z80.rBC))
#define Z80_BCp (*((uint16_t*) (z80.rBC + 2)))

//DE
#define Z80_D  z80.rDE[1]
#define Z80_E  z80.rDE[0]
#define Z80_Dp z80.rDE[3]
#define Z80_Ep z80.rDE[2]
#define Z80_DE  (*((uint16_t*) z80.rDE))
#define Z80_DEp (*((uint16_t*) (z80.rDE + 2)))

//HL
#define Z80_H  z80.rHL[1]
#define Z80_L  z80.rHL[0]
#define Z80_Hp z80.rHL[3]
#define Z80_Lp z80.rHL[2]
#define Z80_HL  (*((uint16_t*) z80.rHL))
#define Z80_HLp (*((uint16_t*) (z80.rHL + 2)))

//WZ
#define Z80_W  z80.rWZ[1]
#define Z80_Z  z80.rWZ[0]
#define Z80_Wp z80.rWZ[3]
#define Z80_Zp z80.rWZ[2]
#define Z80_WZ  (*((uint16_t*) z80.rWZ))
#define Z80_WZp (*((uint16_t*) (z80.rWZ + 2)))

//IX/IY/SP/PC (endianness insensitive)
#define Z80_IX z80.rIX
#define Z80_IY z80.rIY
#define Z80_SP z80.rSP
#define Z80_PC z80.rPC

//Index bytes
#define Z80_IXL (*((uint8_t*)&Z80_IX))
#define Z80_IXH (*(((uint8_t*)&Z80_IX) + 1))
#define Z80_IYL (*((uint8_t*)&Z80_IY))
#define Z80_IYH (*(((uint8_t*)&Z80_IY) + 1))

//IX/IY selection macro. IX if param == 0xDD use IX; IY otherwise.
#define Z80_INDIRECT(PREFIX) (*(PREFIX == 0xDD ? &Z80_IX : &Z80_IY))

// --- Flag masks
#define Z80_FLAG_SIGN     (1<<7)
#define Z80_FLAG_ZERO     (1<<6)
#define Z80_FLAG_UNK5     (1<<5)
#define Z80_FLAG_HC       (1<<4)
#define Z80_FLAG_UNK3     (1<<3)
#define Z80_FLAG_PARITY   (1<<2)
#define Z80_FLAG_SUBTRACT (1<<1)
#define Z80_FLAG_CARRY    (1)

// --- Flag update macros
#define Z80_SETFLAG_SIGN(X) (((X)&(1<<7)) ? Z80_FLAG_SIGN : 0) /**<-- [S] Set sign flag (bit 7)*/
#define Z80_SETFLAG_ZERO(X) (((X) == 0) ? Z80_FLAG_ZERO : 0) /**<-- [Z] Set Zero flag (bit 6)*/
//#define Z80_SETFLAG_HC(O,N) (((O & (1 << 3)) == 0) && ((N) & (1 << 3)) ? Z80_FLAG_HC : 0) /**<-- [H] Set Half-carry flag (bit 4)*/

#define Z80_SETFLAG_HALF_CARRY(OP1,OP2)    (((((OP1)&0x0F) + ((OP2)&0x0F)) > 0x0F) ? Z80_FLAG_HC : 0)
#define Z80_SETFLAG_HALF_BORROW(OP1,OP2)   ((((((OP1)&0xF0) - ((OP2)&0xF0)))&0xF0) > (((OP1)&0xF0)) ? Z80_FLAG_HC : 0)
#define Z80_SETFLAG_HALF_CARRY_16(OP1,OP2)  (((((OP1)&0x0F00) + ((OP2)&0x0F00)) > 0x0F00) ? Z80_FLAG_HC : 0)
#define Z80_SETFLAG_HALF_BORROW_16(OP1,OP2)   ((((((OP1)&0xF000) - ((OP2)&0xF000)))&0xF000) > (((OP1)&0xF000)) ? Z80_FLAG_HC : 0)


#define Z80_SETFLAG_PARITY(X) (z80_parity_lut[(X)] ? Z80_FLAG_PARITY : 0) /**<-- [P] Set parity flag (bit 2)*/
#define Z80_SETFLAG_OVERFLOW(O,N) (((int16_t)O) > ((int16_t)N) ? 0 : Z80_FLAG_PARITY) /**<-- [V] Set overflow flag (bit 2)*/
#define Z80_SETFLAG_SUBTRACT(A) (A ? Z80_FLAG_SUBTRACT : 0) /**<-- [N] Set Add/Subtract flag (bit 1)*/
#define Z80_SETFLAG_CARRY(O,N) (O > N ? Z80_FLAG_CARRY : 0) /**<-- [C] Set Carry flag, adition (bit 0)*/
#define Z80_SETFLAG_BORROW(O,N) (O < N ? Z80_FLAG_CARRY : 0) /**<-- [C] Set carry flag, subtraction, (bit 0)*/
///@bug Decimal Adjust Accumulate is not implemented

// --- Inverted flag masks (for bit clearing)
#define Z80_CLRFLAG_SIGN     (Z80_FLAG_SIGN     ^ 0xFF)
#define Z80_CLRFLAG_ZERO     (Z80_FLAG_ZERO     ^ 0xFF)
#define Z80_CLRFLAG_UNK5     (Z80_FLAG_UNK5     ^ 0xFF)
#define Z80_CLRFLAG_HC       (Z80_FLAG_HC       ^ 0xFF)
#define Z80_CLRFLAG_UNK3     (Z80_FLAG_UNK3     ^ 0xFF)
#define Z80_CLRFLAG_PARITY   (Z80_FLAG_PARITY   ^ 0xFF)
#define Z80_CLRFLAG_SUBTRACT (Z80_FLAG_SUBTRACT ^ 0xFF)
#define Z80_CLRFLAG_CARRY    (Z80_FLAG_CARRY    ^ 0xFF)

// --- Breakpoint macros ---
#define Z80_BREAK_PC     (1 << 0) /*Break on z80 exec (PC)*/
#define Z80_BREAK_RD     (1 << 1) /*Break on memory read*/
#define Z80_BREAK_WR     (1 << 2) /*Break on memory write*/
#define Z80_BREAK_IO_RD  (1 << 3) /*Break on IO read*/
#define Z80_BREAK_IO_WR  (1 << 4) /*Break on IO write*/
#define Z80_BREAK_IO_16B (1 << 5) /*Use 16-bit IO Addressing*/

// --- IX/IY register lut selection macros ---
//This macros provide a pointer to the IX/IY version of the register
//look-up tables. It is presumed a z80 struct is present wherever
//this macro is invoked. Note that any non-0xDD prefix will be
//considered a 0xFD prefix so the decoder must discriminate invalid
//prefixes first.  Return type is `uint16_t* const *`
#define Z80_PREFIX_R_LUT   (((z80.opcode[0]) == 0xDD) ? z80_r_ix : z80_r_iy)
#define Z80_PREFIX_RP_LUT  (((z80.opcode[0]) == 0xDD) ? z80_rp_ix : z80_rp_iy) 
#define Z80_PREFIX_RP2_LUT (((z80.opcode[0]) == 0xDD) ? z80_rp2_ix : z80_rp2_iy) 

// --- Constants ---
#define Z80_ADDRESS_SIZE (65536) /*Address space. 2^16*/
#ifdef __cplusplus
}
#endif

#endif