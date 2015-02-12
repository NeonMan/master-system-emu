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

#include "z80_decoder.h"
#include "z80_macros.h"
#include "z80_register_lut.h"
#include "z80_opcodes.h"
#include <assert.h>

/*
Most information taken from z80.info

Decoding Z80 instructions:
http://www.z80.info/decoding.htm
*/

//Forward declaration of decoder functions
int z80_instruction_decode();

///Removes prefix byte from opcode
void z80_instruction_unprefix(){
    z80.opcode[0] = z80.opcode[1];
    z80.opcode[1] = z80.opcode[2];
    z80.opcode[2] = z80.opcode[3];
    --z80.opcode_index;
}

///Decodes DDCB/FDCB prefix opcodes
///First two bytes already decoded (0xDDCB/0xFDCB)
int z80_instruction_decode_DDCB_FDCB(){

    //Relevant sub-byte divisions, for each of the 4 bytes (max) in an opcode.
    const uint8_t x[4] = { z80.opcode[0] >> 6, z80.opcode[1] >> 6, z80.opcode[2] >> 6, z80.opcode[3] >> 6 };
    const uint8_t y[4] = { (z80.opcode[0] >> 3) & 0x7, (z80.opcode[1] >> 3) & 0x7, (z80.opcode[2] >> 3) & 0x7, (z80.opcode[3] >> 3) & 0x7 };
    const uint8_t z[4] = { z80.opcode[0] & 0x7, z80.opcode[1] & 0x7, z80.opcode[2] & 0x7, z80.opcode[3] & 0x7 };
    const uint8_t p[4] = { (z80.opcode[0] >> 4) & 0x3, (z80.opcode[1] >> 4) & 0x3, (z80.opcode[2] >> 4) & 0x3, (z80.opcode[3] >> 4) & 0x3 };
    const uint8_t q[4] = { z80.opcode[0] & (1 << 3), z80.opcode[1] & (1 << 3), z80.opcode[2] & (1 << 3), z80.opcode[3] & (1 << 3) };

    //DDCB/FDCB always require an extra 2 bytes.
    if (z80.opcode_index == 3)
        return Z80_STAGE_M1;

    //Fourth byte
    assert(0);
    return Z80_STAGE_RESET;
}

