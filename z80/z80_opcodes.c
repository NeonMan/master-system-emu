#include "z80_opcodes.h"
#include "z80_macros.h"
#include "z80_register_lut.h"
#include <assert.h>

extern struct z80_s z80; //<-- Access to z80 internals

///ADD HL, rp[p] ; Size: 1; Flags N,C
int z80_op_ADD_HL_rp(){
    Z80_OPCODE_SUBDIV;
    const uint16_t old_hl = Z80_HL;
    Z80_HL = Z80_HL + *z80_rp[p[0]];
    //Clear N/Carry flag (bits 1,0)
    Z80_F = (Z80_F & (Z80_CLRFLAG_CARRY & Z80_CLRFLAG_ADD))
        //Set carry flag (bit 0)
        | Z80_SETFLAG_CARRY(old_hl, Z80_HL);
    return Z80_STAGE_RESET;
}

///ADD n; Size 2; Flags:ALL
int z80_op_ADD_n(){
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

///AND n; Size 2; Flags:ALL
int z80_op_AND_n(){
    const uint8_t orig_a = Z80_A;
    Z80_A = Z80_A & z80.opcode[1];
    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_SIGN(Z80_A);
    Z80_F |= Z80_SETFLAG_ZERO(Z80_A);
    Z80_F |= Z80_SETFLAG_OVERFLOW(orig_a, Z80_A);
    Z80_F |= Z80_FLAG_HC;
    return Z80_STAGE_RESET;
}

///CCF; Size: 1; Flags: C
int z80_op_CCF(){
    Z80_F = (Z80_F ^ Z80_FLAG_CARRY) & Z80_CLRFLAG_ADD;
    return Z80_STAGE_RESET;
}

///CP n; Size: 2; Flags: All
int z80_op_CP_n(){
    Z80_F = 0;
    Z80_F |= Z80_FLAG_ADD; //Flag is set, always
    Z80_F |= Z80_SETFLAG_SIGN(Z80_A - z80.opcode[1]);
    Z80_F |= Z80_SETFLAG_ZERO(Z80_A - z80.opcode[1]);
    Z80_F |= Z80_SETFLAG_HC(Z80_A, Z80_A - z80.opcode[1]);
    Z80_F |= Z80_SETFLAG_OVERFLOW(Z80_A, Z80_A - z80.opcode[1]);
    Z80_F |= Z80_SETFLAG_BORROW(Z80_A, Z80_A - z80.opcode[1]);
    return Z80_STAGE_RESET;
}

///CPL; Size: 1; Flags: H,N
int z80_op_CPL(){
    Z80_A = ~Z80_A;
    Z80_F = Z80_F | Z80_FLAG_HC | Z80_FLAG_ADD;
    return Z80_STAGE_RESET;
}

///DEC(r[y]); Size:1; Flags: S,Z,H,P,N
///@bug will crash on (HL)
int z80_op_DEC_r(){
    Z80_OPCODE_SUBDIV;
    const uint8_t old_r = *z80_r[y[0]];
    --(*(z80_r[y[0]]));
    Z80_F = (Z80_F & (
        Z80_CLRFLAG_SIGN & Z80_CLRFLAG_ZERO & Z80_CLRFLAG_HC
        & Z80_CLRFLAG_PARITY & Z80_CLRFLAG_ADD)
        )  //Clear S,Z,H,P,N (7,6,4,2,1) ***V0-
        | Z80_SETFLAG_SIGN(*z80_r[y[0]])
        | Z80_SETFLAG_ZERO(*z80_r[y[0]])
        | Z80_SETFLAG_HC(old_r, *z80_r[y[0]])
        | Z80_SETFLAG_OVERFLOW(*z80_r[y[0]], old_r);
    return Z80_STAGE_RESET;
}

///DEC(rp[p]); Size: 1; Flags: None
int z80_op_DEC_rp(){
    Z80_OPCODE_SUBDIV;
    --(*(z80_rp[p[0]]));
    return Z80_STAGE_RESET;
}

///DI; Size: 1; Flags: None
int z80_op_DI(){
    z80.iff[0] = 0;
    z80.iff[1] = 0;
    return Z80_STAGE_RESET;
}

///EI; Size: 1; Flags: None
int z80_op_EI(){
    z80.iff[0] = 1;
    z80.iff[1] = 1;
    return Z80_STAGE_RESET;
}

///DJNZ e; Size: 2; Flags: None
int z80_op_DJNZ_e(){
    --Z80_B;
    if (Z80_B)
        return Z80_STAGE_RESET;
    else{
        Z80_PC += ((int8_t)z80.opcode[1]);
        return Z80_STAGE_RESET;
    }
}

///EX AF, AFp; Size: 1; Flags: None
int z80_op_EX(){
    const uint16_t tmp_af = Z80_AF;
    Z80_AF = Z80_AFp;
    Z80_AFp = tmp_af;
    return Z80_STAGE_RESET;
}

///EX(DE,HL); Size: 1; Flags: None
int z80_op_EX_DE_HL(){
    const uint16_t old_de = Z80_DE;
    Z80_DE = Z80_HL;
    Z80_HL = old_de;
    return Z80_STAGE_RESET;
}

///EXX; Size: 1; Flags: None
int z80_op_EXX(){
    const uint16_t old_bc = Z80_BC;
    const uint16_t old_de = Z80_DE;
    const uint16_t old_hl = Z80_HL;
    Z80_BC = Z80_BCp;
    Z80_DE = Z80_DEp;
    Z80_HL = Z80_HLp;
    Z80_BCp = old_bc;
    Z80_DEp = old_de;
    Z80_HLp = old_hl;
    return Z80_STAGE_RESET;
}

///IM(im[y]); Size: 2; Flags: None
int z80_op_IM(){
    Z80_OPCODE_SUBDIV;
    z80.iff[0] = z80_im[y[1]][0];
    z80.iff[1] = z80_im[y[1]][1];
    return Z80_STAGE_RESET;
}

///IN A, (n); Size: 2; Flags: None
int z80_op_IN_A_np(){
    if (z80.read_index == 0){
        z80.read_address = z80.opcode[1] | (((uint16_t)Z80_A) << 8);
        z80.read_is_io = 1;
        return Z80_STAGE_M2;
    }
    else{
        Z80_A = z80.read_buffer[0];
        return Z80_STAGE_RESET;
    }
}

///INC (HL)
int z80_op_INC_HLp(){
    //Memory read
    if (z80.read_index == 0){
        z80.read_address = Z80_HL;
        return Z80_STAGE_M2;
    }
    else if (z80.write_index == 0){
        const uint8_t old_r = z80.read_buffer[0];
        z80.write_address = Z80_HL;
        z80.write_buffer[0] = old_r + 1;
        Z80_F = (Z80_F & (
            Z80_CLRFLAG_SIGN & Z80_CLRFLAG_ZERO & Z80_CLRFLAG_HC
            & Z80_CLRFLAG_PARITY & Z80_CLRFLAG_ADD)
            )  //Clear S,Z,H,P,N (7,6,4,2,1) ***V0-
            | Z80_SETFLAG_SIGN(old_r + 1)
            | Z80_SETFLAG_ZERO(old_r + 1)
            | Z80_SETFLAG_HC(old_r, old_r + 1)
            | Z80_SETFLAG_OVERFLOW(old_r, old_r + 1);
        return Z80_STAGE_M3;
    }
    else{
        return Z80_STAGE_RESET;
    }
}

///INC r
int z80_op_INC_r(){
    Z80_OPCODE_SUBDIV;
    uint8_t old_r = *z80_r[y[0]];
    ++(*(z80_r[y[0]]));
    Z80_F = (Z80_F & (
        Z80_CLRFLAG_SIGN & Z80_CLRFLAG_ZERO & Z80_CLRFLAG_HC
        & Z80_CLRFLAG_PARITY & Z80_CLRFLAG_ADD)
        )  //Clear S,Z,H,P,N (7,6,4,2,1) ***V0-
        | Z80_SETFLAG_SIGN(*z80_r[y[0]])
        | Z80_SETFLAG_ZERO(*z80_r[y[0]])
        | Z80_SETFLAG_HC(old_r, *z80_r[y[0]])
        | Z80_SETFLAG_OVERFLOW(old_r, *z80_r[y[0]]);
    return Z80_STAGE_RESET;
}

///INC(rp[p]); Size: 1; Flags: None
int z80_op_INC_rp(){
    Z80_OPCODE_SUBDIV;
    ++(*(z80_rp[p[0]]));
    return Z80_STAGE_RESET;
}

///JR cc, e; Size: 2; Flags: None
int z80_op_JR_cc(){
    //Test required flag
    Z80_OPCODE_SUBDIV;
    if ((Z80_F & z80_cc[y[0] - 4]) == (z80_cc_stat[y[0] - 4])){
        const int8_t pc_shift = *((int8_t*)&z80.opcode[1]); ///<-- @bug Endianness
        const int32_t next_pc = Z80_PC + pc_shift; //Signed relative jump
        Z80_PC = (next_pc & 0xFFFF);
    }
    return Z80_STAGE_RESET;
}

///JR, e; Size: 2; Flags: None
int z80_op_JR_e(){
    const int8_t pc_shift = *((int8_t*)&z80.opcode[1]); ///<-- @bug Endianness
    const int32_t next_pc = Z80_PC + pc_shift; //Signed relative jump
    Z80_PC = (next_pc & 0xFFFF);
    return Z80_STAGE_RESET;
}

///LD A,(DE); Size: 1; Flags: None
int z80_op_LD_A_DEp(){
    if (z80.read_index == 0){
        z80.read_address = Z80_DE;
        return Z80_STAGE_M2;
    }
    else{
        Z80_A = z80.read_buffer[0];
        return Z80_STAGE_RESET;
    }
}

///LD (BC), A; Size:1; Flags: None
int z80_op_LD_BCp_A(){
    //If the write has not been performed, request it
    if (z80.write_index == 0){
        z80.write_address = Z80_BC;
        z80.write_buffer[0] = Z80_A;
        return Z80_STAGE_M3;
    }
    //Otherwise, reset pipeline
    else{
        return Z80_STAGE_RESET;
    }
}

///LD (DE), A; Size: 1; Flags: None
int z80_op_LD_DEp_A(){
    if (z80.write_index == 0){
        z80.write_address = Z80_DE;
        z80.write_buffer[0] = Z80_A;
        return Z80_STAGE_M3;
    }
    else{
        return Z80_STAGE_RESET;
    }
}

///LD r[y], (HL); Size:1; Flags: None
int z80_op_LD_r_HLp(){
    //Source is always (HL, Target is never (HL)
    Z80_OPCODE_SUBDIV;
    if (z80.read_index == 0){
        z80.read_address = Z80_HL;
        return Z80_STAGE_M2;
    }
    else{
        *(z80_r[y[0]]) = z80.read_buffer[0];
        return Z80_STAGE_RESET;
    }
}

///LD r, n; Size: 2; Flags: None
int z80_op_LD_r_n(){
    //If target is HL, perform write
    Z80_OPCODE_SUBDIV;
    if (z80_r[y[0]] == 0){
        if (z80.write_index == 0){
            z80.write_address = Z80_HL;
            z80.write_buffer[0] = z80.opcode[1];
            return Z80_STAGE_M3;
        }
        else{
            return Z80_STAGE_RESET;
        }
    }
    //Otherwise
    else{
        *(z80_r[y[0]]) = z80.opcode[1];
        return Z80_STAGE_RESET;
    }
}

///LD r[y],r[z]; Size: 1; Flags: None
int z80_op_LD_r_r(){
    //Source can never be (HL). That implies z[0]==6
    //Target can be (HL)
    Z80_OPCODE_SUBDIV;
    if (z80_r[y[0]]){ //If target != (HL)
        *(z80_r[y[0]]) = *(z80_r[z[0]]);
        return Z80_STAGE_RESET;
    }
    else{ //Target is (HL)
        if (z80.write_index == 0){
            z80.write_address = Z80_HL;
            assert(z80_r[z[0]]);
            z80.write_buffer[0] = *(z80_r[z[0]]);
            return Z80_STAGE_M3;
        }
        else{
            return Z80_STAGE_RESET;
        }
    }
}

///LDIR; Size: 2; Flags: H,P,N (cleared)
int z80_op_LDIR(){
    //(DE) <-- (HL); ++DE; ++HL; --BC; BC? repeat : end;
    //Perform a read
    if (z80.read_index == 0){
        z80.read_address = Z80_HL;
        return Z80_STAGE_M2;
    }
    //Perform a write
    else if (z80.write_index == 0){
        z80.write_address = Z80_DE;
        z80.write_buffer[0] = z80.read_buffer[0];
        return Z80_STAGE_M3;
    }
    //Update registers and end
    else{
        ++Z80_HL;
        ++Z80_DE;
        --Z80_BC;
        Z80_F = Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_PARITY & Z80_CLRFLAG_ADD);
        if (Z80_BC){
            Z80_PC -= 2; //Repeat this intruction
            return Z80_STAGE_RESET;
        }
        else{
            return Z80_STAGE_RESET;
        }
    }
}

