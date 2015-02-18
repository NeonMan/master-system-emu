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

#include "z80_opcodes.h"
#include "z80_macros.h"
#include "z80_register_lut.h"
#include <assert.h>

extern struct z80_s z80; //<-- Access to z80 internals

///ADC n; Size: 2; Flags: ???
int z80_op_ADC_n(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///ADC HL, rp; Size: 2; Flags: ???
int z80_op_ADC_HL_rp(){
    assert(z80.opcode_index == 2); 
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///ADC r; Size: 1; Flags: ???
int z80_op_ADC_r(uint8_t prefixed){
    assert(prefixed == 0); //<-- Prefix unimplemented
    assert(z80.opcode_index == 1); 
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///ADD HL, rp[p] ; Size: 1; Flags N,C
int z80_op_ADD_HL_rp(uint8_t prefixed){
    assert(prefixed == 0); //<-- Prefix unimplemented
    assert(z80.opcode_index == 1); 
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
    assert(z80.opcode_index == 2);
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

///ADD r; Size: 1; Flags:ALL
int z80_op_ADD_r(uint8_t prefixed){
    assert(prefixed == 0); //<-- Prefix unimplemented
    assert(z80.opcode_index == 1);
    assert(0);
    return Z80_STAGE_RESET;
}

///AND n; Size 2; Flags:ALL
int z80_op_AND_n(){
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
int z80_op_AND_r(uint8_t prefixed){
    assert(prefixed == 0); //<-- Prefix unimplemented
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    //Request M2 read if required
    if ((z80_r[z[0]] == 0) && (z80.read_index == 0)){
        z80.read_address = Z80_HL;
        return Z80_STAGE_M2;
    }

    const uint8_t orig_a = Z80_A;
    if (z80_r[z[0]]){                            /*AND r[z]; Size: 1; Flags: All*/
        Z80_A = Z80_A & *(z80_r[z[0]]);
    }
    else{                                        /*AND (HL); Size: 1; Flags: All*/
        Z80_A = Z80_A & z80.read_buffer[0];
    }
    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_SIGN(Z80_A);
    Z80_F |= Z80_SETFLAG_ZERO(Z80_A);
    Z80_F |= Z80_FLAG_HC;
    Z80_F |= Z80_SETFLAG_OVERFLOW(orig_a, Z80_A);
    return Z80_STAGE_RESET;
}

///BIT y,r[z]; Size: 2; Flags: _S,Z,H,_P,N
int z80_op_BIT_y_r(){
    assert(z80.opcode_index == 2);
    Z80_OPCODE_SUBDIV;
    Z80_F = (Z80_F & (Z80_CLRFLAG_ZERO & Z80_CLRFLAG_ADD)); //Clear Z,N
    Z80_F = Z80_F | (((1 << y[1]) & (*z80_r[z[1]])) ? 0 : Z80_CLRFLAG_ZERO);
    Z80_F = Z80_F | Z80_FLAG_HC;
    return Z80_STAGE_RESET;
}

///CALL cc[y] nn; Size: 3; Flags: None
int z80_op_CALL_cc_nn(){
    assert(z80.opcode_index == 3);
    Z80_OPCODE_SUBDIV;
    //Check condition
    if ((Z80_F & z80_cc[y[0]]) == (z80_cc_stat[y[0]])){
        //Push PC onto the stack
        if (z80.write_index == 0){
            //Push PC to the stack (M3 write of current PC)
            *((uint16_t*)(z80.write_buffer)) = Z80_PC; ///<-- @bug Endianness!
            z80.write_address = Z80_SP - 2;
            return Z80_STAGE_M3;
        }
        else if (z80.write_index == 1){
            //Write the second byte
            ++z80.write_address;
            return Z80_STAGE_M3;
        }
        else{
            //Update SP
            Z80_SP -= 2;
            //Update PC
            const uint16_t new_pc = *((uint16_t*)(z80.opcode + 1)); ///<-- @bug Endiannes!
            Z80_PC = new_pc;
            return Z80_STAGE_RESET;
        }
    }
    else{
        return Z80_STAGE_RESET;
    }
}

///CALL nn; Size: 3; Flags: None
int z80_op_CALL_nn(){
    assert(z80.opcode_index == 3);
    if (z80.write_index == 0){
        //Push PC to the stack (M3 write of current PC)
        *((uint16_t*)(z80.write_buffer)) = Z80_PC; ///<-- @bug Endianness!
        z80.write_address = Z80_SP - 2;
        return Z80_STAGE_M3;
    }
    else if (z80.write_index == 1){
        //Write the second byte
        ++z80.write_address;
        return Z80_STAGE_M3;
    }
    else{
        //Update SP
        Z80_SP -= 2;
        //Update PC
        const uint16_t new_pc = *((uint16_t*)(z80.opcode + 1)); ///<-- @bug Endiannes!
        Z80_PC = new_pc;
        return Z80_STAGE_RESET;
    }
}

///CCF; Size: 1; Flags: C
int z80_op_CCF(){
    assert(z80.opcode_index == 1);
    Z80_F = (Z80_F ^ Z80_FLAG_CARRY) & Z80_CLRFLAG_ADD;
    return Z80_STAGE_RESET;
}

///CPD; Size: 2; Flags: ???
int z80_op_CPD(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///CPI; Size: 2; Flags: ???
int z80_op_CPI(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///CPDR; Size: 2; Flags: ???
int z80_op_CPDR(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///CPIR; Size: 2; Flags: ???
int z80_op_CPIR(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///CP n; Size: 2; Flags: All
int z80_op_CP_n(){
    assert(z80.opcode_index == 2);
    Z80_F = 0;
    Z80_F |= Z80_FLAG_ADD; //Flag is set, always
    Z80_F |= Z80_SETFLAG_SIGN(Z80_A - z80.opcode[1]);
    Z80_F |= Z80_SETFLAG_ZERO(Z80_A - z80.opcode[1]);
    Z80_F |= Z80_SETFLAG_HC(Z80_A, Z80_A - z80.opcode[1]);
    Z80_F |= Z80_SETFLAG_OVERFLOW(Z80_A, Z80_A - z80.opcode[1]);
    Z80_F |= Z80_SETFLAG_BORROW(Z80_A, Z80_A - z80.opcode[1]);
    return Z80_STAGE_RESET;
}

///CP r[z]; Size: 1; Flags: All
int z80_op_CP_r(uint8_t prefixed){
    assert(prefixed == 0); //<-- Prefix unimplemented
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    //Request M2 read if required
    if ((z80_r[z[0]] == 0) && (z80.read_index == 0)){
        z80.read_address = Z80_HL;
        return Z80_STAGE_M2;
    }

    uint8_t new_a;
    if (z80_r[z[0]]){
        new_a = Z80_A - *(z80_r[z[0]]);
    }
    else{                                         /*CP (HL); Size: 1; Flags: All*/
        new_a = Z80_A - z80.read_buffer[0];
    }
    Z80_F = Z80_SETFLAG_SIGN(Z80_A)
        | Z80_SETFLAG_ZERO(Z80_A)
        | Z80_SETFLAG_HC(Z80_A, new_a)
        | Z80_SETFLAG_OVERFLOW(Z80_A, new_a)
        | Z80_FLAG_ADD
        | Z80_SETFLAG_BORROW(Z80_A, new_a);
    return Z80_STAGE_RESET;
}

///CPL; Size: 1; Flags: H,N
int z80_op_CPL(){
    assert(z80.opcode_index == 1);
    Z80_A = ~Z80_A;
    Z80_F = Z80_F | Z80_FLAG_HC | Z80_FLAG_ADD;
    return Z80_STAGE_RESET;
}

///DAA; Size: 1; Flags: ???
int z80_op_DAA(){
    assert(z80.opcode_index == 1);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///DEC(r[y]); Size:1; Flags: S,Z,H,P,N
///@bug will crash on (HL)
int z80_op_DEC_r(uint8_t prefixed){
    assert(prefixed == 0); //<-- Prefix unimplemented
    assert(z80.opcode_index == 1);
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
int z80_op_DEC_rp(uint8_t prefixed){
    assert(prefixed == 0); //<-- Prefix unimplemented
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    --(*(z80_rp[p[0]]));
    return Z80_STAGE_RESET;
}

///DI; Size: 1; Flags: None
int z80_op_DI(){
    assert(z80.opcode_index == 1);
    z80.iff[0] = 0;
    z80.iff[1] = 0;
    return Z80_STAGE_RESET;
}

///EI; Size: 1; Flags: None
int z80_op_EI(){
    assert(z80.opcode_index == 1);
    z80.iff[0] = 1;
    z80.iff[1] = 1;
    return Z80_STAGE_RESET;
}

///DJNZ e; Size: 2; Flags: None
int z80_op_DJNZ_d(){
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
int z80_op_EX(){
    assert(z80.opcode_index == 1);
    const uint16_t tmp_af = Z80_AF;
    Z80_AF = Z80_AFp;
    Z80_AFp = tmp_af;
    return Z80_STAGE_RESET;
}

///EX DE, HL; Size: 1; Flags: None
int z80_op_EX_DE_HL(){
    assert(z80.opcode_index == 1);
    const uint16_t old_de = Z80_DE;
    Z80_DE = Z80_HL;
    Z80_HL = old_de;
    return Z80_STAGE_RESET;
}

///EX (SP), HL; Size: 1; Flags: ???
int z80_op_EX_SPp_HL(){
    assert(z80.opcode_index == 1);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///EXX; Size: 1; Flags: None
int z80_op_EXX(){
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

///HALT; Size: 1; Flags: ???
int z80_op_HALT(){
    assert(z80.opcode_index == 1);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///IM(im[y]); Size: 2; Flags: None
int z80_op_IM(){
    assert(z80.opcode_index == 2);
    Z80_OPCODE_SUBDIV;
    z80.iff[0] = z80_im[y[1]][0];
    z80.iff[1] = z80_im[y[1]][1];
    return Z80_STAGE_RESET;
}

///IN A, (n); Size: 2; Flags: None
int z80_op_IN_A_np(){
    assert(z80.opcode_index == 2);
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

///IN (C); Size: 2; Flags: ???
int z80_op_IN_Cp(){
    assert(z80.opcode_index == 2);
    assert(0);
    return Z80_STAGE_RESET;
}

///IN r, (C); Size: 2; Flags: ???
int z80_op_IN_r_Cp(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///INC (HL); Size: 1; Flags: All 
int z80_op_INC_HLp(uint8_t prefixed){
    assert(prefixed == 0); //<-- Prefix unimplemented
    assert(z80.opcode_index == 1);
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

///INC r; Size: 1; Flags: All
int z80_op_INC_r(uint8_t prefixed){
    assert(prefixed == 0); //<-- Prefix unimplemented
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    //If target is (HL)
    if (!(z80_r[y[0]])) return z80_op_INC_HLp(prefixed);
    
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
int z80_op_INC_rp(uint8_t prefixed){
    assert(prefixed == 0); //<-- Prefix unimplemented
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    ++(*(z80_rp[p[0]]));
    return Z80_STAGE_RESET;
}

///IND; Size: 2; Flags: ???
int z80_op_IND(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///INDR; Size: 2; Flags: ???
int z80_op_INDR(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///INI; Size: 2; Flags: ???
int z80_op_INI(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///INIR; Size: 2; Flags: ???
int z80_op_INIR(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///JP nn; Size: 3; Flags: None
int z80_op_JP_nn(){
    assert(z80.opcode_index == 3);
    ///New PC stored in opcode's last bytes. @bug Endianness.
    const uint16_t new_pc = *((uint16_t*)(z80.opcode + 1));
    Z80_PC = new_pc;
    return Z80_STAGE_RESET;
}

///JP cc[y], nn; Size: 3; Flags: None
int z80_op_JP_cc_nn(){
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

///JP HL; Size: 1; Flags: ???
int z80_op_JP_HL(){
    assert(z80.opcode_index == 1);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///JR cc, e; Size: 2; Flags: None
int z80_op_JR_cc_d(){
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
int z80_op_JR_d(){
    assert(z80.opcode_index == 2);
    const int8_t pc_shift = *((int8_t*)&z80.opcode[1]); ///<-- @bug Endianness
    const int32_t next_pc = Z80_PC + pc_shift; //Signed relative jump
    Z80_PC = (next_pc & 0xFFFF);
    return Z80_STAGE_RESET;
}

///LD A, (BC); Size: 1; Flags: ???
int z80_op_LD_A_BCp(){
    assert(z80.opcode_index == 1);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///LD A,(DE); Size: 1; Flags: None
int z80_op_LD_A_DEp(){
    assert(z80.opcode_index == 1);
    if (z80.read_index == 0){
        z80.read_address = Z80_DE;
        return Z80_STAGE_M2;
    }
    else{
        Z80_A = z80.read_buffer[0];
        return Z80_STAGE_RESET;
    }
}

///LD A, I; Size: 2; Flags: ???
int z80_op_LD_A_I(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///LD A, (nn); Size: 3; Flags: None
int z80_op_LD_A_nnp(){
    assert(z80.opcode_index == 3);
    if (z80.read_index == 0){
        z80.read_address = *((uint16_t*)(z80.opcode + 1)); ///<-- @bug endianness!
        return Z80_STAGE_M2;
    }
    else{
        Z80_A = z80.read_buffer[0];
        return Z80_STAGE_RESET;
    }
}

///LD A, R; Size: 2; Flags: ???
int z80_op_LD_A_R(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///LD (BC), A; Size:1; Flags: None
int z80_op_LD_BCp_A(){
    assert(z80.opcode_index == 1);
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

///LD HL, (nn); Size: 3; Flags: None
int z80_op_LD_HL_nnp(){
    assert(z80.opcode_index == 3);
    if (z80.read_index == 0){
        z80.read_address = *((uint16_t*)(z80.opcode + 1)); ///<-- @bug endianness!
        return Z80_STAGE_M2;
    }
    else if (z80.read_index == 1){
        ++z80.read_address;
        return Z80_STAGE_M2;
    }
    else{
        Z80_H = z80.read_buffer[0];
        Z80_L = z80.read_buffer[1];
        return Z80_STAGE_RESET;
    }
}

///LD I, A; Size: 2; Flags: ???
int z80_op_LD_I_A(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///LD (DE), A; Size: 1; Flags: None
int z80_op_LD_DEp_A(){
    assert(z80.opcode_index == 1);
    if (z80.write_index == 0){
        z80.write_address = Z80_DE;
        z80.write_buffer[0] = Z80_A;
        return Z80_STAGE_M3;
    }
    else{
        return Z80_STAGE_RESET;
    }
}

///LD (nn), rp; Size:4; Flags: ??
int z80_op_LD_nnp_rp(uint8_t prefixed){
    assert(prefixed == 0); //<-- Unimplemented prefixed opcodes
    assert(z80.opcode_index == 4);
    Z80_OPCODE_SUBDIV;
    
    if (z80.write_index == 0){
        //First write
        *((uint16_t*)z80.write_buffer) = *((uint16_t*)(z80_rp[p[1]]));
        z80.write_address = (z80.opcode[2]) + (((uint16_t) z80.opcode[3]) << 8);
        return Z80_STAGE_M3;
    }
    else if (z80.write_index == 1){
        ++(z80.write_address);
        return Z80_STAGE_M3;
    }
    else{
        return Z80_STAGE_RESET;
    }
    
    return Z80_STAGE_RESET;
}

///LD R, A; Size: 2; Flags: ???
int z80_op_LD_R_A(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///LD r, n; Size: 2; Flags: None
int z80_op_LD_r_n(uint8_t prefixed){
    assert(prefixed == 0); //<-- Unimplemented prefixed opcodes
    assert(z80.opcode_index == 2);
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
int z80_op_LD_r_r(uint8_t prefixed){
    assert(prefixed == 0); //<-- Unimplemented prefixed opcodes
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    //If source is (HL), target is a regular reg.
    if (z80_r[z[0]] == 0){
        if (z80.read_index == 0){
            z80.read_address = Z80_HL;
            return Z80_STAGE_M2;
        }
        else{
            *(z80_r[y[0]]) = z80.read_buffer[0];
            return Z80_STAGE_RESET;
        }
    }
    //If source is a regular reg, target *can* be (HL)
    else{
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
}

///LD rp[n], nn; Size: 3; Flags: None
int z80_op_LD_rp_nn(uint8_t prefixed){
    assert(prefixed == 0); //<-- Unimplemented prefixed opcodes
    assert(z80.opcode_index == 3);
    //nn is stored in the opcode's bytes 1,2.
    Z80_OPCODE_SUBDIV;
    const uint16_t immediate = *((uint16_t*)(z80.opcode + 1)); ///<-- @bug Endianness!
    *z80_rp[p[0]] = immediate; ///<-- @bug Endianness!
    return Z80_STAGE_RESET;
}

///LD rp, (nn); Size: 4; Flags: ???
int z80_op_LD_rp_nnp(){
    assert(z80.opcode_index == 4);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///LD SP, HL; Size: 1; Flags: ???
int z80_op_LD_SP_HL(){
    assert(z80.opcode_index == 1);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///LD (nn), A; Size: 3; Flags: None
int z80_op_LD_nnp_A(){
    assert(z80.opcode_index == 3);
    //Perform the byte write
    if (z80.write_index == 0){
        z80.write_address = *((uint16_t*)(z80.opcode + 1)); ///<-- @bug endianness!
        z80.write_buffer[0] = Z80_A;
        return Z80_STAGE_M3;
    }
    else{
        return Z80_STAGE_RESET;
    }
}

///LD (nn), HL; size: 3; Flags: None
int z80_op_LD_nnp_HL(){
    assert(z80.opcode_index == 3);
    if (z80.write_index == 0){
        z80.write_address = *((uint16_t*)(z80.opcode + 1));
        z80.write_buffer[0] = Z80_H;
        z80.write_buffer[1] = Z80_L;
        return Z80_STAGE_M3;
    }
    else if (z80.write_index == 1){
        ++(z80.write_address);
        return Z80_STAGE_M3;
    }
    else{
        return Z80_STAGE_RESET;
    }
}

///LDD; Size: 2; Flags: ???
int z80_op_LDD(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///LDI; Size: 2; Flags: ???
int z80_op_LDI(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///LDDR; Size: 2; Flags: ???
int z80_op_LDDR(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///LDIR; Size: 2; Flags: H,P,N (cleared)
int z80_op_LDIR(){
    assert(z80.opcode_index == 2);
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

///NEG; Size: 2; Flags: ???
int z80_op_NEG(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///NOP; Size: 1; Flags: None
int z80_op_NOP(){
    assert(z80.opcode_index == 1);
    return Z80_STAGE_RESET;
}

///OR n; Size: 2; Flags:ALL
int z80_op_OR_n(){
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
int z80_op_OR_r(uint8_t prefixed){
    assert(prefixed == 0); //<-- Prefix unimplemented
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    //Request M2 read if required
    if ((z80_r[z[0]] == 0) && (z80.read_index == 0)){
        z80.read_address = Z80_HL;
        return Z80_STAGE_M2;
    }

    const uint8_t orig_a = Z80_A;
    if (z80_r[z[0]]){
        Z80_A = Z80_A | *(z80_r[z[0]]);
    }
    else{                                          /*OR (HL); Size: 1; Flags:ALL*/
        //Data retrieved from (HL)
        Z80_A = Z80_A | z80.read_buffer[0];
    }
    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_SIGN(Z80_A);
    Z80_F |= Z80_SETFLAG_ZERO(Z80_A);
    Z80_F |= Z80_SETFLAG_OVERFLOW(orig_a, Z80_A);
    return Z80_STAGE_RESET;
}

///OTDR; Size: 2; Flags: ???
int z80_op_OTDR(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///OTIR; Size: 2; Flags: Z,N
int z80_op_OTIR(){
    assert(z80.opcode_index == 2);
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
    assert(z80.opcode_index == 2);
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

///OUT (C), 0; Size: 2; Flags: ???
int z80_op_OUT_Cp_0(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///OUT (C), r; Size: 2; Flags: ???
int z80_op_OUT_Cp_r(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///OUTD; Size: 2; Flags: ???
int z80_op_OUTD(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///OUTI; Size: 2; Flags: ???
int z80_op_OUTI(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///POP rp2[p]; Size: 1; Flags: None
int z80_op_POP_rp2(uint8_t prefixed){
    assert(prefixed == 0); //<-- Unimplemented prefixed opcodes
    assert(z80.opcode_index == 1);
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
int z80_op_PUSH_rp2(uint8_t prefixed){
    assert(prefixed == 0); //<-- Unimplemented prefixed opcodes
    assert(z80.opcode_index == 1);
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

///RES y,r[z]; Size: 2; Flags: None
int z80_op_RES_y_r(){
    assert(z80.opcode_index == 2);
    Z80_OPCODE_SUBDIV;
    *(z80_r[z[1]]) = *(z80_r[z[1]]) & (~(1 << y[1]));
    return Z80_STAGE_RESET;
}

///RET; Size: 1; Flags: None
int z80_op_RET(){
    assert(z80.opcode_index == 1);
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
    assert(z80.opcode_index == 1);
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

///RETI; Size: 2; Flags: ???
int z80_op_RETI(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///RETN; Size: 2; Flags: ???
int z80_op_RETN(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///RLA; Size: 2; Flags: HC,A,C
int z80_op_RLA(){
    assert(z80.opcode_index == 2);
    const uint8_t next_carry = Z80_A & (1 << 7);
    Z80_A = (Z80_A << 1) | (Z80_F & Z80_FLAG_CARRY ? 1 : 0);
    Z80_F = (Z80_A & (Z80_CLRFLAG_HC & Z80_CLRFLAG_ADD & Z80_CLRFLAG_CARRY))
        | (next_carry ? Z80_FLAG_CARRY : 0);
    return Z80_STAGE_RESET;
}

///RLCA; Size: 1; Flags: H,N,C
int z80_op_RLCA(){
    assert(z80.opcode_index == 1);
    Z80_A = (Z80_A << 1) | (Z80_A & (1 << 7) ? 1 : 0);
    Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_ADD & Z80_CLRFLAG_CARRY))
        | ((Z80_A & (1)) ? Z80_FLAG_CARRY : 0);
    return Z80_STAGE_RESET;
}

///RLC r; Size: 2; Flags: ???
int z80_op_RLC_r(uint8_t prefixed){
    assert(prefixed == 0); //<-- Unimplemented prefixed opcodes
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///RLD; Size: 2; Flags: ???
int z80_op_RLD(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///RL r; Size: 2; Flags: ???
int z80_op_RL_r(uint8_t prefixed){
    assert(prefixed == 0); //<-- Unimplemented prefixed opcodes
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///RRA; Size: 1; Flags: H,N,C
int z80_op_RRA(){
    assert(z80.opcode_index == 1);
    const uint8_t next_carry = Z80_A & (1);
    Z80_A = (Z80_A >> 1) | (Z80_F & Z80_FLAG_CARRY ? (1 << 7) : 0);
    Z80_F = (Z80_A & (Z80_CLRFLAG_HC & Z80_CLRFLAG_ADD & Z80_CLRFLAG_CARRY))
        | (next_carry ? Z80_FLAG_CARRY : 0);
    return Z80_STAGE_RESET;
}

///RRCA; Size: 1; Flags: H,N,C
int z80_op_RRCA(){
    assert(z80.opcode_index == 1);
    Z80_A = (Z80_A >> 1) | (Z80_A & 1 ? (1 << 7) : 0);
    Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_ADD & Z80_CLRFLAG_CARRY))
        | ((Z80_A & (1 << 7)) ? Z80_FLAG_CARRY : 0);
    return Z80_STAGE_RESET;
}

///RRC r; Size: 2; Flags: ???
int z80_op_RRC_r(uint8_t prefixed){
    assert(prefixed == 0); //<-- Unimplemented prefixed opcodes
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///RRD; Size: 2; Flags: ???
int z80_op_RRD(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///RR r; Size: 2; Flags: ???
int z80_op_RR_r(uint8_t prefixed){
    assert(prefixed == 0); //<-- Unimplemented prefixed opcodes
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///RST y; Size: 1; Flags: ???
int z80_op_RST_y(){
    assert(z80.opcode_index == 1);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SBC HL,rp[p]; Size: 2; Flags: ?
int z80_op_SBC_HL_rp(){
    assert(z80.opcode_index == 2);
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

///SBC n; Size: 2; Flags: ???
int z80_op_SBC_n(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SBC r; Size: 1; Flags: ???
int z80_op_SBC_r(uint8_t prefixed){
    assert(prefixed == 0); //<-- Prefix unimplemented
    assert(z80.opcode_index == 1);
    assert(0);
    return Z80_STAGE_RESET;
}

///SET y, r; Size: 2; Flags: ???
int z80_op_SET_y_r(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SCF; Size: 1; Flags: C
int z80_op_SCF(){
    assert(z80.opcode_index == 1);
    Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_ADD)) | Z80_FLAG_CARRY;
    return Z80_STAGE_RESET;
}

///SLA r; Size: 2; Flags: ???
int z80_op_SLA_r(uint8_t prefixed){
    assert(prefixed == 0); //<-- Unimplemented prefixed opcodes
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SLL r; Size: 2; Flags: ???
int z80_op_SLL_r(uint8_t prefixed){
    assert(prefixed == 0); //<-- Unimplemented prefixed opcodes
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SRA r; Size: 2; Flags: ???
int z80_op_SRA_r(uint8_t prefixed){
    assert(prefixed == 0); //<-- Unimplemented prefixed opcodes
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SRL r[z]; Size: 2; Flags: ?
int z80_op_SRL_r(uint8_t prefixed){
    assert(prefixed == 0); //<-- Unimplemented prefixed opcodes
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

///SUB n; Size: 2; Flags: ???
int z80_op_SUB_n(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

///SUB r; Size: 1; Flags: ???
int z80_op_SUB_r(uint8_t prefixed){
    assert(prefixed == 0); //<-- Prefix unimplemented
    assert(z80.opcode_index == 1);
    assert(0);
    return Z80_STAGE_RESET;
}

///XOR n; Size: 2; Flags: ???
int z80_op_XOR_n(){
    assert(z80.opcode_index == 2);
    assert(0); ///<-- unimplemented
    return Z80_STAGE_RESET;
}

///XOR r[z]; Size: 1; Flags: All
int z80_op_XOR_r(uint8_t prefixed){
    assert(prefixed == 0); //<-- Prefix unimplemented
    assert(z80.opcode_index == 1);
    Z80_OPCODE_SUBDIV;
    //Request M2 read if required
    if ((z80_r[z[0]] == 0) && (z80.read_index == 0)){
        z80.read_address = Z80_HL;
        return Z80_STAGE_M2;
    }
    if (z80_r[z[0]]){
        Z80_A = Z80_A ^ *(z80_r[z[0]]);
    }
    else{                                         /*XOR (HL); Size: 1; Flags:ALL*/
        //Data retrieved from (HL)
        Z80_A = Z80_A ^ z80.read_buffer[0];
    }
    Z80_F = 0;
    Z80_F |= Z80_SETFLAG_SIGN(Z80_A);
    Z80_F |= Z80_SETFLAG_ZERO(Z80_A);
    Z80_F |= Z80_SETFLAG_PARITY(Z80_A);
    return Z80_STAGE_RESET;
}