///Decodes DD/FD prefix
int z80_instruction_decode_DD_FD(){

    //Relevant sub-byte divisions, for each of the 4 bytes (max) in an opcode.
    const uint8_t x[4] = { z80.opcode[0] >> 6, z80.opcode[1] >> 6, z80.opcode[2] >> 6, z80.opcode[3] >> 6 };
    const uint8_t y[4] = { (z80.opcode[0] >> 3) & 0x7, (z80.opcode[1] >> 3) & 0x7, (z80.opcode[2] >> 3) & 0x7, (z80.opcode[3] >> 3) & 0x7 };
    const uint8_t z[4] = { z80.opcode[0] & 0x7, z80.opcode[1] & 0x7, z80.opcode[2] & 0x7, z80.opcode[3] & 0x7 };
    const uint8_t p[4] = { (z80.opcode[0] >> 4) & 0x3, (z80.opcode[1] >> 4) & 0x3, (z80.opcode[2] >> 4) & 0x3, (z80.opcode[3] >> 4) & 0x3 };
    const uint8_t q[4] = { z80.opcode[0] & (1 << 3), z80.opcode[1] & (1 << 3), z80.opcode[2] & (1 << 3), z80.opcode[3] & (1 << 3) };

    /* From z80.info.
     *
     * "If the next byte is a DD, ED or FD prefix, the current DD prefix is 
     *  ignored (it's equivalent to a NONI) and processing continues with
     *  the next byte." <-- This part is done in main decoder function.
     *
     * "If the next byte is a CB prefix, the instruction will be decoded as
     *  stated in section 7, DDCB-prefixed opcodes." <-- DDCB_FDCB decoder.
     *
     * "If the next opcode makes use of HL, H, L, but not (HL), any occurrence
     *  of these will be replaced by IX, IXH, IXL respectively. An exception
     *  of this is EX DE, HL which is unaffected."
     *
     *  For this opcodes, overwritting the register LUT may work.
     *
     * "If the next opcode makes use of (HL), it will be replaced by (IX+d), 
     *  where d is a signed 8-bit displacement immediately following the
     *  opcode (any immediate data, i.e. n, will follow the displacement byte),
     *  and any other instances of H and L will be unaffected. Therefore, an
     *  instruction like LD IXH, (IX+d) does not exist, but LD H, (IX+d) does."
     *
     *  This instructions must be decoded here.
     */

    switch (z80.opcode_index){
    case 1:
        return Z80_STAGE_M1;
    case 2:
        //---Check xxCB prefix---
        if (z80.opcode[1] == 0xCB) //DDCB/FDCB prefix, read one more byte
            return Z80_STAGE_M1;
        switch (z80.opcode[1] & (Z80_OPCODE_X_MASK | Z80_OPCODE_Z_MASK)){
        case Z80_OPCODE_XZ(0, 0): //
        case Z80_OPCODE_XZ(0, 7): //
        case Z80_OPCODE_XZ(3, 0): //
        case Z80_OPCODE_XZ(3, 2): //
        case Z80_OPCODE_XZ(3, 4): //
        case Z80_OPCODE_XZ(3, 7): // Unaffected opcodes
            z80_instruction_unprefix();      //
            return z80_instruction_decode(); //<-- Remove prefix, execute as a normal opcode

        case Z80_OPCODE_XZ(0, 1):
            if (!q[1])                                  /*LD rp[p], nn; Size: 4; Flags: None*/
                return Z80_STAGE_M1; //+2 bytes
            else{                                       /*ADD HL, rp[p]; Size:2; Flags: None*/
                assert(0);
                return Z80_STAGE_RESET;
            }

        case Z80_OPCODE_XZ(0, 2):
            if (p[1] == 2){
                if (!q[1]){                              /*LD (nn), HL; Size: 4; Flags: None*/
                    return Z80_STAGE_M1; //+2 bytes
                }
                else{                                    /*LD HL, (nn); Size: 4; Flags: None*/
                    return Z80_STAGE_M1; //+2 bytes
                }
            }
            else{ /*Unprefix and reevaluate*/
                z80_instruction_unprefix();
                return z80_instruction_decode();
            }

        case Z80_OPCODE_XZ(0, 3):
            if (!q[1]){                                     /*INC rp[p]; Size: 2; Flags: All*/
                assert(0);
                return Z80_STAGE_RESET;
            }
            else{                                           /*DEC rp[p]; Size: 2; Flags: All*/
                assert(0);
                return Z80_STAGE_RESET;
            }

        case Z80_OPCODE_XZ(0, 4):                            /*INC r[y]; Size: 2; Flags: All*/
            assert(0);
            return Z80_STAGE_RESET;

        case Z80_OPCODE_XZ(0, 5):                            /*DEC r[y]; Size: 2; Flags: All*/
            assert(0);
            return Z80_STAGE_RESET;

        case Z80_OPCODE_XZ(0, 6):                            /*LD r[y]; Size: 3; Flags: None*/
            return Z80_STAGE_M1;

        case Z80_OPCODE_XZ(1, 0):                      /**/
        case Z80_OPCODE_XZ(1, 1):                      /**/
        case Z80_OPCODE_XZ(1, 2):                      /**/
        case Z80_OPCODE_XZ(1, 3):                      /**/
        case Z80_OPCODE_XZ(1, 4):                      /**/
        case Z80_OPCODE_XZ(1, 5):                      /**/
        case Z80_OPCODE_XZ(1, 6):                      /**/
        case Z80_OPCODE_XZ(1, 7):                      /*LD r[y], r[z]; Size: 2; Flags: None*/
            if ((y[1] == 6) && (z[1] == 6)){ /*HALT*/
                z80_instruction_unprefix();
                return z80_instruction_decode();
            }
            else{
                assert(0);
                return Z80_STAGE_RESET;
            }

        case Z80_OPCODE_XZ(2, 0):
        case Z80_OPCODE_XZ(2, 1):
        case Z80_OPCODE_XZ(2, 2):
        case Z80_OPCODE_XZ(2, 3):
        case Z80_OPCODE_XZ(2, 4):
        case Z80_OPCODE_XZ(2, 5):
        case Z80_OPCODE_XZ(2, 6):
        case Z80_OPCODE_XZ(2, 7):
            /*4,5,6 use H,L,(HL)*/
            if ((z[1] >= 4) && (z[1] <= 6)){                      /*alu; Size: 2; Flags: All*/
                assert(0);
                return Z80_STAGE_RESET;
            }
            else{
                z80_instruction_unprefix();
                return z80_instruction_decode();
            }

        case Z80_OPCODE_XZ(3, 1):
            if (!q[1]){                                   /*POP rp2[p]; Size: 2; Flags: None*/
                //Otherwise, perform the operation as usual
                if (z80.read_index == 0){
                    z80.read_address = Z80_SP;
                    return Z80_STAGE_M2;
                }
                else if (z80.read_index == 1){
                    ++(z80.read_address);
                    return Z80_STAGE_M2;
                }
                else{
                    Z80_SP += 2;
                    if (z80.opcode[0] == 0xDD)
                        *z80_rp2_ix[p[1]] = *((uint16_t*)(z80.read_buffer)); ///<-- @bug Endianness
                    else
                        *z80_rp2_iy[p[1]] = *((uint16_t*)(z80.read_buffer)); ///<-- @bug Endianness
                    return Z80_STAGE_RESET;
                }
            }
            else{
                if (p[1] >= 2){                     /*JP HL; LD SP, HL; Size: 2; Flags: None*/
                    assert(0);
                    return Z80_STAGE_RESET;
                }
                else{
                    z80_instruction_unprefix();
                    return z80_instruction_decode();
                }
            }

        case Z80_OPCODE_XZ(3, 3):
            if (y[1] == 4){                              /*EX (SP), HL; Size: 2; Flags: None*/
                assert(0);
                return Z80_STAGE_RESET;
            }
            else if (y[1] == 5){                           /*EX DE, HL; Size: 2; Flags: None*/
                assert(0);
                return Z80_STAGE_RESET;
            }
            else{
                z80_instruction_unprefix();
                return z80_instruction_decode();
            }

        case Z80_OPCODE_XZ(3, 5):
            if (!q[1]){
                if (p[1] == 2){                          /*PUSH aX; Size: 2; Flags: None*/
                    if (z80.write_index == 0){
                        z80.write_address = Z80_SP - 2;
                        *((uint16_t*)z80.write_buffer) = Z80_INDIRECT(z80.opcode[0]); ///<-- @bug Endianness
                        return Z80_STAGE_M3;
                    }
                    else if (z80.write_index == 1){
                        ++ z80.write_address;
                        return Z80_STAGE_M3;
                    }
                    else{
                        Z80_SP -= 2;
                        return Z80_STAGE_RESET;
                    }
                }
                else{
                    z80_instruction_unprefix();
                    return z80_instruction_decode();
                }
            }
            else{                                            /*CALL nn; Size: 4; Flags: None*/
                return Z80_STAGE_M1; //+2 bytes
            }

        case Z80_OPCODE_XZ(3, 6):                     /*alu + immediate; Size: 3; Flags: All*/
            return Z80_STAGE_M1; //+1 bytes
        }
        assert(0); /*unimplemented*/
        return Z80_STAGE_RESET;

    case 3:
        //---Check xxCB prefix---
        if (z80.opcode[1] == 0xCB) //DDCB/FDCB prefix, call its own decoder.
            return z80_instruction_decode_DDCB_FDCB();

        switch (z80.opcode[1] & (Z80_OPCODE_X_MASK | Z80_OPCODE_Z_MASK)){
        case Z80_OPCODE_XZ(0, 1):
            if (!q[1]){                                  /*LD rp[p], nn; Size: 4; Flags: None*/
                return Z80_STAGE_M1; //+1 bytes
            }
            else{
                assert(0);
                return Z80_STAGE_RESET;
            }

        case Z80_OPCODE_XZ(0, 2):
            if (p[1] == 2){
                if (!q[1]){                              /*LD (nn), HL; Size: 4; Flags: None*/
                    return Z80_STAGE_M1; //+1 bytes
                }
                else{                                    /*LD HL, (nn); Size: 4; Flags: None*/
                    return Z80_STAGE_M1; //+1 bytes
                }
            }
            else{ 
                assert(0);
                return Z80_STAGE_RESET;
            }

        case Z80_OPCODE_XZ(0, 6):                            /*LD r[y]; Size: 3; Flags: None*/
            assert(0);
            return Z80_STAGE_RESET;

        case Z80_OPCODE_XZ(3, 5):
            if (q[1]){                                       /*CALL nn; Size: 4; Flags: None*/
                return Z80_STAGE_M1; //+1 bytes
            }
            assert(0);
            return Z80_STAGE_RESET;

        case Z80_OPCODE_XZ(3, 6):                     /*alu + immediate; Size: 3; Flags: All*/
            assert(0);
            return Z80_STAGE_RESET;
        }
        assert(0); /*unimplemented*/
        return Z80_STAGE_RESET;

    case 4:
        switch (z80.opcode[1] & (Z80_OPCODE_X_MASK | Z80_OPCODE_Z_MASK)){
        case Z80_OPCODE_XZ(0, 1):
            if (!q[1]){                                  /*LD rp[p], nn; Size: 4; Flags: None*/
                assert(0);
                return Z80_STAGE_RESET;
            }
            else{
                assert(0);
                return Z80_STAGE_RESET;
            }

        case Z80_OPCODE_XZ(0, 2):
            if (p[1] == 2){
                if (!q[1]){                              /*LD (nn), HL; Size: 4; Flags: None*/
                    assert(0);
                    return Z80_STAGE_RESET;
                }
                else{                                    /*LD HL, (nn); Size: 4; Flags: None*/
                    assert(0);
                    return Z80_STAGE_RESET;
                }
            }
            else{
                assert(0);
                return Z80_STAGE_RESET;
            }
        default:
            assert(0); /*unimplemented*/
            return Z80_STAGE_RESET;
        }

    default:
        assert(0); /*unimplemented*/
        return Z80_STAGE_RESET;
    }
}