///NOP; Size: 1; Flags: None
int z80_op_NOP(){
    return Z80_STAGE_RESET;
}

///OR n; Size: 2; Flags:ALL
int z80_op_OR_n(){
    const uint8_t orig_a = Z80_A;
    Z80_A = Z80_A | z80.opcode[1];
    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_SIGN(Z80_A);
    Z80_F |= Z80_SETFLAG_ZERO(Z80_A);
    Z80_F |= Z80_SETFLAG_OVERFLOW(orig_a, Z80_A);
    return Z80_STAGE_RESET;
}

///OTIR; Size: 2; Flags: Z,N
int z80_op_OTIR(){
    //(C)<-(HL), B<-B – 1, HL<-HL + 1; B? repeat : end
    //Perform read
    if (z80.read_index == 0){
        z80.read_address = Z80_HL;
        return Z80_STAGE_M2;
    }
    //Perform write
    else if (z80.write_index == 0){
        z80.write_address = Z80_C | (((uint16_t)Z80_A) << 8);
        z80.write_is_io = 1;
        z80.write_buffer[0] = z80.read_buffer[0];
        return Z80_STAGE_M3;
    }
    //Update state and flags
    else{
        ++Z80_HL;
        --Z80_B;
        Z80_F = Z80_F & (Z80_CLRFLAG_ZERO & Z80_CLRFLAG_ADD); //Z,N
        if (Z80_B){ //Repeat instruction
            Z80_PC = Z80_PC - 2;
            return Z80_STAGE_RESET;
        }
        else{
            return Z80_STAGE_RESET;
        }
    }
}

