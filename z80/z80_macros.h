#ifndef __Z80_MACROS_H
#define __Z80_MACROS_H


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
#define Z80_OPCODE_XZ(X,Z) (((X&0x3)<<6)|(Z&0x7))
#define Z80_OPCODE_YZ(Y,Z) (((Y&0x7)<<3)|(Z&0x7))
#define Z80_OPCODE_X_MASK (3<<6)
#define Z80_OPCODE_Y_MASK (7<<3)
#define Z80_OPCODE_Z_MASK (7)

//Little endian
#define Z80_LITTLE_ENDIAN(X) (*((uint16_t*)X))
//Big endian
#define Z80_BIG_ENDIAN(X) ((*((uint8_t*)X)) + (((uint16_t)(*((uint8_t*)(X + 1))))<<8))
//To host endian
///@bug use macros to select the correct endianness
#define Z80_HOST_ENDIAN(X) Z80_LITTLE_ENDIAN(X)

//Register macros. *BEWARE* some macros are endianness sensitive!
//Little endian (x86) is presumed unless noted otherwise.

//Address H/L
#define Z80_ADDRH (*((uint8_t*)(&z80_address)))
#define Z80_ADDRL (*((uint8_t*)((&z80_address) + 1)))

//I (endianness insensitive)
#define Z80_I  (z80.rI)

//R (endianness insensitive)
#define Z80_R  (z80.rR)

//AF
#define Z80_A  (z80.rAF[0])
#define Z80_Ap (z80.rAF[2])
#define Z80_F  (z80.rAF[1])
#define Z80_Fp (z80.rAF[3])
#define Z80_AF  (*((uint16_t*) z80.rAF))
#define Z80_AFp (*((uint16_t*) z80.rAF + 2))

//BC
#define Z80_B  z80.rBC[1]
#define Z80_C  z80.rBC[0]
#define Z80_Bp z80.rBC[3]
#define Z80_Cp z80.rBC[2]
#define Z80_BC  (*((uint16_t*) z80.rBC))
#define Z80_BCp (*((uint16_t*) z80.rBC + 2))

//DE
#define Z80_D  z80.rDE[1]
#define Z80_E  z80.rDE[0]
#define Z80_Dp z80.rDE[3]
#define Z80_Ep z80.rDE[2]
#define Z80_DE  (*((uint16_t*) z80.rDE))
#define Z80_DEp (*((uint16_t*) z80.rDE + 2))

//HL
#define Z80_H  z80.rHL[1]
#define Z80_L  z80.rHL[0]
#define Z80_Hp z80.rHL[3]
#define Z80_Lp z80.rHL[2]
#define Z80_HL  (*((uint16_t*) z80.rHL))
#define Z80_HLp (*((uint16_t*) z80.rHL + 2))

//WZ
#define Z80_W  z80.rWZ[1]
#define Z80_Z  z80.rWZ[0]
#define Z80_Wp z80.rWZ[3]
#define Z80_Zp z80.rWZ[2]
#define Z80_WZ  (*((uint16_t*) z80.rWZ))
#define Z80_WZp (*((uint16_t*) z80.rWZ + 2))

//IX (endianness insensitive)
#define Z80_IX z80.rIX
#define Z80_IY z80.rIY
#define Z80_SP z80.rSP
#define Z80_PC z80.rPC

// --- Flag update macros
#define Z80_SETFLAG_SIGN(X) (((X) & (1 << 7)) ? (1 << 7) : 0) /**<-- [S] Set sign flag (bit 7)*/
#define Z80_SETFLAG_ZERO(X) (((X) == 0) ? (1 << 6) : 0) /**<-- [Z] Set Zero flag (bit 6)*/
#define Z80_SETFLAG_HC(O,N) (((O & (1 << 3)) == 0) && ((N) & (1 << 3)) ? (1 << 4) : 0) /**<-- [H] Set Half-carry flag (bit 4)*/
#define Z80_SETFLAG_PARITY(X) (z80_parity(X)) /**<-- [P] Set parity flag (bit 2)*/
#define Z80_SETFLAG_OVERFLOW(O,N) (((int8_t)O) > ((int8_t)N) ? 0 : (1 << 2)) /**<-- [V] Set overflow flag (bit 2)*/
#define Z80_SETFLAG_ADD(A) (A ? 0 : (1<<1)) /**<-- [N] Set Add/Subtract flag (bit 1)*/
#define Z80_SETFLAG_CARRY(O,N) (O > N ? 1 : 0) /**<-- [C] Set Carry flag, adition (bit 0)*/
#define Z80_SETFLAG_BORROW(O,N) (O > N ? 1 : 0) /**<-- [C] Set carry flag, subtraction, (bit 0)*/
///@bug Deacimal Adjust Accumulate is not implemented

// --- Flag masks
#define Z80_FLAG_SIGN   (1<<7)
#define Z80_FLAG_ZERO   (1<<6)
#define Z80_FLAG_UNK5   (1<<5)
#define Z80_FLAG_HC     (1<<4)
#define Z80_FLAG_UNK3   (1<<3)
#define Z80_FLAG_PARITY (1<<2)
#define Z80_FLAG_ADD    (1<<1)
#define Z80_FLAG_CARRY  (1)

// --- Inverted flag masks (for bit clearing)
#define Z80_CLRFLAG_SIGN   (Z80_FLAG_SIGN   ^ 0xFF)
#define Z80_CLRFLAG_ZERO   (Z80_FLAG_ZERO   ^ 0xFF)
#define Z80_CLRFLAG_UNK5   (Z80_FLAG_UNK5   ^ 0xFF)
#define Z80_CLRFLAG_HC     (Z80_FLAG_HC     ^ 0xFF)
#define Z80_CLRFLAG_UNK3   (Z80_FLAG_UNK3   ^ 0xFF)
#define Z80_CLRFLAG_PARITY (Z80_FLAG_PARITY ^ 0xFF)
#define Z80_CLRFLAG_ADD    (Z80_FLAG_ADD    ^ 0xFF)
#define Z80_CLRFLAG_CARRY  (Z80_FLAG_CARRY  ^ 0xFF)


#endif