///Decodes the CB-prefixed opcodes.
int z80_instruction_decode_CB(){

    //Relevant sub-byte divisions, for each of the 4 bytes (max) in an opcode.
    const uint8_t x[4] = { z80.opcode[0] >> 6, z80.opcode[1] >> 6, z80.opcode[2] >> 6, z80.opcode[3] >> 6 };
    const uint8_t y[4] = { (z80.opcode[0] >> 3) & 0x7, (z80.opcode[1] >> 3) & 0x7, (z80.opcode[2] >> 3) & 0x7, (z80.opcode[3] >> 3) & 0x7 };
    const uint8_t z[4] = { z80.opcode[0] & 0x7, z80.opcode[1] & 0x7, z80.opcode[2] & 0x7, z80.opcode[3] & 0x7 };
    const uint8_t p[4] = { (z80.opcode[0] >> 4) & 0x3, (z80.opcode[1] >> 4) & 0x3, (z80.opcode[2] >> 4) & 0x3, (z80.opcode[3] >> 4) & 0x3 };
    const uint8_t q[4] = { z80.opcode[0] & (1 << 3), z80.opcode[1] & (1 << 3), z80.opcode[2] & (1 << 3), z80.opcode[3] & (1 << 3) };

    switch (z80.opcode_index){
    case 1:
        return Z80_STAGE_M1;
    //Second opcode byte
    case 2:
        switch (x[1]){
        case 0:                                     /*rotation[y] r[z]; Size: 2; Flags: ?*/
            switch (z80.opcode[1] & (Z80_OPCODE_Y_MASK |Z80_OPCODE_Z_MASK)){
            case Z80_OPCODE_YZ(0, 0): //
            case Z80_OPCODE_YZ(0, 1): //
            case Z80_OPCODE_YZ(0, 2): //
            case Z80_OPCODE_YZ(0, 3): //
            case Z80_OPCODE_YZ(0, 4): //
            case Z80_OPCODE_YZ(0, 5): //
            case Z80_OPCODE_YZ(0, 6): //
            case Z80_OPCODE_YZ(0, 7): // RLC r[z]
                assert(0); //Unimplemented
                return Z80_STAGE_RESET;

            case Z80_OPCODE_YZ(1, 0): //
            case Z80_OPCODE_YZ(1, 1): //
            case Z80_OPCODE_YZ(1, 2): //
            case Z80_OPCODE_YZ(1, 3): //
            case Z80_OPCODE_YZ(1, 4): //
            case Z80_OPCODE_YZ(1, 5): //
            case Z80_OPCODE_YZ(1, 6): //
            case Z80_OPCODE_YZ(1, 7): // RRC r[z]
                assert(0); //Unimplemented
                return Z80_STAGE_RESET;

            case Z80_OPCODE_YZ(2, 0): //
            case Z80_OPCODE_YZ(2, 1): //
            case Z80_OPCODE_YZ(2, 2): //
            case Z80_OPCODE_YZ(2, 3): //
            case Z80_OPCODE_YZ(2, 4): //
            case Z80_OPCODE_YZ(2, 5): //
            case Z80_OPCODE_YZ(2, 6): //
            case Z80_OPCODE_YZ(2, 7): // RL r[z]
                assert(0); //Unimplemented
                return Z80_STAGE_RESET;

            case Z80_OPCODE_YZ(3, 0): //
            case Z80_OPCODE_YZ(3, 1): //
            case Z80_OPCODE_YZ(3, 2): //
            case Z80_OPCODE_YZ(3, 3): //
            case Z80_OPCODE_YZ(3, 4): //
            case Z80_OPCODE_YZ(3, 5): //
            case Z80_OPCODE_YZ(3, 6): //
            case Z80_OPCODE_YZ(3, 7): // RR r[z]
                assert(0); //Unimplemented
                return Z80_STAGE_RESET;

            case Z80_OPCODE_YZ(4, 0): //
            case Z80_OPCODE_YZ(4, 1): //
            case Z80_OPCODE_YZ(4, 2): //
            case Z80_OPCODE_YZ(4, 3): //
            case Z80_OPCODE_YZ(4, 4): //
            case Z80_OPCODE_YZ(4, 5): //
            case Z80_OPCODE_YZ(4, 6): //
            case Z80_OPCODE_YZ(4, 7): // SLA r[z]
                assert(0); //Unimplemented
                return Z80_STAGE_RESET;

            case Z80_OPCODE_YZ(5, 0): //
            case Z80_OPCODE_YZ(5, 1): //
            case Z80_OPCODE_YZ(5, 2): //
            case Z80_OPCODE_YZ(5, 3): //
            case Z80_OPCODE_YZ(5, 4): //
            case Z80_OPCODE_YZ(5, 5): //
            case Z80_OPCODE_YZ(5, 6): //
            case Z80_OPCODE_YZ(5, 7): // SRA r[z]
                assert(0); //Unimplemented
                return Z80_STAGE_RESET;

            case Z80_OPCODE_YZ(6, 0): //
            case Z80_OPCODE_YZ(6, 1): //
            case Z80_OPCODE_YZ(6, 2): //
            case Z80_OPCODE_YZ(6, 3): //
            case Z80_OPCODE_YZ(6, 4): //
            case Z80_OPCODE_YZ(6, 5): //
            case Z80_OPCODE_YZ(6, 6): //
            case Z80_OPCODE_YZ(6, 7): // SLL r[z]
                assert(0); //Unimplemented
                return Z80_STAGE_RESET;

            case Z80_OPCODE_YZ(7, 0): //
            case Z80_OPCODE_YZ(7, 1): //
            case Z80_OPCODE_YZ(7, 2): //
            case Z80_OPCODE_YZ(7, 3): //
            case Z80_OPCODE_YZ(7, 4): //
            case Z80_OPCODE_YZ(7, 5): //
            case Z80_OPCODE_YZ(7, 6): //
            case Z80_OPCODE_YZ(7, 7): // SRL r[z]
                return z80_op_SRL_r();

            default:
                assert(0); //Should never get here
            }
        case 1:                                /*BIT y,r[z]; Size: 2; Flags: _S,Z,H,_P,N*/
            return z80_op_BIT_y_r();
        case 2:                                     /*RES y,r[z]; Size: 2; Flags: None*/
            return z80_op_RES_y_r();
        case 3:                                     /*SET y,r[z]; Size: 2; Flags: None*/
            assert(0); /*Unimplemented*/
            return Z80_STAGE_RESET;
        }

    //No more opcode bytes
    default:
        assert(0);
        return Z80_STAGE_RESET;
    }
}


///Decodes ED prefixed opcodes
int z80_instruction_decode_ED(){
    assert(0); ///<-- Unimplemented
    return Z80_STAGE_RESET;
}

