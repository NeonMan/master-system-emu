#include "z80_opcodes.h"

extern struct z80_s z80; //<-- Access to z80 internals

///ADD n; Size 2; Flags:ALL
int z80_opcode_ADD(){
    const uint8_t orig_a = Z80_A;
    Z80_A = Z80_A + z80.opcode[1];
    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_SIGN(Z80_A);
    Z80_F |= Z80_SETFLAG_ZERO(Z80_A);
    Z80_F |= Z80_SETFLAG_HC(orig_a, Z80_A);
    Z80_F |= Z80_SETFLAG_OVERFLOW(orig_a, Z80_A);
    Z80_F |= Z80_SETFLAG_CARRY(orig_a, Z80_A);
    return Z80_STAGE_RESET;
}
