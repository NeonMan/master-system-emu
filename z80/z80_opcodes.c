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

#include "z80_opcodes.h"
#include "z80_macros.h"
#include "z80_register_lut.h"
#include <assert.h>

extern struct z80_s z80; //<-- Access to z80 internals

///ADC n; Size: 2; Flags: ???
int ADC_n(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///ADC (HL); Size: 1; Flags: ???
int ADC_HLp(){
    assert(z80.opcode_index == 1);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///ADC HL, rp; Size: 2; Flags: ???
int ADC_HL_rp(){
    assert(z80.opcode_index == 2); 
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///ADC r; Size: 1; Flags: ???
int ADC_r(){
    assert(z80.opcode_index == 1); 
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///ADD HL, rp[p] ; Size: 1; Flags N,C
int ADD_HL_rp(){
    assert(z80.opcode_index == 1); 
    Z80_OPCODE_SUBDIV;
    const uint16_t old_hl = Z80_HL;
    Z80_HL = Z80_HL + *z80_rp[p[0]];
    //Clear N/Carry flag (bits 1,0)
    Z80_F = (Z80_F & (Z80_CLRFLAG_CARRY & Z80_CLRFLAG_SUBTRACT))
        //Set carry flag (bit 0)
        | Z80_SETFLAG_CARRY(old_hl, Z80_HL);
    return Z80_STAGE_RESET;
}

///ADD (HL); Size: 1; Flags: ???
int ADD_HLp(){
    assert(z80.opcode_index == 1);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///ADD n; Size 2; Flags:ALL
int ADD_n(){
    assert(z80.opcode_index == 2);
    const uint8_t orig_a = Z80_A;
    Z80_A = Z80_A + z80.opcode[1];
    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_SIGN(Z80_A);
    Z80_F |= Z80_SETFLAG_ZERO(Z80_A);
    Z80_F |= Z80_SETFLAG_HALF_CARRY(orig_a, z80.opcode[1]);
    Z80_F |= Z80_SETFLAG_OVERFLOW(orig_a, Z80_A);
    Z80_F |= Z80_SETFLAG_CARRY(orig_a, Z80_A);
    return Z80_STAGE_RESET;
}

///ADD r; Size: 1; Flags:ALL
int ADD_r(){
    assert(z80.opcode_index == 1);
    assert(0);
    return Z80_STAGE_RESET;
}

///AND n; Size 2; Flags:ALL
int AND_n(){
    assert(z80.opcode_index == 2);
    const uint8_t orig_a = Z80_A;
    Z80_A = Z80_A & z80.opcode[1];
    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_SIGN(Z80_A);
    Z80_F |= Z80_SETFLAG_ZERO(Z80_A);
    Z80_F |= Z80_SETFLAG_OVERFLOW(orig_a, Z80_A);
    Z80_F |= Z80_FLAG_HC;
    return Z80_STAGE_RESET;
}

///AND r[z]; Size: 1; Flags: All
int AND_r(){
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    assert(z[0] != Z80_R_INDEX_HL);

    const uint8_t orig_a = Z80_A;
    Z80_A = Z80_A & (*(z80_r[z[0]]));

    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_SIGN(Z80_A);
    Z80_F |= Z80_SETFLAG_ZERO(Z80_A);
    Z80_F |= Z80_FLAG_HC;
    Z80_F |= Z80_SETFLAG_OVERFLOW(orig_a, Z80_A);
    return Z80_STAGE_RESET;
}

///AND (HL); Size: 1; Flags: ???
int AND_HLp(){
    assert(z80.opcode_index == 1);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///BIT y, (HL); Size: 2; Flags: ???
int BIT_b_HLp(){
    assert(z80.opcode_index == 2);
    assert(0);
    return Z80_STAGE_RESET;
}

///BIT y,r[z]; Size: 2; Flags: _S,Z,H,_P,N
int BIT_b_r(){
    assert(z80.opcode_index == 2);
    Z80_OPCODE_SUBDIV;
    Z80_F = (Z80_F & (Z80_CLRFLAG_ZERO & Z80_CLRFLAG_SUBTRACT)); //Clear Z,N
    Z80_F = Z80_F | (((1 << y[1]) & (*z80_r[z[1]])) ? 0 : Z80_CLRFLAG_ZERO);
    Z80_F = Z80_F | Z80_FLAG_HC;
    return Z80_STAGE_RESET;
}

///CALL cc[y] nn; Size: 3; Flags: None
int CALL_cc_nn(){
    assert(z80.opcode_index == 3);
    Z80_OPCODE_SUBDIV;
    //Check condition
    if ((Z80_F & z80_cc[y[0]]) == (z80_cc_stat[y[0]])){
        //Push PC onto the stack
        Z80_16BIT_WRITE(Z80_SP - 2, 0, Z80_PC & 0xFF, ((Z80_PC >> 8) & 0xFF));

        //Update SP
        Z80_SP -= 2;
        //Update PC
        const uint16_t new_pc = *((uint16_t*)(z80.opcode + 1)); ///<-- @bug Endiannes!
        Z80_PC = new_pc;
        return Z80_STAGE_RESET;
    }
    else{
        return Z80_STAGE_RESET;
    }
}

///CALL nn; Size: 3; Flags: None
int CALL_nn(){
    assert(z80.opcode_index == 3);
    //Push PC into stack
    Z80_16BIT_WRITE(Z80_SP - 2, 0, Z80_PC & 0xFF, (Z80_PC >> 8) & 0xFF);
    //Update SP
    Z80_SP -= 2;
    //Update PC
    const uint16_t new_pc = *((uint16_t*)(z80.opcode + 1)); ///<-- @bug Endiannes!
    Z80_PC = new_pc;
    return Z80_STAGE_RESET;
}

///CCF; Size: 1; Flags: C
int CCF(){
    assert(z80.opcode_index == 1);
    Z80_F = (Z80_F ^ Z80_FLAG_CARRY) & Z80_CLRFLAG_SUBTRACT;
    return Z80_STAGE_RESET;
}

///CPD; Size: 2; Flags: ???
int CPD(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///CPI; Size: 2; Flags: ???
int CPI(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///CPDR; Size: 2; Flags: ???
int CPDR(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///CPIR; Size: 2; Flags: ???
int CPIR(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///CP n; Size: 2; Flags: All
int CP_n(){
    assert(z80.opcode_index == 2);
    Z80_F = 0;
    Z80_F |= Z80_FLAG_SUBTRACT; //Flag is set, always
    Z80_F |= Z80_SETFLAG_SIGN(Z80_A - z80.opcode[1]);
    Z80_F |= Z80_SETFLAG_ZERO(Z80_A - z80.opcode[1]);
    Z80_F |= Z80_SETFLAG_HALF_BORROW(Z80_A, z80.opcode[1]);
    Z80_F |= Z80_SETFLAG_OVERFLOW(Z80_A, Z80_A - z80.opcode[1]);
    Z80_F |= Z80_SETFLAG_BORROW(Z80_A, Z80_A - z80.opcode[1]);
    return Z80_STAGE_RESET;
}

///CP (HL); Size: 1; Flags: ???
int CP_HLp(){
    assert(z80.opcode_index == 1);
    Z80_8BIT_READ(Z80_HL, 0);
    const uint8_t result = Z80_A - z80.read_buffer[0];

    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_SIGN(result);
    Z80_F |= Z80_SETFLAG_ZERO(result);
    Z80_F |= Z80_SETFLAG_HALF_BORROW(Z80_A, result);
    Z80_F |= Z80_SETFLAG_OVERFLOW(Z80_A, result);
    Z80_F |= Z80_FLAG_SUBTRACT;
    Z80_F |= Z80_SETFLAG_BORROW(Z80_A, result);
    return Z80_STAGE_RESET;
}

///CP r[z]; Size: 1; Flags: All
int CP_r(){
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    assert(z[0] != Z80_R_INDEX_HL);

    uint8_t new_a;
    new_a = Z80_A - *(z80_r[z[0]]);

    Z80_F = Z80_SETFLAG_SIGN(Z80_A)
        | Z80_SETFLAG_ZERO(Z80_A)
        | Z80_SETFLAG_HALF_BORROW(Z80_A, *(z80_r[z[0]]))
        | Z80_SETFLAG_OVERFLOW(Z80_A, new_a)
        | Z80_FLAG_SUBTRACT
        | Z80_SETFLAG_BORROW(Z80_A, new_a);
    return Z80_STAGE_RESET;
}

///CPL; Size: 1; Flags: H,N
int CPL(){
    assert(z80.opcode_index == 1);
    Z80_A = ~Z80_A;
    Z80_F = Z80_F | Z80_FLAG_HC | Z80_FLAG_SUBTRACT;
    return Z80_STAGE_RESET;
}

///DAA; Size: 1; Flags: ???
int DAA(){
    assert(z80.opcode_index == 1);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///DEC(r[y]); Size:1; Flags: S,Z,H,P,N
int DEC_r(){
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    const uint8_t old_r = *z80_r[y[0]];
    --(*(z80_r[y[0]]));
    Z80_F = (Z80_F & (
        Z80_CLRFLAG_SIGN & Z80_CLRFLAG_ZERO & Z80_CLRFLAG_HC
        & Z80_CLRFLAG_PARITY & Z80_CLRFLAG_SUBTRACT)
        )  //Clear S,Z,H,P,N (7,6,4,2,1) ***V0-
        | Z80_SETFLAG_SIGN(*z80_r[y[0]])
        | Z80_SETFLAG_ZERO(*z80_r[y[0]])
        | Z80_SETFLAG_HALF_BORROW(old_r, 1)
        | Z80_SETFLAG_OVERFLOW(*z80_r[y[0]], old_r);
    return Z80_STAGE_RESET;
}

///DEC (HL); Size: 1; Flags: ???
int DEC_HLp(){
    assert(z80.opcode_index == 1);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///DEC(rp[p]); Size: 1; Flags: None
int DEC_rp(){
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    --(*(z80_rp[p[0]]));
    return Z80_STAGE_RESET;
}

///DI; Size: 1; Flags: None
int DI(){
    assert(z80.opcode_index == 1);
    z80.iff[0] = 0;
    z80.iff[1] = 0;
    return Z80_STAGE_RESET;
}

///EI; Size: 1; Flags: None
int EI(){
    assert(z80.opcode_index == 1);
    z80.iff[0] = 1;
    z80.iff[1] = 1;
    return Z80_STAGE_RESET;
}

///DJNZ e; Size: 2; Flags: None
int DJNZ_d(){
    assert(z80.opcode_index == 2);
    --Z80_B;
    if (Z80_B)
        return Z80_STAGE_RESET;
    else{
        Z80_PC += ((int8_t)z80.opcode[1]);
        return Z80_STAGE_RESET;
    }
}

///EX AF, AFp; Size: 1; Flags: None
int EX(){
    assert(z80.opcode_index == 1);
    const uint16_t tmp_af = Z80_AF;
    Z80_AF = Z80_AFp;
    Z80_AFp = tmp_af;
    return Z80_STAGE_RESET;
}

///EX DE, HL; Size: 1; Flags: None
int EX_DE_HL(){
    assert(z80.opcode_index == 1);
    const uint16_t old_de = Z80_DE;
    Z80_DE = Z80_HL;
    Z80_HL = old_de;
    return Z80_STAGE_RESET;
}

///EX (SP), HL; Size: 1; Flags: None
int EX_SPp_HL(){
    assert(z80.opcode_index == 1);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///EXX; Size: 1; Flags: None
int EXX(){
    assert(z80.opcode_index == 1);
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

///HALT; Size: 1; Flags: None
int HALT(){
    assert(z80.opcode_index == 1);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///IM(im[y]); Size: 2; Flags: None
int IM(){
    assert(z80.opcode_index == 2);
    Z80_OPCODE_SUBDIV;
    z80.iff[0] = z80_im[y[1]][0];
    z80.iff[1] = z80_im[y[1]][1];
    return Z80_STAGE_RESET;
}

///IN A, (n); Size: 2; Flags: None
int IN_A_np(){
    assert(z80.opcode_index == 2);

    Z80_8BIT_READ((z80.opcode[1] | (((uint16_t)Z80_A) << 8)), 1);
    Z80_A = z80.read_buffer[0];
    return Z80_STAGE_RESET;
}

///IN (C); Size: 2; Flags: ???
int IN_Cp(){
    assert(z80.opcode_index == 2);
    assert(0);
    return Z80_STAGE_RESET;
}

///IN r, (C); Size: 2; Flags: ???
int IN_r_Cp(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///INC (HL); Size: 1; Flags: All 
int INC_HLp(){
    assert(z80.opcode_index == 1);
    //Memory read
    Z80_8BIT_READ(Z80_HL, 0);
    //Mem write
    Z80_8BIT_WRITE(Z80_HL, 0, z80.read_buffer[0] + 1);
    
    const uint8_t old_r = z80.read_buffer[0];
    Z80_F = (Z80_F & (
        Z80_CLRFLAG_SIGN & Z80_CLRFLAG_ZERO & Z80_CLRFLAG_HC
        & Z80_CLRFLAG_PARITY & Z80_CLRFLAG_SUBTRACT)
        )  //Clear S,Z,H,P,N (7,6,4,2,1) ***V0-
        | Z80_SETFLAG_SIGN(old_r + 1)
        | Z80_SETFLAG_ZERO(old_r + 1)
        | Z80_SETFLAG_HALF_CARRY(old_r, 1)
        | Z80_SETFLAG_OVERFLOW(old_r, old_r + 1);
    return Z80_STAGE_RESET;
}

///INC r; Size: 1; Flags: All
int INC_r(){
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    
    uint8_t old_r = *z80_r[y[0]];
    ++(*(z80_r[y[0]]));
    Z80_F = (Z80_F & (
        Z80_CLRFLAG_SIGN & Z80_CLRFLAG_ZERO & Z80_CLRFLAG_HC
        & Z80_CLRFLAG_PARITY & Z80_CLRFLAG_SUBTRACT)
        )  //Clear S,Z,H,P,N (7,6,4,2,1) ***V0-
        | Z80_SETFLAG_SIGN(*z80_r[y[0]])
        | Z80_SETFLAG_ZERO(*z80_r[y[0]])
        | Z80_SETFLAG_HALF_CARRY(old_r, 1)
        | Z80_SETFLAG_OVERFLOW(old_r, *z80_r[y[0]]);
    return Z80_STAGE_RESET;
}

///INC(rp[p]); Size: 1; Flags: None
int INC_rp(){
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    ++(*(z80_rp[p[0]]));
    return Z80_STAGE_RESET;
}

///IND; Size: 2; Flags: ???
int IND(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///INDR; Size: 2; Flags: ???
int INDR(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///INI; Size: 2; Flags: ???
int INI(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///INIR; Size: 2; Flags: ???
int INIR(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///JP nn; Size: 3; Flags: None
int JP_nn(){
    assert(z80.opcode_index == 3);
    ///New PC stored in opcode's last bytes. @bug Endianness.
    const uint16_t new_pc = *((uint16_t*)(z80.opcode + 1));
    Z80_PC = new_pc;
    return Z80_STAGE_RESET;
}

///JP cc[y], nn; Size: 3; Flags: None
int JP_cc_nn(){
    assert(z80.opcode_index == 3);
    Z80_OPCODE_SUBDIV;
    //Test condition
    if ((Z80_F & (z80_cc[y[0]])) == (z80_cc_stat[y[0]])){
        const uint16_t new_pc = *((uint16_t*)(z80.opcode + 1));
        Z80_PC = new_pc; ///<-- @bug Endianness!
        return Z80_STAGE_RESET;
    }
    else{
        return Z80_STAGE_RESET;
    }
}

///JP HL; Size: 1; Flags: None
int JP_HLp(){
    assert(z80.opcode_index == 1);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///JR cc, e; Size: 2; Flags: None
int JR_cc_d(){
    assert(z80.opcode_index == 2);
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
int JR_d(){
    assert(z80.opcode_index == 2);
    const int8_t pc_shift = *((int8_t*)&z80.opcode[1]); ///<-- @bug Endianness
    const int32_t next_pc = Z80_PC + pc_shift; //Signed relative jump
    Z80_PC = (next_pc & 0xFFFF);
    return Z80_STAGE_RESET;
}

///LD A, (BC); Size: 1; Flags: None
int LD_A_BCp(){
    assert(z80.opcode_index == 1);
    Z80_8BIT_READ(Z80_BC, 0);
    Z80_A = z80.read_buffer[0];
    return Z80_STAGE_RESET;
}

///LD A,(DE); Size: 1; Flags: None
int LD_A_DEp(){
    assert(z80.opcode_index == 1);
    Z80_8BIT_READ(Z80_DE, 0);
    Z80_A = z80.read_buffer[0];
    return Z80_STAGE_RESET;
}

///LD A, I; Size: 2; Flags: ???
int LD_A_I(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///LD A, (nn); Size: 3; Flags: None
int LD_A_nnp(){
    assert(z80.opcode_index == 3);
    Z80_8BIT_READ(z80.opcode[1] | (((uint16_t)z80.opcode[2])<<8), 0);
    Z80_A = z80.read_buffer[0];
    return Z80_STAGE_RESET;
}

///LD A, R; Size: 2; Flags: ???
int LD_A_R(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///LD (BC), A; Size:1; Flags: None
int LD_BCp_A(){
    assert(z80.opcode_index == 1);
    Z80_8BIT_WRITE(Z80_BC, 0, Z80_A);
    //Otherwise, reset pipeline
    return Z80_STAGE_RESET;
}

///LD HL, (nn); Size: 3; Flags: None
int LD_HL_nnp(){
    assert(z80.opcode_index == 3);
    Z80_16BIT_READ(z80.opcode[1] | (((uint16_t)z80.opcode[2])<<8), 0);

    Z80_H = z80.read_buffer[1];
    Z80_L = z80.read_buffer[0];
    return Z80_STAGE_RESET;
}

///LD (HL), n; Size: 2; Flags: None
int LD_HLp_n(){
    assert(z80.opcode_index == 2);
    Z80_8BIT_WRITE(Z80_HL, 0, z80.opcode[1]);
    return Z80_STAGE_RESET;
}

///LD (HL), r; Size: 1; Flags: None
int LD_HLp_r(){
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    Z80_8BIT_WRITE(Z80_HL, 0, *(z80_r[z[0]]))
    return Z80_STAGE_RESET;
}

///LD I, A; Size: 2; Flags: None
int LD_I_A(){
    assert(z80.opcode_index == 2);
    Z80_I = Z80_A;
    return Z80_STAGE_RESET;
}

///LD (DE), A; Size: 1; Flags: None
int LD_DEp_A(){
    assert(z80.opcode_index == 1);
    Z80_8BIT_WRITE(Z80_DE, 0, Z80_A);
    return Z80_STAGE_RESET;
}

///LD (nn), rp; Size:4; Flags: None
int LD_nnp_rp(){
    Z80_OPCODE_SUBDIV;
    assert(z80.opcode_index = 4);

    const uint8_t b0 = (*(z80_rp[p[1]])) & 0x00FF;
    const uint8_t b1 = ((*(z80_rp[p[1]])) >> 8) & 0x00FF;
    Z80_16BIT_WRITE((z80.opcode[2] | (((uint16_t)z80.opcode[3]) << 8)), 0, b0, b1);
    return Z80_STAGE_RESET;
}

///LD R, A; Size: 2; Flags: None
int LD_R_A(){
    assert(z80.opcode_index == 2);
    Z80_R = Z80_A;
    return Z80_STAGE_RESET;
}

///LD r, (HL); Size: 1; Flags: None
int LD_r_HLp(){
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;

    Z80_8BIT_READ(Z80_HL, 0);
    *(z80_r[y[0]]) = z80.read_buffer[0];
    return Z80_STAGE_RESET;
}

///LD r, n; Size: 2; Flags: None
int LD_r_n(){
    assert(z80.opcode_index == 2);
    Z80_OPCODE_SUBDIV;
    *(z80_r[y[0]]) = z80.opcode[1];
    return Z80_STAGE_RESET;
}

///LD r[y],r[z]; Size: 1; Flags: None
int LD_r_r(){
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    assert(y[0] != 0x06); //
    assert(z[0] != 0x06); // (HL) is treated as a special case
    *(z80_r[y[0]]) = *(z80_r[z[0]]);
    return Z80_STAGE_RESET;
}

///LD rp[n], nn; Size: 3; Flags: None
int LD_rp_nn(){
    assert(z80.opcode_index == 3);
    //nn is stored in the opcode's bytes 1,2.
    Z80_OPCODE_SUBDIV;
    const uint16_t immediate = *((uint16_t*)(z80.opcode + 1)); ///<-- @bug Endianness!
    *z80_rp[p[0]] = immediate; ///<-- @bug Endianness!
    return Z80_STAGE_RESET;
}

///LD rp, (nn); Size: 4; Flags: None
int LD_rp_nnp(){
    Z80_OPCODE_SUBDIV;
    assert(z80.opcode_index == 4);
    Z80_16BIT_READ(z80.opcode[2] | (((uint16_t)z80.opcode[3]) << 8), 0);
    *(z80_rp[p[1]]) = z80.read_buffer[0] | (((uint16_t)z80.read_buffer[1])<<8);
    return Z80_STAGE_RESET;
}

///LD SP, HL; Size: 1; Flags: None
int LD_SP_HL(){
    assert(z80.opcode_index == 1);
    Z80_SP = Z80_HL;
    return Z80_STAGE_RESET;
}

///LD (nn), A; Size: 3; Flags: None
int LD_nnp_A(){
    assert(z80.opcode_index == 3);
    Z80_8BIT_WRITE((z80.opcode[1] | (((uint16_t)z80.opcode[2])<<8)), 0, Z80_A);
    return Z80_STAGE_RESET;
}

///LD (nn), HL; size: 3; Flags: None
int LD_nnp_HL(){
    assert(z80.opcode_index == 3);
    Z80_16BIT_WRITE(z80.opcode[1] | (((uint16_t)z80.opcode[2]) << 8), 0, Z80_L, Z80_H);
    return Z80_STAGE_RESET;
}

///LDD; Size: 2; Flags: ???
int LDD(){
    assert(z80.opcode_index == 2);
    //Copy a byte.
    Z80_8BIT_READ(Z80_HL, 0);
    Z80_8BIT_WRITE(Z80_DE, 0, z80.read_buffer[0]);
    //Update registers
    --(Z80_DE);
    --(Z80_HL);
    --(Z80_BC);
    //Update flags
    Z80_F &= Z80_CLRFLAG_HC & Z80_CLRFLAG_PARITY & Z80_CLRFLAG_SUBTRACT;
    Z80_F |= ((Z80_BC == 0) ? 0 : Z80_FLAG_PARITY);
    //Shall we continue copying?
    if (Z80_BC){
        //Execute this opcode again
        Z80_PC -= 2;
    }
    return Z80_STAGE_RESET;
}

///LDI; Size: 2; Flags: H,V,N
int LDI(){
    assert(z80.opcode_index == 2);
    //Copy a byte.
    Z80_8BIT_READ(Z80_HL, 0);
    Z80_8BIT_WRITE(Z80_DE, 0, z80.read_buffer[0]);
    //Update registers
    ++(Z80_DE);
    ++(Z80_HL);
    --(Z80_BC);
    //Update flags
    Z80_F &= Z80_CLRFLAG_HC & Z80_CLRFLAG_PARITY & Z80_CLRFLAG_SUBTRACT;
    Z80_F |= ((Z80_BC == 0) ? 0: Z80_FLAG_PARITY);
    //Shall we continue copying?
    if (Z80_BC){
        //Execute this opcode again
        Z80_PC -= 2;
    }
    return Z80_STAGE_RESET;
}

///LDDR; Size: 2; Flags: ???
int LDDR(){
    assert(z80.opcode_index == 2);
    //Copy a byte.
    Z80_8BIT_READ(Z80_HL, 0);
    Z80_8BIT_WRITE(Z80_DE, 0, z80.read_buffer[0]);
    //Update registers
    --(Z80_DE);
    --(Z80_HL);
    --(Z80_BC);
    //Update flags
    Z80_F &= Z80_CLRFLAG_HC & Z80_CLRFLAG_PARITY & Z80_CLRFLAG_SUBTRACT;
    //Shall we continue copying?
    if (Z80_BC){
        //Execute this opcode again
        Z80_PC -= 2;
    }
    return Z80_STAGE_REFRESH;
}

///LDIR; Size: 2; Flags: H,P,N (cleared)
int LDIR(){
    assert(z80.opcode_index == 2);
    //(DE) <-- (HL); ++DE; ++HL; --BC; BC? repeat : end;
    //Perform a read
    Z80_8BIT_READ(Z80_HL, 0);
    //Perform a write
    Z80_8BIT_WRITE(Z80_DE, 0, z80.read_buffer[0]);
    //Update registers and end
    ++Z80_HL;
    ++Z80_DE;
    --Z80_BC;
    Z80_F = Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_PARITY & Z80_CLRFLAG_SUBTRACT);
    if (Z80_BC){
        Z80_PC -= 2; //Repeat this intruction
    }
    return Z80_STAGE_REFRESH;
}

///NEG; Size: 2; Flags: ???
int NEG(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///NOP; Size: 1; Flags: None
int NOP(){
    assert(z80.opcode_index == 1);
    return Z80_STAGE_RESET;
}

///OR (HL); Size: 1; Flags: ???
int OR_HLp(){
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    Z80_8BIT_READ(Z80_HL, 0);
    
    const uint8_t orig_a = Z80_A;
    Z80_A = Z80_A | z80.read_buffer[0];
    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_SIGN(Z80_A);
    Z80_F |= Z80_SETFLAG_ZERO(Z80_A);
    Z80_F |= Z80_SETFLAG_OVERFLOW(orig_a, Z80_A);
    return Z80_STAGE_RESET;
    
}

///OR n; Size: 2; Flags:ALL
int OR_n(){
    assert(z80.opcode_index == 2);
    const uint8_t orig_a = Z80_A;
    Z80_A = Z80_A | z80.opcode[1];
    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_SIGN(Z80_A);
    Z80_F |= Z80_SETFLAG_ZERO(Z80_A);
    Z80_F |= Z80_SETFLAG_OVERFLOW(orig_a, Z80_A);
    return Z80_STAGE_RESET;
}

///OR r[z]; Size: 1; Flags: All
int OR_r(){
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    const uint8_t orig_a = Z80_A;

    Z80_A = Z80_A | *(z80_r[z[0]]);
    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_SIGN(Z80_A);
    Z80_F |= Z80_SETFLAG_ZERO(Z80_A);
    Z80_F |= Z80_SETFLAG_OVERFLOW(orig_a, Z80_A);
    return Z80_STAGE_RESET;
}

///OTDR; Size: 2; Flags: ???
int OTDR(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///OTIR; Size: 2; Flags: Z,N
int OTIR(){
    assert(z80.opcode_index == 2);
    //(C)<-(HL), B<-B  1, HL<-HL + 1; B? repeat : end
    //Perform read
    Z80_8BIT_READ(Z80_HL, 0);
    //Perform write
    Z80_8BIT_WRITE(Z80_C | (((uint16_t)Z80_A) << 8), 1, z80.read_buffer[0]);
    //Update state and flags
    ++Z80_HL;
    --Z80_B;
    Z80_F = Z80_F & (Z80_CLRFLAG_ZERO & Z80_CLRFLAG_SUBTRACT); //Z,N
    if (Z80_B){ //Repeat instruction
        Z80_PC = Z80_PC - 2;
        return Z80_STAGE_RESET;
    }
    else{
        return Z80_STAGE_RESET;
    }
}

///OUT (n), A; Size: 2; Flags: None
int OUT_np_A(){
    assert(z80.opcode_index == 2);
    const uint16_t port_addr = z80.opcode[1] + (((uint16_t)Z80_A) << 8);
    Z80_8BIT_WRITE(port_addr, 1, Z80_A);
    return Z80_STAGE_RESET;
}

///OUT (C), 0; Size: 2; Flags: None
int OUT_Cp_0(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///OUT (C), r; Size: 2; Flags: None
int OUT_Cp_r(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///OUTD; Size: 2; Flags: ???
int OUTD(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///OUTI; Size: 2; Flags: ???
int OUTI(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///POP rp2[p]; Size: 1; Flags: None
int POP_rp2(){
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    //Read stack
    Z80_16BIT_READ(Z80_SP, 0);

    Z80_SP += 2;
    *(z80_rp2[p[0]]) = *((uint16_t*)z80.read_buffer); ///<-- @bug Endianness!
    return Z80_STAGE_RESET;
}

///PUSH rp2[p]; Size: 1; Flags: None
int PUSH_rp2(){
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    Z80_16BIT_WRITE(Z80_SP - 2, 0, (*(z80_rp2[p[0]])) & 0xFF, ((*(z80_rp2[p[0]])) >> 8) & 0xFF);
    Z80_SP -= 2;
    return Z80_STAGE_RESET;
}

///RES y, (HL); Size: 2; Flags: None
int RES_b_HLp(){
    assert(z80.opcode_index == 2);
    assert(0);
    return Z80_STAGE_RESET;
}

///RES y,r[z]; Size: 2; Flags: None
int RES_b_r(){
    assert(z80.opcode_index == 2);
    Z80_OPCODE_SUBDIV;
    *(z80_r[z[1]]) = *(z80_r[z[1]]) & (~(1 << y[1]));
    return Z80_STAGE_RESET;
}

///RET; Size: 1; Flags: None
int RET(){
    assert(z80.opcode_index == 1);
    //Read stack
    Z80_16BIT_READ(Z80_SP, 0);

    Z80_SP += 2;
    Z80_PC = z80.read_buffer[0] | (((uint16_t)(z80.read_buffer[1])) << 8);
    return Z80_STAGE_RESET;
}

///RET cc[y]; Size: 1; Flags: None
int RET_cc(){
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    if ((Z80_F & z80_cc[y[0]]) == z80_cc_stat[y[0]]){
        //POP stack, update PC
        Z80_16BIT_READ(Z80_SP, 0);
        
        Z80_PC = *((uint16_t*)(z80.read_buffer));
        Z80_SP += 2;
        return Z80_STAGE_RESET;
    }
    else{
        return Z80_STAGE_RESET;
    }
}

///RETI; Size: 2; Flags: None
///@note essentially identical to RET(?)
int RETI(){
    assert(z80.opcode_index == 2);
    //Read stack
    Z80_16BIT_READ(Z80_SP, 0);

    Z80_SP += 2;
    Z80_PC = z80.read_buffer[0] | (((uint16_t)(z80.read_buffer[1]))<<8);
    return Z80_STAGE_RESET;
}

///RETN; Size: 2; Flags: None
int RETN(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///RLA; Size: 2; Flags: HC,A,C
int RLA(){
    assert(z80.opcode_index == 2);
    const uint8_t next_carry = Z80_A & (1 << 7);
    Z80_A = (Z80_A << 1) | (Z80_F & Z80_FLAG_CARRY ? 1 : 0);
    Z80_F = (Z80_A & (Z80_CLRFLAG_HC & Z80_CLRFLAG_SUBTRACT & Z80_CLRFLAG_CARRY))
        | (next_carry ? Z80_FLAG_CARRY : 0);
    return Z80_STAGE_RESET;
}

///RLCA; Size: 1; Flags: H,N,C
int RLCA(){
    assert(z80.opcode_index == 1);
    Z80_A = (Z80_A << 1) | (Z80_A & (1 << 7) ? 1 : 0);
    Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_SUBTRACT & Z80_CLRFLAG_CARRY))
        | ((Z80_A & (1)) ? Z80_FLAG_CARRY : 0);
    return Z80_STAGE_RESET;
}

///RLC r; Size: 2; Flags: ???
int RLC_r(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///RLC (HL); Size: 2; Flags: ???
int RLC_HLp() {
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///RLC (IX+d); Size: 4; Flags: All
int RLC_IXYp() {
    assert(z80.opcode_index == 4);
    const int8_t d = (int8_t)z80.opcode[2];
    /*Perform memory read*/
    if (z80.opcode[0] == 0xDD){
        Z80_8BIT_READ(Z80_IX + d, 0);
    }
    else{
        Z80_8BIT_READ(Z80_IY + d, 0);
    }
    /*Perform rotation*/
    const uint8_t carry_out = ((z80.read_buffer[0] >> 7) & 0x01);
    const uint8_t result = ((z80.read_buffer[0] << 1) & 0xFE) | carry_out;
    /*Flags*/
    Z80_F =
        Z80_SETFLAG_SIGN(result)
        | Z80_SETFLAG_ZERO(result)
        | Z80_SETFLAG_PARITY(result)
        | (carry_out ? Z80_FLAG_CARRY : 0)
        ;
    /*Perform write back*/
    if (z80.opcode[0] == 0xDD){
        Z80_8BIT_WRITE(Z80_IX + d, 0, result);
    }
    else{
        Z80_8BIT_WRITE(Z80_IY + d, 0, result);
    }
    return Z80_STAGE_RESET;
}

///RLD; Size: 2; Flags: ???
int RLD(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///RL (HL); Size: 2; Flags: ???
int RL_HLp(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}


///RL r; Size: 2; Flags: ???
int RL_r(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///RRA; Size: 1; Flags: H,N,C
int RRA(){
    assert(z80.opcode_index == 1);
    const uint8_t next_carry = Z80_A & (1);
    Z80_A = (Z80_A >> 1) | (Z80_F & Z80_FLAG_CARRY ? (1 << 7) : 0);
    Z80_F = (Z80_A & (Z80_CLRFLAG_HC & Z80_CLRFLAG_SUBTRACT & Z80_CLRFLAG_CARRY))
        | (next_carry ? Z80_FLAG_CARRY : 0);
    return Z80_STAGE_RESET;
}

///RRCA; Size: 1; Flags: H,N,C
int RRCA(){
    assert(z80.opcode_index == 1);
    Z80_A = (Z80_A >> 1) | (Z80_A & 1 ? (1 << 7) : 0);
    Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_SUBTRACT & Z80_CLRFLAG_CARRY))
        | ((Z80_A & (1 << 7)) ? Z80_FLAG_CARRY : 0);
    return Z80_STAGE_RESET;
}

///RRC (HL); Size: 2; Flags: ???
int RRC_HLp(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///RRC r; Size: 2; Flags: ???
int RRC_r(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///RRD; Size: 2; Flags: ???
int RRD(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///RR (HL); Size: 2; Flags: ???
int RR_HLp(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///RR r; Size: 2; Flags: ???
int RR_r(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///RST y; Size: 1; Flags: None
int RST_y(){
    Z80_OPCODE_SUBDIV;
    assert(z80.opcode_index == 1);
    Z80_16BIT_WRITE(Z80_SP - 2, 0, Z80_PC & 0x00FF, (Z80_PC >> 8) & 0x00FF);
    Z80_SP = Z80_SP - 2;
    Z80_PC = 0x08 * y[0];
    return Z80_STAGE_RESET;
}

///SBC HL,rp[p]; Size: 2; Flags: ?
int SBC_HL_rp(){
    assert(z80.opcode_index == 2);
    Z80_OPCODE_SUBDIV;
    const uint16_t old_hl = Z80_HL;
    Z80_HL = (Z80_F & Z80_FLAG_CARRY) ? Z80_HL - *(z80_rp[p[1]]) : Z80_HL - *(z80_rp[p[1]]) - 1;
    Z80_F = Z80_FLAG_SUBTRACT
        | Z80_SETFLAG_BORROW(old_hl, Z80_HL)
        | Z80_SETFLAG_SIGN(Z80_HL)
        | Z80_SETFLAG_ZERO(Z80_HL)
        | Z80_SETFLAG_HALF_BORROW_16(old_hl, *(z80_rp[p[1]]))
        | Z80_SETFLAG_OVERFLOW(old_hl, Z80_HL);
    return Z80_STAGE_RESET;
}

///SBC (HL); Size: 1; Flags: ???
int SBC_HLp(){
    assert(z80.opcode_index == 1);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SBC n; Size: 2; Flags: ???
int SBC_n(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SBC r; Size: 1; Flags: ???
int SBC_r(){
    assert(z80.opcode_index == 1);
    assert(0);
    return Z80_STAGE_RESET;
}

///SET y, (HL); Size: 2; Flags: None
int SET_b_HLp(){
    assert(z80.opcode_index == 2);
    assert(0);
    return Z80_STAGE_RESET;
}

///SET y, r; Size: 2; Flags: None
int SET_b_r(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SCF; Size: 1; Flags: C
int SCF(){
    assert(z80.opcode_index == 1);
    Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_SUBTRACT)) | Z80_FLAG_CARRY;
    return Z80_STAGE_RESET;
}

///SLA (HL); Size: 2; Flags: ???
int SLA_HLp(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SLA r; Size: 2; Flags: ???
int SLA_r(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SLL (HL); Size: 2; Flags: ???
int SLL_HLp(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SLL r; Size: 2; Flags: ???
int SLL_r(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SRA (HL); Size: 2; Flags: ???
int SRA_HLp(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SRA r; Size: 2; Flags: ???
int SRA_r(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SRL (HL); Size: 2; Flags: ???
int SRL_HLp(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SRL r[z]; Size: 2; Flags: ?
int SRL_r(){
    assert(z80.opcode_index == 2);
    Z80_OPCODE_SUBDIV;
    if (z80_r[z[1]]){
        const uint8_t old_r = *(z80_r[z[1]]);
        *(z80_r[z[1]]) = (*(z80_r[z[1]])) >> 1;
        Z80_F = 0;
        Z80_F |= old_r & 1 ? Z80_FLAG_CARRY : 0;
        Z80_F |= Z80_SETFLAG_ZERO(*(z80_r[z[1]]));
        Z80_F |= Z80_SETFLAG_PARITY(*(z80_r[z[1]]));
        return Z80_STAGE_RESET;
    }
    else{ //SRL (HL)
        assert(0); //Unimplemented
    }
    assert(0); //Unimplemented
    return Z80_STAGE_RESET;
}

///SUB (HL); Size: 1; Flags: ???
int SUB_HLp(){
    assert(z80.opcode_index == 1);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SUB n; Size: 2; Flags: ???
int SUB_n(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SUB r; Size: 1; Flags: ???
int SUB_r(){
    assert(z80.opcode_index == 1);
    assert(0);
    return Z80_STAGE_RESET;
}

///XOR (HL); Size: 1; Flags: All
int XOR_HLp(){
    assert(z80.opcode_index == 1);
    Z80_8BIT_READ(Z80_HL, 0);
    Z80_A = Z80_A ^ z80.read_buffer[0];

    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_ZERO(Z80_A);
    Z80_F |= Z80_SETFLAG_SIGN(Z80_A);
    Z80_F |= Z80_SETFLAG_PARITY(Z80_A);
    return Z80_STAGE_RESET;
}

///XOR n; Size: 2; Flags: ???
int XOR_n(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- unimplemented
    return Z80_STAGE_RESET;
}

///XOR r[z]; Size: 1; Flags: All
int XOR_r(){
    Z80_OPCODE_SUBDIV;
    assert(z80.opcode_index == 1);
    assert(z80_r[z[0]] != 0);

    Z80_A = Z80_A ^ *(z80_r[z[0]]);

    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_SIGN(Z80_A);
    Z80_F |= Z80_SETFLAG_ZERO(Z80_A);
    Z80_F |= Z80_SETFLAG_PARITY(Z80_A);
    return Z80_STAGE_RESET;
}

/* Stubs for IX/IY/(IX+d)/(IY+d)*/
int ADD_IXY_rp(){
    assert(0); /*<-- Unimplemented*/
    return Z80_STAGE_RESET;
}

int LD_IXY_nn(){
    Z80_OPCODE_SUBDIV;
    assert(z80.opcode_index == 4);
    assert(p[1] == 2);
    if (z80.opcode[0] == 0xDD){
        Z80_IX = (((uint16_t)z80.opcode[3]) << 8) | z80.opcode[2];
    }
    else{
        assert(z80.opcode[0] == 0xFD);
        Z80_IY = (((uint16_t)z80.opcode[3]) << 8) | z80.opcode[2];
    }
    return Z80_STAGE_RESET;
}

int LD_IXYH_n(){
    Z80_OPCODE_SUBDIV;
    assert(z80.opcode_index == 3);
    assert(y[1] == 4);
    if (z80.opcode[0] == 0xDD){
        Z80_IXH = z80.opcode[2];
    }
    else{
        assert(z80.opcode[0] == 0xFD);
        Z80_IYH = z80.opcode[2];
    }
    return Z80_STAGE_RESET;
}

int LD_IXYL_n(){
    Z80_OPCODE_SUBDIV;
    assert(z80.opcode_index == 3);
    assert(y[1] == 5);
    if (z80.opcode[0] == 0xDD){
        Z80_IXL = z80.opcode[2];
    }
    else{
        assert(z80.opcode[0] == 0xFD);
        Z80_IYL = z80.opcode[2];
    }
    return Z80_STAGE_RESET;
}

int LD_nnp_IXY(){
    assert(z80.opcode_index == 4);
    if (z80.opcode[0] == 0xDD){
        Z80_16BIT_WRITE(z80.opcode[2] | (((uint16_t)z80.opcode[3]) << 8), 0, Z80_IXL, Z80_IXH);
    }
    else{
        assert(z80.opcode[0] == 0xFD);
        Z80_16BIT_WRITE(z80.opcode[2] | (((uint16_t)z80.opcode[3]) << 8), 0, Z80_IYL, Z80_IYH);
    }
    return Z80_STAGE_RESET;
}

int INC_IXY(){
    assert(0); /*<-- Unimplemented*/
    return Z80_STAGE_RESET;
}

int INC_IXYp(){
    assert(0); /*<-- Unimplemented*/
    return Z80_STAGE_RESET;
}

int LD_IXY_nnp(){
    assert(z80.opcode_index == 4);
    Z80_16BIT_READ(z80.opcode[2] | (((uint16_t)z80.opcode[3]) << 8), 0);
    assert(z80.read_index == 2);
    if (z80.opcode[0] == 0xDD){
        Z80_IX = z80.read_buffer[0] | (((uint16_t)z80.read_buffer[1])<<8);
    }
    else{
        assert(z80.opcode[0] == 0xFD);
        Z80_IY = z80.read_buffer[0] | (((uint16_t)z80.read_buffer[1]) << 8);
    }
    return Z80_STAGE_RESET;
}

int DEC_IXY(){
    assert(0); /*<-- Unimplemented*/
    return Z80_STAGE_RESET;
}

int DEC_IXYp(){
    assert(0); /*<-- Unimplemented*/
    return Z80_STAGE_RESET;
}

int LD_IXYp_n(){
    assert(z80.opcode_index == 4);
    if (z80.opcode[0] == 0xDD){
        Z80_8BIT_WRITE(Z80_IX + ((int8_t)z80.opcode[2]), 0, z80.opcode[3]);
    }
    else{
        assert(z80.opcode[0] == 0xFD);
        Z80_8BIT_WRITE(Z80_IY + ((int8_t)z80.opcode[2]), 0, z80.opcode[3]);
    }
    return Z80_STAGE_RESET;
}

int LD_r_IXYp(){
    Z80_OPCODE_SUBDIV;
    assert(z80.opcode_index == 3);
    if (z80.opcode[0] == 0xDD){
        Z80_8BIT_READ(Z80_IX + ((int8_t)z80.opcode[2]), 0);
    }
    else{
        assert(z80.opcode[0] == 0xFD);
        Z80_8BIT_READ(Z80_IY + ((int8_t)z80.opcode[2]), 0);
    }
    *(z80_r[y[1]]) = z80.read_buffer[0];
    return Z80_STAGE_RESET;
}

int LD_IXYp_r(){
    Z80_OPCODE_SUBDIV;
    assert(z80.opcode_index == 3);
    assert(z[1] != 6);
    if (z80.opcode[0] == 0xDD){
        Z80_8BIT_WRITE(Z80_IX + ((int8_t)z80.opcode[2]), 0, *(z80_r[z[1]]));
    }
    else{
        assert(z80.opcode[0] == 0xFD);
        Z80_8BIT_WRITE(Z80_IY + ((int8_t)z80.opcode[2]), 0, *(z80_r[z[1]]));
    }
    return Z80_STAGE_RESET;
}

int ADD_IXYp(){
    assert(0); /*<-- Unimplemented*/
    return Z80_STAGE_RESET;
}

int ADC_IXYp(){
    assert(0); /*<-- Unimplemented*/
    return Z80_STAGE_RESET;
}

int SUB_IXYp(){
    assert(0); /*<-- Unimplemented*/
    return Z80_STAGE_RESET;
}

int SBC_IXYp(){
    assert(0); /*<-- Unimplemented*/
    return Z80_STAGE_RESET;
}

int AND_IXYp(){
    assert(0); /*<-- Unimplemented*/
    return Z80_STAGE_RESET;
}

int XOR_IXYp(){
    assert(0); /*<-- Unimplemented*/
    return Z80_STAGE_RESET;
}

int OR_IXYp(){
    assert(0); /*<-- Unimplemented*/
    return Z80_STAGE_RESET;
}

int CP_IXYp(){
    assert(0); /*<-- Unimplemented*/
    return Z80_STAGE_RESET;
}

///POP IX/POP IY; Size: 2; Flags: None
int POP_IXY(){
    Z80_OPCODE_SUBDIV;
    assert(z80.opcode_index == 2);
    assert(p[1] == Z80_RP_INDEX_HL);

    Z80_16BIT_READ(Z80_SP, 0);
    if (z80.opcode[0] == 0xDD){
        Z80_IXL = z80.read_buffer[0];
        Z80_IXH = z80.read_buffer[1];
    }
    else{
        Z80_IYL = z80.read_buffer[0];
        Z80_IYH = z80.read_buffer[1];
    }
    Z80_SP = Z80_SP + 2;
    return Z80_STAGE_RESET;
}

int EX_SPp_IXY(){
    assert(0); /*<-- Unimplemented*/
    return Z80_STAGE_RESET;
}

int PUSH_IXY(){
    assert(z80.opcode_index == 2);
    if (z80.opcode[0] == 0xDD){
        Z80_16BIT_WRITE(Z80_SP - 2, 0, Z80_IXL, Z80_IXH);
    }
    else{
        Z80_16BIT_WRITE(Z80_SP - 2, 0, Z80_IYL, Z80_IYH);
    }
    Z80_SP = Z80_SP - 2;
    return Z80_STAGE_RESET;
}

int JP_IXYp(){
    assert(0); /*<-- Unimplemented*/
    return Z80_STAGE_RESET;
}

int LD_SP_IXY(){
    assert(z80.opcode_index == 2);
    if (z80.opcode[0] == 0xDD){
        Z80_SP = Z80_IX;
    }
    else{
        assert(z80.opcode[0] == 0xFD);
        Z80_SP = Z80_IY;
    }
    return Z80_STAGE_RESET;
}

int SLL_IXYp(){
    /*Select index register address*/
    const uint16_t address = (z80.opcode[0] == 0xDD) ? Z80_IX : Z80_IY;
    /*Read*/
    const int8_t d = (int8_t)z80.opcode[2];
    Z80_8BIT_READ(address + d, 0);
    /*Execute opcode and update flags*/
    const uint8_t next_c = z80.read_buffer[0] & 0x80;
    const uint8_t next_b = (z80.read_buffer[0] << 1) | 0x01;
    /*Write back*/
    Z80_8BIT_WRITE(address + d, 0, next_b);
    /*Update flags*/
    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_ZERO(next_b);
    Z80_F |= next_c ? Z80_FLAG_CARRY : 0;
    Z80_F |= Z80_SETFLAG_SIGN(next_b);
    Z80_F |= Z80_SETFLAG_PARITY(next_b);
    return Z80_STAGE_RESET;
}

int RRC_IXYp(){
    /*Select index register address*/
    const uint16_t address = (z80.opcode[0] == 0xDD) ? Z80_IX : Z80_IY;
    /*Read*/
    const int8_t d = (int8_t)z80.opcode[2];
    Z80_8BIT_READ(address + d, 0);
    /*Execute opcode and update flags*/
    const uint8_t next_c = z80.read_buffer[0] & 0x01;
    const uint8_t next_b = (z80.read_buffer[0] >> 1) & 0x7F;
    /*Write back*/
    Z80_8BIT_WRITE(address + d, 0, next_b);
    /*Update flags*/
    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_ZERO(next_b);
    Z80_F |= next_c ? Z80_FLAG_CARRY : 0;
    Z80_F |= Z80_SETFLAG_SIGN(next_b);
    Z80_F |= Z80_SETFLAG_PARITY(next_b);
    return Z80_STAGE_RESET;
}

int RL_IXYp(){
    /*Select index register address*/
    const uint16_t address = (z80.opcode[0] == 0xDD) ? Z80_IX : Z80_IY;
    /*Read*/
    const int8_t d = (int8_t)z80.opcode[2];
    Z80_8BIT_READ(address + d, 0);
    /*Execute opcode and update flags*/
    const uint8_t next_lsb = Z80_C ? 1 : 0;
    const uint8_t next_c = z80.read_buffer[0] & 0x80;
    const uint8_t next_b = (z80.read_buffer[0] << 1) | next_lsb;
    /*Write back*/
    Z80_8BIT_WRITE(address + d, 0, next_b);
    /*Update flags*/
    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_ZERO(next_b);
    Z80_F |= next_c ? Z80_FLAG_CARRY : 0;
    Z80_F |= Z80_SETFLAG_SIGN(next_b);
    Z80_F |= Z80_SETFLAG_PARITY(next_b);
    return Z80_STAGE_RESET;
}

int SLA_IXYp(){
    /*Select index register address*/
    const uint16_t address = (z80.opcode[0] == 0xDD) ? Z80_IX : Z80_IY;
    /*Read*/
    const int8_t d = (int8_t)z80.opcode[2];
    Z80_8BIT_READ(address + d, 0);
    /*Execute opcode and update flags*/
    const uint8_t next_c = z80.read_buffer[0] & 0x80;
    const uint8_t next_b = (z80.read_buffer[0] << 1);
    /*Write back*/
    Z80_8BIT_WRITE(address + d, 0, next_b);
    /*Update flags*/
    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_ZERO(next_b);
    Z80_F |= next_c ? Z80_FLAG_CARRY : 0;
    Z80_F |= Z80_SETFLAG_SIGN(next_b);
    Z80_F |= Z80_SETFLAG_PARITY(next_b);
    return Z80_STAGE_RESET;
}

int RR_IXYp(){
    /*Select index register address*/
    const uint16_t address = (z80.opcode[0] == 0xDD) ? Z80_IX : Z80_IY;
    /*Read*/
    const int8_t d = (int8_t)z80.opcode[2];
    Z80_8BIT_READ(address + d, 0);
    /*Execute opcode and update flags*/
    const uint8_t next_msb = Z80_C ? 0x80 : 0;
    const uint8_t next_c = z80.read_buffer[0] & 0x01;
    const uint8_t next_b = ((z80.read_buffer[0] >> 1) & 0x7F) | next_msb;
    /*Write back*/
    Z80_8BIT_WRITE(address + d, 0, next_b);
    /*Update flags*/
    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_ZERO(next_b);
    Z80_F |= next_c ? Z80_FLAG_CARRY : 0;
    Z80_F |= Z80_SETFLAG_SIGN(next_b);
    Z80_F |= Z80_SETFLAG_PARITY(next_b);
    return Z80_STAGE_RESET;
}

int SRA_IXYp(){
    /*Select index register address*/
    const uint16_t address = (z80.opcode[0] == 0xDD) ? Z80_IX : Z80_IY;
    /*Read*/
    const int8_t d = (int8_t)z80.opcode[2];
    Z80_8BIT_READ(address + d, 0);
    /*Execute opcode and update flags*/
    const uint8_t next_c = z80.read_buffer[0] & 0x01;
    const uint8_t next_b = (z80.read_buffer[0] >> 1) | (z80.read_buffer[0] & 0x80);
    /*Write back*/
    Z80_8BIT_WRITE(address + d, 0, next_b);
    /*Update flags*/
    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_ZERO(next_b);
    Z80_F |= next_c ? Z80_FLAG_CARRY : 0;
    Z80_F |= Z80_SETFLAG_SIGN(next_b);
    Z80_F |= Z80_SETFLAG_PARITY(next_b);
    return Z80_STAGE_RESET;
}

///SRL (IX+d); Size: 4; Flags: All
int SRL_IXYp(){
	assert(z80.opcode_index == 4);
	const int8_t d = (int8_t)z80.opcode[2];
	/*Perform read*/
	if (z80.opcode[0] == 0xDD){
		Z80_8BIT_READ(Z80_IX + d, 0);
	}
	else{
		Z80_8BIT_READ(Z80_IY + d, 0);
	}
	/*Perform shift*/
	const uint8_t carry_out = z80.read_buffer[0] & 0x01;
	const uint8_t result = (z80.read_buffer[0] >> 1) & 0x7F;
	/*Flags*/
	Z80_F = 
		Z80_SETFLAG_ZERO(result)
		| Z80_SETFLAG_PARITY(result)
		| (carry_out ? Z80_FLAG_CARRY : 0)
		;
	/*Write back*/
	if (z80.opcode[0] == 0xDD){
		Z80_8BIT_WRITE(Z80_IX + d, 0, result);
	}
	else{
		Z80_8BIT_WRITE(Z80_IY + d, 0, result);
	}
    return Z80_STAGE_RESET;
}

int SET_b_IXYp(){
    assert(0); /*<-- Unimplemented*/
    return Z80_STAGE_RESET;
}

int RES_b_IXYp(){
    assert(0); /*<-- Unimplemented*/
    return Z80_STAGE_RESET;
}

int BIT_b_IXYp(){
    assert(0); /*<-- Unimplemented*/
    return Z80_STAGE_RESET;
}

int UNDOC(){
    assert(0); /*<-- Stop execution*/
    return Z80_STAGE_RESET;
}