/**
* @brief decode/execute a Z80 opcode.
*
* Decode a z80 opcode, if the opcode is completly read, execute it.
* If a memory read/write is needed, signal it by returning the apropriate
* information.
*
* @return Signal wether we need a read/write/fetch or none.
*/
int z80_instruction_decode(){

    //Relevant sub-byte divisions, for each of the 4 bytes (max) in an opcode.
    const uint8_t x[4] = { z80.opcode[0] >> 6, z80.opcode[1] >> 6, z80.opcode[2] >> 6, z80.opcode[3] >> 6 };
    const uint8_t y[4] = { (z80.opcode[0] >> 3) & 0x7, (z80.opcode[1] >> 3) & 0x7, (z80.opcode[2] >> 3) & 0x7, (z80.opcode[3] >> 3) & 0x7 };
    const uint8_t z[4] = { z80.opcode[0] & 0x7, z80.opcode[1] & 0x7, z80.opcode[2] & 0x7, z80.opcode[3] & 0x7 };
    const uint8_t p[4] = { (z80.opcode[0] >> 4) & 0x3, (z80.opcode[1] >> 4) & 0x3, (z80.opcode[2] >> 4) & 0x3, (z80.opcode[3] >> 4) & 0x3 };
    const uint8_t q[4] = { z80.opcode[0] & (1 << 3), z80.opcode[1] & (1 << 3), z80.opcode[2] & (1 << 3), z80.opcode[3] & (1 << 3) };

    //Select by X/Z at the same time
    switch (z80.opcode_index){

    case 1: //First opcode byte
        //Select by first byte
        //All cases are sorted in ascending order.
        //To ease debugging, XYZ and XPQZ macros are used
        switch (z80.opcode[0]){
        //X==0
        case Z80_OPCODE_XYZ(0, 0, 0): return z80_op_NOP();                   /*NOP (Size: 1)*/
        case Z80_OPCODE_XYZ(0, 0, 1): return Z80_STAGE_M1;             /*LD rp, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(0, 0, 0, 2): return z80_op_LD_BCp_A();   /*LD (BC), A (size: 1)*/
        case Z80_OPCODE_XYZ(0, 0, 3): return z80_op_INC_rp();             /*INC rp (size: 1)*/
        case Z80_OPCODE_XYZ(0, 0, 4): return z80_op_INC_r();               /*INC r (Size: 1)*/
        case Z80_OPCODE_XYZ(0, 0, 5): return z80_op_DEC_r();               /*DEC r (size: 1)*/
        case Z80_OPCODE_XYZ(0, 0, 6): return Z80_STAGE_M1;               /*LD r, n (size: 1)*/
        case Z80_OPCODE_XYZ(0, 0, 7): return z80_op_RLCA();                 /*RLCA (size: 1)*/

        case Z80_OPCODE_XYZ(0, 1, 0): return z80_op_EX();             /*EX AF, AF' (Size: 1)*/
        case Z80_OPCODE_XYZ(0, 1, 1): return z80_op_ADD_HL_rp();      /*ADD HL, rp (size: 1)*/
        case Z80_OPCODE_XPQZ(0, 0, 1, 2): return z80_op_LD_A_BCp();   /*LD A, (BC) (size: 1)*/
        case Z80_OPCODE_XYZ(0, 1, 3): return z80_op_DEC_rp();             /*DEC rp (size: 1)*/
        case Z80_OPCODE_XYZ(0, 1, 4): return z80_op_INC_r();               /*INC r (Size: 1)*/
        case Z80_OPCODE_XYZ(0, 1, 5): return z80_op_DEC_r();               /*DEC r (size: 1)*/
        case Z80_OPCODE_XYZ(0, 1, 6): return Z80_STAGE_M1;               /*LD r, n (size: 1)*/
        case Z80_OPCODE_XYZ(0, 1, 7): return z80_op_RRCA();                 /*RRCA (size: 1)*/

        case Z80_OPCODE_XYZ(0, 2, 0): return Z80_STAGE_M1;                /*DJNZ d (size: 2)*/
        case Z80_OPCODE_XYZ(0, 2, 1): return Z80_STAGE_M1;             /*LD rp, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(0, 1, 0, 2): return z80_op_LD_DEp_A();   /*LD (DE), A (size: 1)*/
        case Z80_OPCODE_XYZ(0, 2, 3): return z80_op_INC_rp();             /*INC rp (size: 1)*/
        case Z80_OPCODE_XYZ(0, 2, 4): return z80_op_INC_r();               /*INC r (Size: 1)*/
        case Z80_OPCODE_XYZ(0, 2, 5): return z80_op_DEC_r();               /*DEC r (size: 1)*/
        case Z80_OPCODE_XYZ(0, 2, 6): return Z80_STAGE_M1;               /*LD r, n (size: 1)*/
        case Z80_OPCODE_XYZ(0, 2, 7): return z80_op_RLA();                   /*RLA (size: 1)*/

        case Z80_OPCODE_XYZ(0, 3, 0): return Z80_STAGE_M1;                  /*JR d (size: 2)*/
        case Z80_OPCODE_XYZ(0, 3, 1): return z80_op_ADD_HL_rp();      /*ADD HL, rp (size: 1)*/
        case Z80_OPCODE_XPQZ(0, 1, 1, 2): return z80_op_LD_A_DEp();    /*LD A,(DE) (size: 1)*/
        case Z80_OPCODE_XYZ(0, 3, 3): return z80_op_DEC_rp();             /*DEC rp (size: 1)*/
        case Z80_OPCODE_XYZ(0, 3, 4): return z80_op_INC_r();               /*INC r (Size: 1)*/
        case Z80_OPCODE_XYZ(0, 3, 5): return z80_op_DEC_r();               /*DEC r (size: 1)*/
        case Z80_OPCODE_XYZ(0, 3, 6): return Z80_STAGE_M1;               /*LD r, n (size: 1)*/
        case Z80_OPCODE_XYZ(0, 3, 7): return z80_op_RRA();                   /*RRA (size: 1)*/

        case Z80_OPCODE_XYZ(0, 4, 0): return Z80_STAGE_M1;               /*JR cc, d (size 2)*/
        case Z80_OPCODE_XYZ(0, 4, 1): return Z80_STAGE_M1;             /*LD rp, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(0, 2, 0, 2): return Z80_STAGE_M1;       /*LD (nn), HL (size: 3)*/
        case Z80_OPCODE_XYZ(0, 4, 3): return z80_op_INC_rp();             /*INC rp (size: 1)*/
        case Z80_OPCODE_XYZ(0, 4, 4): return z80_op_INC_r();               /*INC r (Size: 1)*/
        case Z80_OPCODE_XYZ(0, 4, 5): return z80_op_DEC_r();               /*DEC r (size: 1)*/
        case Z80_OPCODE_XYZ(0, 4, 6): return Z80_STAGE_M1;               /*LD r, n (size: 1)*/
        case Z80_OPCODE_XYZ(0, 4, 7): return z80_op_DAA();                   /*DAA (size: 1)*/

        case Z80_OPCODE_XYZ(0, 5, 0): return Z80_STAGE_M1;               /*JR cc, d (size 2)*/
        case Z80_OPCODE_XYZ(0, 5, 1): return z80_op_ADD_HL_rp();      /*ADD HL, rp (size: 1)*/
        case Z80_OPCODE_XPQZ(0, 2, 1, 2): return Z80_STAGE_M1;       /*LD HL, (nn) (size: 3)*/
        case Z80_OPCODE_XYZ(0, 5, 3): return z80_op_DEC_rp();             /*DEC rp (size: 1)*/
        case Z80_OPCODE_XYZ(0, 5, 4): return z80_op_INC_r();               /*INC r (Size: 1)*/
        case Z80_OPCODE_XYZ(0, 5, 5): return z80_op_DEC_r();               /*DEC r (size: 1)*/
        case Z80_OPCODE_XYZ(0, 5, 6): return Z80_STAGE_M1;               /*LD r, n (size: 1)*/
        case Z80_OPCODE_XYZ(0, 5, 7): return z80_op_CPL();                   /*CPL (size: 1)*/

        case Z80_OPCODE_XYZ(0, 6, 0): return Z80_STAGE_M1;               /*JR cc, d (size 2)*/
        case Z80_OPCODE_XYZ(0, 6, 1): return Z80_STAGE_M1;             /*LD rp, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(0, 3, 0, 2): return Z80_STAGE_M1;        /*LD (nn), A (size: 3)*/
        case Z80_OPCODE_XYZ(0, 6, 3): return z80_op_INC_rp();             /*INC rp (size: 1)*/
        case Z80_OPCODE_XYZ(0, 6, 4): return z80_op_INC_r();               /*INC r (Size: 1)*/
        case Z80_OPCODE_XYZ(0, 6, 5): return z80_op_DEC_r();               /*DEC r (size: 1)*/
        case Z80_OPCODE_XYZ(0, 6, 6): return Z80_STAGE_M1;               /*LD r, n (size: 1)*/
        case Z80_OPCODE_XYZ(0, 6, 7): return z80_op_SCF();                   /*SCF (size: 1)*/

        case Z80_OPCODE_XYZ(0, 7, 0): return Z80_STAGE_M1;               /*JR cc, d (size 2)*/
        case Z80_OPCODE_XYZ(0, 7, 1): return z80_op_ADD_HL_rp();      /*ADD HL, rp (size: 1)*/
        case Z80_OPCODE_XPQZ(0, 3, 1, 2): return Z80_STAGE_M1;        /*LD A, (nn) (size: 3)*/
        case Z80_OPCODE_XYZ(0, 7, 3): return z80_op_DEC_rp();             /*DEC rp (size: 1)*/
        case Z80_OPCODE_XYZ(0, 7, 4): return z80_op_INC_r();               /*INC r (Size: 1)*/
        case Z80_OPCODE_XYZ(0, 7, 5): return z80_op_DEC_r();               /*DEC r (size: 1)*/
        case Z80_OPCODE_XYZ(0, 7, 6): return Z80_STAGE_M1;               /*LD r, n (size: 1)*/
        case Z80_OPCODE_XYZ(0, 7, 7): return z80_op_CCF();                   /*CCF (size: 1)*/

        //X==1
        case Z80_OPCODE_XYZ(1, 0, 0): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 0, 1): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 0, 2): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 0, 3): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 0, 4): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 0, 5): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 0, 6): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 0, 7): /*Fall-through*/

        case Z80_OPCODE_XYZ(1, 1, 0): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 1, 1): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 1, 2): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 1, 3): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 1, 4): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 1, 5): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 1, 6): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 1, 7): /*Fall-through*/

        case Z80_OPCODE_XYZ(1, 2, 0): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 2, 1): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 2, 2): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 2, 3): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 2, 4): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 2, 5): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 2, 6): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 2, 7): /*Fall-through*/

        case Z80_OPCODE_XYZ(1, 3, 0): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 3, 1): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 3, 2): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 3, 3): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 3, 4): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 3, 5): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 3, 6): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 3, 7): /*Fall-through*/

        case Z80_OPCODE_XYZ(1, 4, 0): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 4, 1): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 4, 2): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 4, 3): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 4, 4): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 4, 5): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 4, 6): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 4, 7): /*Fall-through*/

        case Z80_OPCODE_XYZ(1, 5, 0): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 5, 1): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 5, 2): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 5, 3): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 5, 4): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 5, 5): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 5, 6): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 5, 7): /*Fall-through*/

        case Z80_OPCODE_XYZ(1, 6, 0): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 6, 1): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 6, 2): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 6, 3): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 6, 4): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 6, 5): return z80_op_LD_r_r();            /*LD r, r (size: 1)*/
        case Z80_OPCODE_XYZ(1, 6, 6): return z80_op_HALT();                 /*HALT (size: 1)*/ /*Replaces LD (HL), (HL)*/
        case Z80_OPCODE_XYZ(1, 6, 7): /*Fall-through*/

        case Z80_OPCODE_XYZ(1, 7, 0): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 7, 1): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 7, 2): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 7, 3): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 7, 4): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 7, 5): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 7, 6): /*Fall-through*/
        case Z80_OPCODE_XYZ(1, 7, 7): return z80_op_LD_r_r();            /*LD r, r (size: 1)*/

        //X==2
        case Z80_OPCODE_XYZ(2, 0, 0): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 0, 1): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 0, 2): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 0, 3): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 0, 4): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 0, 5): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 0, 6): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 0, 7): return z80_op_ADD_r();               /*ADD r (size: 1)*/

        case Z80_OPCODE_XYZ(2, 1, 0): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 1, 1): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 1, 2): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 1, 3): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 1, 4): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 1, 5): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 1, 6): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 1, 7): return z80_op_ADC_r();               /*ADC r (size: 1)*/

        case Z80_OPCODE_XYZ(2, 2, 0): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 2, 1): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 2, 2): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 2, 3): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 2, 4): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 2, 5): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 2, 6): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 2, 7): return z80_op_SUB_r();               /*SUB r (size: 1)*/

        case Z80_OPCODE_XYZ(2, 3, 0): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 3, 1): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 3, 2): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 3, 3): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 3, 4): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 3, 5): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 3, 6): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 3, 7): return z80_op_SBC_r();               /*SBC r (size: 1)*/

        case Z80_OPCODE_XYZ(2, 4, 0): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 4, 1): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 4, 2): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 4, 3): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 4, 4): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 4, 5): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 4, 6): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 4, 7): return z80_op_AND_r();               /*AND r (size: 1)*/

        case Z80_OPCODE_XYZ(2, 5, 0): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 5, 1): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 5, 2): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 5, 3): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 5, 4): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 5, 5): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 5, 6): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 5, 7): return z80_op_XOR_r();               /*XOR r (size: 1)*/

        case Z80_OPCODE_XYZ(2, 6, 0): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 6, 1): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 6, 2): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 6, 3): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 6, 4): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 6, 5): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 6, 6): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 6, 7): return z80_op_OR_r();                 /*OR r (size: 1)*/

        case Z80_OPCODE_XYZ(2, 7, 0): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 7, 1): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 7, 2): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 7, 3): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 7, 4): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 7, 5): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 7, 6): /*fall-through*/
        case Z80_OPCODE_XYZ(2, 7, 7): return z80_op_CP_r();                 /*CP r (size: 1)*/

        //X==3
        case Z80_OPCODE_XYZ(3, 0, 0): return z80_op_RET_cc();             /*RET cc (size: 1)*/
        case Z80_OPCODE_XPQZ(3, 0, 0, 1): return z80_op_POP_rp2();       /*POP rp2 (size: 1)*/
        case Z80_OPCODE_XYZ(3, 0, 2): return Z80_STAGE_M1;             /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 0, 3): return Z80_STAGE_M1;                 /*JP nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 0, 4): return Z80_STAGE_M1;           /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 0, 0, 5): return z80_op_PUSH_rp2();     /*PUSH rp2 (size: 1)*/
        case Z80_OPCODE_XPQZ(3, 0, 0, 6): return Z80_STAGE_M1;             /*ADD n (size: 2)*/
        case Z80_OPCODE_XYZ(3, 0, 7): return z80_op_RST_y();               /*RST y (size: 1)*/

        case Z80_OPCODE_XYZ(3, 1, 0): return z80_op_RET_cc();             /*RET cc (size: 1)*/
        case Z80_OPCODE_XPQZ(3, 0, 1, 1): return z80_op_RET();               /*RET (size: 1)*/
        case Z80_OPCODE_XYZ(3, 1, 2): return Z80_STAGE_M1;             /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 1, 3): return z80_instruction_decode_CB();      /*0xCB Prefix*/
        case Z80_OPCODE_XYZ(3, 1, 4): return Z80_STAGE_M1;           /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 0, 1, 5): return Z80_STAGE_M1;           /*CALL nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 0, 1, 6): return Z80_STAGE_M1;             /*ADC n (size: 2)*/
        case Z80_OPCODE_XYZ(3, 1, 7): return z80_op_RST_y();               /*RST y (size: 1)*/

        case Z80_OPCODE_XYZ(3, 2, 0): return z80_op_RET_cc();             /*RET cc (size: 1)*/
        case Z80_OPCODE_XPQZ(3, 1, 0, 1): return z80_op_POP_rp2();       /*POP rp2 (size: 1)*/
        case Z80_OPCODE_XYZ(3, 2, 2): return Z80_STAGE_M1;             /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 2, 3): return Z80_STAGE_M1;            /*OUT (n), A (size: 2)*/
        case Z80_OPCODE_XYZ(3, 2, 4): return Z80_STAGE_M1;           /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 1, 0, 5): return z80_op_PUSH_rp2();     /*PUSH rp2 (size: 1)*/
        case Z80_OPCODE_XPQZ(3, 1, 0, 6): return Z80_STAGE_M1;             /*SUB n (size: 2)*/
        case Z80_OPCODE_XYZ(3, 2, 7): return z80_op_RST_y();               /*RST y (size: 1)*/

        case Z80_OPCODE_XYZ(3, 3, 0): return z80_op_RET_cc();             /*RET cc (size: 1)*/
        case Z80_OPCODE_XPQZ(3, 1, 1, 1): return z80_op_EXX();               /*EXX (size: 1)*/
        case Z80_OPCODE_XYZ(3, 3, 2): return Z80_STAGE_M1;             /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 3, 3): return Z80_STAGE_M1;             /*IN A, (n) (size: 2)*/
        case Z80_OPCODE_XYZ(3, 3, 4): return Z80_STAGE_M1;           /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 1, 1, 5): return z80_instruction_decode_DD_FD(); /*DD prefix*/
        case Z80_OPCODE_XPQZ(3, 1, 1, 6): return Z80_STAGE_M1;             /*SBC n (size: 2)*/
        case Z80_OPCODE_XYZ(3, 3, 7): return z80_op_RST_y();               /*RST y (size: 1)*/

        case Z80_OPCODE_XYZ(3, 4, 0): return z80_op_RET_cc();             /*RET cc (size: 1)*/
        case Z80_OPCODE_XPQZ(3, 2, 0, 1): return z80_op_POP_rp2();       /*POP rp2 (size: 1)*/
        case Z80_OPCODE_XYZ(3, 4, 2): return Z80_STAGE_M1;             /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 4, 3): return z80_op_EX_SPp_HL();     /*EX (SP), HL (size: 1)*/
        case Z80_OPCODE_XYZ(3, 4, 4): return Z80_STAGE_M1;           /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 2, 0, 5): return z80_op_PUSH_rp2();     /*PUSH rp2 (size: 1)*/
        case Z80_OPCODE_XPQZ(3, 2, 0, 6): return Z80_STAGE_M1;             /*AND n (size: 2)*/
        case Z80_OPCODE_XYZ(3, 4, 7): return z80_op_RST_y();               /*RST y (size: 1)*/

        case Z80_OPCODE_XYZ(3, 5, 0): return z80_op_RET_cc();             /*RET cc (size: 1)*/
        case Z80_OPCODE_XPQZ(3, 2, 1, 1): return z80_op_JP_HL();           /*JP HL (size: 1)*/
        case Z80_OPCODE_XYZ(3, 5, 2): return Z80_STAGE_M1;             /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 5, 3): return z80_op_EX_DE_HL();        /*EX DE, HL (size: 1)*/
        case Z80_OPCODE_XYZ(3, 5, 4): return Z80_STAGE_M1;           /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 2, 1, 5): return z80_instruction_decode_ED();    /*ED prefix*/
        case Z80_OPCODE_XPQZ(3, 2, 1, 6): return Z80_STAGE_M1;             /*XOR n (size: 2)*/
        case Z80_OPCODE_XYZ(3, 5, 7): return z80_op_RST_y();               /*RST y (size: 1)*/

        case Z80_OPCODE_XYZ(3, 6, 0): return z80_op_RET_cc();             /*RET cc (size: 1)*/
        case Z80_OPCODE_XPQZ(3, 3, 0, 1): return z80_op_POP_rp2();       /*POP rp2 (size: 1)*/
        case Z80_OPCODE_XYZ(3, 6, 2): return Z80_STAGE_M1;             /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 6, 3): return z80_op_DI();                     /*DI (size: 1)*/
        case Z80_OPCODE_XYZ(3, 6, 4): return Z80_STAGE_M1;           /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 3, 0, 5): return z80_op_PUSH_rp2();     /*PUSH rp2 (size: 1)*/
        case Z80_OPCODE_XPQZ(3, 3, 0, 6): return Z80_STAGE_M1;             /*OR  n (size: 2)*/
        case Z80_OPCODE_XYZ(3, 6, 7): return z80_op_RST_y();               /*RST y (size: 1)*/

        case Z80_OPCODE_XYZ(3, 7, 0): return z80_op_RET_cc();             /*RET cc (size: 1)*/
        case Z80_OPCODE_XPQZ(3, 3, 1, 1): return z80_op_LD_SP_HL();    /*LD SP, HL (size: 1)*/
        case Z80_OPCODE_XYZ(3, 7, 2): return Z80_STAGE_M1;             /*JP cc, nn (size: 3)*/
        case Z80_OPCODE_XYZ(3, 7, 3): return z80_op_EI();                     /*EI (size: 1)*/
        case Z80_OPCODE_XYZ(3, 7, 4): return Z80_STAGE_M1;           /*CALL cc, nn (size: 3)*/
        case Z80_OPCODE_XPQZ(3, 3, 1, 5): return z80_instruction_decode_DD_FD(); /*FD prefix*/
        case Z80_OPCODE_XPQZ(3, 3, 1, 6): return Z80_STAGE_M1;             /*CP  n (size: 2)*/
        case Z80_OPCODE_XYZ(3, 7, 7): return z80_op_RST_y();               /*RST y (size: 1)*/

        default:
            assert(0); //Should never get here.
            return Z80_STAGE_RESET;
        }

    case 2: //Second opcode byte
        //Test prefixes
        switch (z80.opcode[0]){
        case 0xCB: // --- 0xCB prefixed opcodes
            return z80_instruction_decode_CB();
        case 0xDD: // --- 0xDD prefixed opcodes
            if (z80.opcode[1] == 0xDD){
                z80.opcode_index--; //0xDDDD = 0xDD
                return Z80_STAGE_M1;
            }
            else if (z80.opcode[1] == 0xFD){
                //0xFD replaces a previous 0xDD
                z80.opcode[0] = 0xFD;
                z80.opcode_index--;
                return Z80_STAGE_M1;
            }
            else if (z80.opcode[1] == 0xED){
                z80.opcode[0] = 0xED;
                z80.opcode_index--;
                return Z80_STAGE_M1;
            }
            //0xDD prefixed opcodes below
            return z80_instruction_decode_DD_FD();

        case 0xFD: // --- 0xFD prefixed opcodes
            if (z80.opcode[1] == 0xFD){
                z80.opcode_index--; //0xDDDD = 0xDD
                return Z80_STAGE_M1;
            }
            else if (z80.opcode[1] == 0xDD){
                //0xFD replaces a previous 0xDD
                z80.opcode[0] = 0xFD;
                z80.opcode_index--;
                return Z80_STAGE_M1;
            }
            else if (z80.opcode[1] == 0xED){
                z80.opcode[0] = 0xED;
                z80.opcode_index--;
                return Z80_STAGE_M1;
            }
            //0xFD prefixed opcodes below
            return z80_instruction_decode_DD_FD();

        case 0xED: // --- 0xED prefixed opcodes
            //Select by X/Z
            switch (z80.opcode[1] & (Z80_OPCODE_X_MASK | Z80_OPCODE_Z_MASK)){
            case Z80_OPCODE_XZ(1, 0):
                if (y[1] != 6){                    /*IN r[y], (C); Size: 2; Flags: S,Z,H,P,N*/
                    assert(0);
                    return Z80_STAGE_RESET;
                }
                else{                                 /*IN F, (C); Size: 2; Flags: S,Z,H,P,N*/
                    assert(0);
                    return Z80_STAGE_RESET;
                }

            case Z80_OPCODE_XZ(1, 1):
                if (y[1] != 6){                        /*OUT r[y], (C); Size: 2; Flags: None*/
                    assert(0);
                    return Z80_STAGE_RESET;
                }
                else          {                           /*OUT F, (C); Size: 2; Flags: None*/
                    assert(0);
                    return Z80_STAGE_RESET;
                }

            case Z80_OPCODE_XZ(1, 2):
                if (!(q[1]))                               /*SBC HL,rp[p]; Size: 2; Flags: ?*/
                    return z80_op_SBC_HL_rp();
                else        {                              /*ADC HL,rp[p]; Size: 2; Flags: ?*/
                    assert(0);
                    return Z80_STAGE_RESET;
                }

            case Z80_OPCODE_XZ(1, 3):
                if (!(q[1])){                          /*LD (nn),rp[p]; Size: 4; Flags: None*/
                    return Z80_STAGE_M1; //Needs two extra bytes
                }
                else        {                          /*LD rp[p],(nn); Size: 4; Flags: None*/
                    return Z80_STAGE_M1; //Needs two extra bytes
                }

            case Z80_OPCODE_XZ(1, 4):                             /*NEG; Size: 2; Flags: All*/
                assert(0);
                return Z80_STAGE_RESET;

            case Z80_OPCODE_XZ(1, 5):
                if (y[1] != 1) {                                /*RETN; Size: 2; Flags: None*/
                    assert(0);
                    return Z80_STAGE_RESET;
                }
                else           {                                /*RETI; Size: 2; Flags: None*/
                    assert(0);
                    return Z80_STAGE_RESET;
                }

            case Z80_OPCODE_XZ(1, 6):                      /*IM(im[y]); Size: 2; Flags: None*/
                return z80_op_IM();

            case Z80_OPCODE_XZ(1, 7):
                /*Make this a switch!*/
                if (y[1] == 0){ assert(0); /*LD(I,A)*/ }
                else if (y[1] == 1){ assert(0); /*LD(R,A)*/ }
                else if (y[1] == 2){ assert(0); /*LD(A,I)*/ }
                else if (y[1] == 3){ assert(0); /*LD(A,R)*/ }
                else if (y[1] == 4){ assert(0); /*RRD*/ }
                else if (y[1] == 5){ assert(0); /*RLD*/ }
                else               { assert(0); /*NOP*/ }
                return Z80_STAGE_RESET;

            case Z80_OPCODE_XZ(2, 0): /* */
            case Z80_OPCODE_XZ(2, 1): /* */
            case Z80_OPCODE_XZ(2, 2): /* */
            case Z80_OPCODE_XZ(2, 3): /*BLOCK(y,z)*/
                switch (z80.opcode[1] & (Z80_OPCODE_Y_MASK | Z80_OPCODE_Z_MASK)){
                case Z80_OPCODE_YZ(4, 0): /*LDI*/
                    assert(0);
                case Z80_OPCODE_YZ(4, 1): /*CPI*/
                    assert(0);
                case Z80_OPCODE_YZ(4, 2): /*INI*/
                    assert(0);
                case Z80_OPCODE_YZ(4, 3): /*OUTI*/
                    assert(0);

                case Z80_OPCODE_YZ(5, 0): /*LDD*/
                    assert(0);
                case Z80_OPCODE_YZ(5, 1): /*CPD*/
                    assert(0);
                case Z80_OPCODE_YZ(5, 2): /*IND*/
                    assert(0);
                case Z80_OPCODE_YZ(5, 3): /*OUTD*/
                    assert(0);
                    return Z80_STAGE_RESET;
                case Z80_OPCODE_YZ(6, 0):            /*LDIR; Size: 2; Flags: H,P,N (cleared)*/
                    return z80_op_LDIR();
                case Z80_OPCODE_YZ(6, 1): /*CPIR*/
                    assert(0);
                case Z80_OPCODE_YZ(6, 2): /*INIR*/
                    assert(0);
                case Z80_OPCODE_YZ(6, 3):                        /*OTIR; Size: 2; Flags: Z,N*/
                    return z80_op_OTIR();
                case Z80_OPCODE_YZ(7, 0): /*LDDR*/
                    assert(0);
                case Z80_OPCODE_YZ(7, 1): /*CPDR*/
                    assert(0);
                case Z80_OPCODE_YZ(7, 2): /*INDR*/
                    assert(0);
                case Z80_OPCODE_YZ(7, 3): /*OTDR*/
                    assert(0);
                    return Z80_STAGE_RESET;

                default: /*NONI/NOP*/
                    return Z80_STAGE_RESET;
                }

            case Z80_OPCODE_XZ(2, 4):                      /* */
            case Z80_OPCODE_XZ(2, 5):                      /* */
            case Z80_OPCODE_XZ(2, 6):                      /* */
            case Z80_OPCODE_XZ(2, 7):                      /* */
            default:     /*x==0 || x==3*/                  /* NOP/NOPI; Size: 2; Flags: None*/
                return Z80_STAGE_RESET;
            }

        default: // --- Unprefixed opcodes (byte 2)
            switch (z80.opcode[0] & (Z80_OPCODE_X_MASK | Z80_OPCODE_Z_MASK)){
            case Z80_OPCODE_XZ(0, 0):
                switch (y[0]){
                case 2:                                       /*DJNZ e; Size: 2; Flags: None*/
                    return z80_op_DJNZ_e();
                case 3:                                        /*JR, e; Size: 2; Flags: None*/
                    return z80_op_JR_e();
                case 4:
                case 5:
                case 6:
                case 7: //(4,5,6,7)                         /*JR cc, e; Size: 2; Flags: None*/
                    return z80_op_JR_cc();
                default:
                    assert(0); //Unimplemented
                    return Z80_STAGE_RESET;
                }
            case Z80_OPCODE_XZ(0, 1):
                if (!q[0]){                           /* LD rp[n], mm; Size: 3; Flags: None */
                    return Z80_STAGE_M1; //+1 byte
                }
                else{
                    assert(0); /*No opcode reaches this point*/ return Z80_STAGE_RESET;
                }
            case Z80_OPCODE_XZ(0, 2):
                if (!q[0]){
                    switch (p[0]){
                    case 2:                              /*LD (nn), HL; size: 3; Flags: None*/
                    case 3:                               /*LD (nn), A; Size: 3; Flags: None*/
                        return Z80_STAGE_M1; //+1 byte
                    default:
                        assert(0); //No opcode reaches this point
                        return Z80_STAGE_RESET;
                    }
                }
                else{
                    switch (p[0]){
                    case 2:                              /*LD HL, (nn); Size: 3; Flags: None*/
                        return Z80_STAGE_M1; //+1 byte
                    case 3:                               /*LD A, (nn); Size: 3; Flags: None*/
                        return Z80_STAGE_M1; //+1 byte
                    default:
                        assert(0); //No opcode should get here
                        return Z80_STAGE_RESET;
                    }
                    assert(0); //Unimplemented
                    return Z80_STAGE_RESET;
                }
            case Z80_OPCODE_XZ(0, 6):                       /* LD r, n; Size: 2; Flags: None*/
                return z80_op_LD_r_n();

            case Z80_OPCODE_XZ(3, 2):                  /* JP cc[y], nn; Size: 3; Flags: None*/
                return Z80_STAGE_M1; //+1 byte
            case Z80_OPCODE_XZ(3, 3):
                switch (y[0]){
                case 0:                                       /* JP nn; Size: 3; Flags: None*/
                    return Z80_STAGE_M1; //+1 byte
                case 2:                                  /* OUT (n), A; Size: 2; Flags: None*/
                    return z80_op_OUT_np_A();
                case 3:                                    /*IN A, (n); Size: 2; Flags: None*/
                    return z80_op_IN_A_np();
                default:
                    assert(0); /*Unimplemented*/
                    return Z80_STAGE_RESET;
                }
            case Z80_OPCODE_XZ(3, 4):                 /* CALL cc[y] nn; Size: 3; Flags: None*/
                return Z80_STAGE_M1; // +1 byte
            case Z80_OPCODE_XZ(3, 5):
                if (q[0]){
                    if (p[0] == 0)                          /* CALL nn; Size: 3; Flags: None*/
                        return Z80_STAGE_M1; //+1 byte
                    else{
                        assert(0); //Unimplemented
                        return Z80_STAGE_RESET;
                    }
                }
                else{
                    assert(0); //Unimplemented
                    return Z80_STAGE_RESET;
                }
            case Z80_OPCODE_XZ(3, 6):            /*alu + 8bit immediate; Size: 2; Flags: ALL*/
                //Select ALU operation by 'y'
                switch (y[0]){
                case Z80_ALUOP_ADD:                               /*ADD n; Size 2; Flags:ALL*/
                    return z80_op_ADD_n();
                case Z80_ALUOP_ADC:
                    assert(0); //Unimplemented
                    return Z80_STAGE_RESET;
                case Z80_ALUOP_SUB:
                    assert(0); //Unimplemented
                    return Z80_STAGE_RESET;
                case Z80_ALUOP_SBC:
                    assert(0); //Unimplemented
                    return Z80_STAGE_RESET;
                case Z80_ALUOP_AND:                               /*AND n; Size 2; Flags:ALL*/
                    return z80_op_AND_n();
                case Z80_ALUOP_XOR:
                    assert(0); //Unimplemented
                    return Z80_STAGE_RESET;
                case Z80_ALUOP_OR:                                /*OR n; Size: 2; Flags:ALL*/
                    return z80_op_OR_n();
                case Z80_ALUOP_CP:                               /*CP n; Size: 2; Flags: All*/
                    return z80_op_CP_n();
                }
            default:
                assert(0); /*Unimplemented*/
                return Z80_STAGE_RESET;
            }
        }
        assert(0); /*Should never get here */ return Z80_STAGE_RESET;

    case 3: //Third opcode byte
        //Test prefixes
        if ((z80.opcode[0] == 0xDD) && (z80.opcode[1] == 0xCB)){ assert(0); }
        else if ((z80.opcode[0] == 0xFD) && (z80.opcode[1] == 0xCB)){ assert(0); }
        else if (z80.opcode[0] == 0xED){
            switch (z80.opcode[1] & (Z80_OPCODE_X_MASK | Z80_OPCODE_Z_MASK)){
            case Z80_OPCODE_XZ(1, 3):                  /*LD (nn), rp[p]; Size: 4; Flags: None*/
                return Z80_STAGE_M1; //+1 byte    /*LD rp[p], (nn); Size: 4; Flags: None*/
            default:
                assert(0);
                return Z80_STAGE_RESET;
            }
            assert(0);
            return Z80_STAGE_RESET;
        }
        else{
            //3-byte. Unprefixed
            switch (z80.opcode[0] & (Z80_OPCODE_X_MASK | Z80_OPCODE_Z_MASK)){
            case Z80_OPCODE_XZ(0, 1):
                if (!q[0])                             /* LD rp[n], nn; Size: 3; Flags: None*/
                    return z80_op_LD_rp_nn();
                else{
                    assert(0); //Unimplemented stuff
                    return Z80_STAGE_RESET;
                }
            case Z80_OPCODE_XZ(0, 2):
                if (!q[0]){
                    switch (p[0]){
                    case 0:
                    case 1:
                        assert(0); //No opcode reaches this point
                        return Z80_STAGE_RESET;
                    case 2:                              /*LD (nn), HL; size: 3; Flags: None*/
                        return z80_op_LD_nnp_HL();
                    case 3:                               /*LD (nn), A; Size: 3; Flags: None*/
                        return z80_op_LD_nnp_A();
                    }
                }
                else{
                    switch (p[0]){
                    case 2:                              /*LD HL, (nn); Size: 3; Flags: None*/
                        return z80_op_LD_HL_nnp();
                    case 3:                               /*LD A, (nn); Size: 3; Flags: None*/
                        return z80_op_LD_A_nnp();
                    default:
                        assert(0); //No opcode should get here
                        return Z80_STAGE_M1;
                    }
                }
            case Z80_OPCODE_XZ(3, 2):                  /* JP cc[y], nn; Size: 3; Flags: None*/
                return z80_op_JP_cc_nn();
            case Z80_OPCODE_XZ(3, 3):
                switch (y[0]){
                case 0:                                       /* JP nn; Size: 3; Flags: None*/
                    return z80_op_JP_nn();
                default:
                    assert(0); //Unimplemented stuff
                    return Z80_STAGE_RESET;
                }
            case Z80_OPCODE_XZ(3, 4):                 /* CALL cc[y] nn; Size: 3; Flags: None*/
                return z80_op_CALL_cc_nn();

            case Z80_OPCODE_XZ(3, 5):
                if (!q[0]){
                    assert(0); //unimplemented
                    return Z80_STAGE_RESET;
                }
                else{
                    if (p[0] == 0)                          /* CALL nn; Size: 3; Flags: None*/
                        return z80_op_CALL_nn();
                    else
                        assert(0); //Unimplemented (?)
                }
            }
            assert(0); /*Unimplemented*/
            return Z80_STAGE_RESET;
        }
        assert(0); /*Should never get here */
        return Z80_STAGE_RESET;

    case 4: //Fourth opcode byte
        //Test prefixes
        if (z80.opcode[0] == 0xED){
            switch (z80.opcode[1] & (Z80_OPCODE_X_MASK | Z80_OPCODE_Z_MASK)){
            case Z80_OPCODE_XZ(1, 3):
                if (!q[1]){                           /*LD (nn), rp[p]; Size: 4; Flags: None*/
                    //Perform write
                    if (z80.write_index == 0){
                        z80.write_address = *((uint16_t*)(z80.opcode + 2)); ///<--- @bug Endianness!
                        *((uint16_t*)(z80.write_buffer)) = *(z80_rp[p[1]]); ///<--- @bug Endianness!
                        return Z80_STAGE_M3;
                    }
                    else if (z80.write_index == 1){
                        ++z80.write_address;
                        return Z80_STAGE_M3;
                    }
                    else{
                        return Z80_STAGE_RESET;
                    }
                }
                else{                                 /*LD rp[p], (nn); Size: 4; Flags: None*/
                    //Perform read
                    if (z80.read_index == 0){
                        z80.read_address = *((uint16_t*)(z80.opcode + 2)); ///<--- @bug Endianness!
                        return Z80_STAGE_M2;
                    }
                    else if (z80.read_index == 1){
                        ++z80.read_address;
                        return Z80_STAGE_M2;
                    }
                    else{
                        *(z80_rp[p[1]]) = *((uint16_t*)(z80.read_buffer)); ///<--- @bug Endianness!
                        return Z80_STAGE_RESET;
                    }
                }
            default:
                assert(0); //Unimplemented
                return Z80_STAGE_RESET;
            }
        }
        else{
            assert(0); //Unimplemented
            return Z80_STAGE_RESET;
        }
        assert(0); /*Should never get here */
        return Z80_STAGE_RESET;
    }
    //Opcode_index > 4
    assert(0); //Opcode overflow!
    return Z80_STAGE_RESET;
}