///OUT (n), A; Size: 2; Flags: None
int z80_op_OUT_np_A(){
    //if no byte has been written, prepare a write
    if (z80.write_index == 0){
        const uint16_t port_addr = z80.opcode[1] + (((uint16_t)Z80_A) << 8);
        z80.write_address = port_addr;
        z80.write_buffer[0] = Z80_A;
        z80.write_is_io = 1;
        return Z80_STAGE_M3;
    }
    //Otherwise, reset.
    else
        return Z80_STAGE_RESET;
}

///POP rp2[p]; Size: 1; Flags: None
int z80_op_POP_rp2(){
    Z80_OPCODE_SUBDIV;
    //Read stack
    if (z80.read_index == 0){
        z80.read_address = Z80_SP;
        return Z80_STAGE_M2;
    }
    else if (z80.read_index == 1){
        ++z80.read_address;
        return Z80_STAGE_M2;
    }
    //Update state
    else{
        Z80_SP += 2;
        *(z80_rp2[p[0]]) = *((uint16_t*)z80.read_buffer); ///<-- @bug Endianness!
        return Z80_STAGE_RESET;
    }
}

///PUSH rp2[p]; Size: 1; Flags: None
int z80_op_PUSH_rp2(){
    Z80_OPCODE_SUBDIV;
    //Prepare a write if needed
    if (z80.write_index == 0){
        z80.write_address = Z80_SP - 2;
        *((uint16_t*)z80.write_buffer) = *(z80_rp2[p[0]]); ///<-- @bug Endianness
        return Z80_STAGE_M3;
    }
    else if (z80.write_index == 1){
        ++z80.write_address;
        return Z80_STAGE_M3;
    }
    else{
        Z80_SP -= 2;
        return Z80_STAGE_RESET;
    }
}

///RET; Size: 1; Flags: None
int z80_op_RET(){
    //Read stack
    if (z80.read_index == 0){
        z80.read_address = Z80_SP;
        return Z80_STAGE_M2;
    }
    else if (z80.read_index == 1){
        ++z80.read_address;
        return Z80_STAGE_M2;
    }
    else{
        Z80_SP += 2;
        Z80_PC = *((uint16_t*)z80.read_buffer); ///<-- @bug Endianness!
        return Z80_STAGE_RESET;
    }
}

///RET cc[y]; Size: 1; Flags: None
int z80_op_RET_cc(){
    Z80_OPCODE_SUBDIV;
    if ((Z80_F & z80_cc[y[0]]) == z80_cc_stat[y[0]]){
        //POP stack, update PC
        if (z80.read_index == 0){
            z80.read_address = Z80_SP;
            return Z80_STAGE_M2;
        }
        else if (z80.read_index == 1){
            ++(z80.read_address);
            return Z80_STAGE_M2;
        }
        else{
            Z80_PC = *((uint16_t*)(z80.read_buffer));
            Z80_SP += 2;
            return Z80_STAGE_RESET;
        }
    }
    else{
        return Z80_STAGE_RESET;
    }
}

int z80_op_RLA(){
    const uint8_t next_carry = Z80_A & (1 << 7);
    Z80_A = (Z80_A << 1) | (Z80_F & Z80_FLAG_CARRY ? 1 : 0);
    Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_ADD & Z80_CLRFLAG_CARRY))
        | (next_carry ? Z80_FLAG_CARRY : 0);
    return Z80_STAGE_RESET;
}

///RLCA; Size: 1; Flags: H,N,C
int z80_op_RLCA(){
    Z80_A = (Z80_A << 1) | (Z80_A & (1 << 7) ? 1 : 0);
    Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_ADD & Z80_CLRFLAG_CARRY))
        | ((Z80_F & (1)) ? Z80_FLAG_CARRY : 0);
    return Z80_STAGE_RESET;
}

///RRA; Size: 1; Flags: H,N,C
int z80_op_RRA(){
    const uint8_t next_carry = Z80_A & (1);
    Z80_A = (Z80_A >> 1) | (Z80_F & Z80_FLAG_CARRY ? (1 << 7) : 0);
    Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_ADD & Z80_CLRFLAG_CARRY))
        | (next_carry ? Z80_FLAG_CARRY : 0);
    return Z80_STAGE_RESET;
}

///RRCA; Size: 1; Flags: H,N,C
int z80_op_RRCA(){
    Z80_A = (Z80_A >> 1) | (Z80_A & 1 ? (1 << 7) : 0);
    Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_ADD & Z80_CLRFLAG_CARRY))
        | ((Z80_F & (1 << 7)) ? Z80_FLAG_CARRY : 0);
    return Z80_STAGE_RESET;
}

///SBC HL,rp[p]; Size: 2; Flags: ?
int z80_op_SBC_HL_rp(){
    Z80_OPCODE_SUBDIV;
    const uint16_t old_hl = Z80_HL;
    Z80_HL = (Z80_F & Z80_FLAG_CARRY) ? Z80_HL - *(z80_rp[p[1]]) : Z80_HL - *(z80_rp[p[1]]) - 1;
    Z80_F = Z80_FLAG_ADD
        | Z80_SETFLAG_BORROW(old_hl, Z80_HL)
        | Z80_SETFLAG_SIGN(Z80_HL)
        | Z80_SETFLAG_ZERO(Z80_HL)
        | Z80_SETFLAG_HC((old_hl >> 8), (Z80_HL >> 8))
        | Z80_SETFLAG_OVERFLOW(old_hl, Z80_HL);
    return Z80_STAGE_RESET;
}

///SCF; Size: 1; Flags: C
int z80_op_SCF(){
    Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_ADD)) | Z80_FLAG_CARRY;
    return Z80_STAGE_RESET;
}
