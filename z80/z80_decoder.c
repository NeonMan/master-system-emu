#include "z80_decoder.h"
#include "z80_macros.h"
#include "z80_register_lut.h"
#include <assert.h>

/*
Most information taken from z80.info

Decoding Z80 instructions:
http://www.z80.info/decoding.htm
*/

///Decodes DDCB/FDCB prefix opcodes
///First two bytes already decoded (0xDDCB/0xFDCB)
int z80_instruction_decode_DDCB_FDCB(){

    //Relevant sub-byte divisions, for each of the 4 bytes (max) in an opcode.
    const uint8_t x[4] = { z80.opcode[0] >> 6, z80.opcode[1] >> 6, z80.opcode[2] >> 6, z80.opcode[3] >> 6 };
    const uint8_t y[4] = { (z80.opcode[0] >> 3) & 0x7, (z80.opcode[1] >> 3) & 0x7, (z80.opcode[2] >> 3) & 0x7, (z80.opcode[3] >> 3) & 0x7 };
    const uint8_t z[4] = { z80.opcode[0] & 0x7, z80.opcode[1] & 0x7, z80.opcode[2] & 0x7, z80.opcode[3] & 0x7 };
    const uint8_t p[4] = { (z80.opcode[0] >> 4) & 0x3, (z80.opcode[1] >> 4) & 0x3, (z80.opcode[2] >> 4) & 0x3, (z80.opcode[3] >> 4) & 0x3 };
    const uint8_t q[4] = { z80.opcode[0] & (1 << 3), z80.opcode[1] & (1 << 3), z80.opcode[2] & (1 << 3), z80.opcode[3] & (1 << 3) };

    assert(0);
    return Z80_STAGE_RESET;
}

///Decodes DD/FD prefix
///First byte is already decoded (0xDD/0xFD)
int z80_instruction_decode_DD_FD(){

    //Relevant sub-byte divisions, for each of the 4 bytes (max) in an opcode.
    const uint8_t x[4] = { z80.opcode[0] >> 6, z80.opcode[1] >> 6, z80.opcode[2] >> 6, z80.opcode[3] >> 6 };
    const uint8_t y[4] = { (z80.opcode[0] >> 3) & 0x7, (z80.opcode[1] >> 3) & 0x7, (z80.opcode[2] >> 3) & 0x7, (z80.opcode[3] >> 3) & 0x7 };
    const uint8_t z[4] = { z80.opcode[0] & 0x7, z80.opcode[1] & 0x7, z80.opcode[2] & 0x7, z80.opcode[3] & 0x7 };
    const uint8_t p[4] = { (z80.opcode[0] >> 4) & 0x3, (z80.opcode[1] >> 4) & 0x3, (z80.opcode[2] >> 4) & 0x3, (z80.opcode[3] >> 4) & 0x3 };
    const uint8_t q[4] = { z80.opcode[0] & (1 << 3), z80.opcode[1] & (1 << 3), z80.opcode[2] & (1 << 3), z80.opcode[3] & (1 << 3) };

    assert(0);
    return Z80_STAGE_RESET;
}

///Decodes the CB-prefixed opcodes.
///First byte (0xCB) is already decoded.
int z80_instruction_decode_CB(){

    //Relevant sub-byte divisions, for each of the 4 bytes (max) in an opcode.
    const uint8_t x[4] = { z80.opcode[0] >> 6, z80.opcode[1] >> 6, z80.opcode[2] >> 6, z80.opcode[3] >> 6 };
    const uint8_t y[4] = { (z80.opcode[0] >> 3) & 0x7, (z80.opcode[1] >> 3) & 0x7, (z80.opcode[2] >> 3) & 0x7, (z80.opcode[3] >> 3) & 0x7 };
    const uint8_t z[4] = { z80.opcode[0] & 0x7, z80.opcode[1] & 0x7, z80.opcode[2] & 0x7, z80.opcode[3] & 0x7 };
    const uint8_t p[4] = { (z80.opcode[0] >> 4) & 0x3, (z80.opcode[1] >> 4) & 0x3, (z80.opcode[2] >> 4) & 0x3, (z80.opcode[3] >> 4) & 0x3 };
    const uint8_t q[4] = { z80.opcode[0] & (1 << 3), z80.opcode[1] & (1 << 3), z80.opcode[2] & (1 << 3), z80.opcode[3] & (1 << 3) };

    switch (z80.opcode_index){
    //Second opcode byte
    case 2:
        switch (x[1]){
        case 0:                                     /*rotation r[z]; Size: 2; Flags: ?*/
            assert(0); /*Unimplemented*/ return Z80_STAGE_RESET;
        case 1:                                /*BIT y,r[z]; Size: 2; Flags: _S,Z,H,_P,N*/
            Z80_F = (Z80_F & (Z80_CLRFLAG_ZERO & Z80_CLRFLAG_ADD)); //Clear Z,N
            Z80_F = Z80_F | ((1 << y[1]) & (*z80_r[z[1]])) ? 0 : Z80_CLRFLAG_ZERO;
            Z80_F = Z80_F | Z80_FLAG_HC;
            return Z80_STAGE_RESET;
        case 2:                                     /*RES y,r[z]; Size: 2; Flags: None*/
            *(z80_r[z[1]]) = *(z80_r[z[1]]) & (~(1 << y[1]));
            return Z80_STAGE_RESET;
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
        //Select by X/Z
        switch (z80.opcode[0] & (Z80_OPCODE_X_MASK | Z80_OPCODE_Z_MASK)){
        case Z80_OPCODE_XZ(0, 0):
            //Select by y
            switch (y[0]){
            case 0:                                             /* NOP; Size: 1; Flags: None*/
                return Z80_STAGE_RESET;
            case 1:                                       /*EX AF, AFp; Size: 1; Flags: None*/
            {
                const uint16_t tmp_af = Z80_AF;
                Z80_AF = Z80_AFp;
                Z80_AFp = tmp_af;
                return Z80_STAGE_RESET;
            }
            case 2:                                           /*DJNZ d; Size: 2; Flags: None*/
                return Z80_STAGE_M1; //One extra byte
            case 3:                                             /*JR d; Size: 2; Flags: None*/
                return Z80_STAGE_M1; //One extra byte
            default: //(4,5,6,7)                  /* JR [C,NC,Z,NZ], e; Size: 2; Flags: None*/
                return Z80_STAGE_M1; // +1 Byte
            }

        case Z80_OPCODE_XZ(0, 1):
            if (!q[0])                                  /*LD rp[p], nn; Size: 3; Flags: None*/
                return Z80_STAGE_M1; //Needs two extra bytes
            else{                                        /*ADD HL,rp[p] ; Size: 1; Flags N,C*/
                const uint16_t old_hl = Z80_HL;
                Z80_HL = Z80_HL + *z80_rp[p[0]];
                //Clear N/Carry flag (bits 1,0)
                Z80_F = (Z80_F & (Z80_CLRFLAG_CARRY & Z80_CLRFLAG_ADD))
                    //Set carry flag (bit 0)
                    | Z80_SETFLAG_CARRY(old_hl, Z80_HL);
                return Z80_STAGE_RESET;
            }

        case Z80_OPCODE_XZ(0, 2):
            //Select by q
            if (!(q[0])){
                if (p[0] == 0) {                            /*LD (BC),A; Size:1; Flags: None*/
                    //If the write has not been performed, request it
                    if (z80.write_index == 0){
                        z80.write_address = Z80_BC;
                        z80.write_buffer[0] = Z80_A;
                        return Z80_STAGE_M3;
                    }
                    //Otherwise, reset pipeline
                    else{
                        assert(z80.write_index == 1);
                        return Z80_STAGE_RESET;
                    }
                }
                else if (p[0] == 1)                        /*LD (DE),A; Size: 1; Flags: None*/
                    assert(0); //unimplemented
                else if (p[0] == 2)                       /*LD (nn),HL; Size: 3; Flags: None*/
                    return Z80_STAGE_M1; //Needs two extra bytes
                else if (p[0] == 3)                        /*LD (nn),A; Size: 3; Flags: None*/
                    return Z80_STAGE_M1; //Needs two extra bytes
            }
            else{
                if (p[0] == 0){                            /*LD A,(BC); Size: 1; Flags: None*/
                    assert(0); //Unimplemented
                }
                else if (p[0] == 1){                       /*LD A,(DE); Size: 1; Flags: None*/
                    if (z80.read_index == 0){
                        z80.read_address = Z80_DE;
                        return Z80_STAGE_M2;
                    }
                    else{
                        Z80_A = z80.read_buffer[0];
                        return Z80_STAGE_RESET;
                    }
                }
                else if (p[0] == 2){                      /*LD HL,(nn); Size: 3; Flags: None*/
                    return Z80_STAGE_M1; //2 extra bytes
                }
                else if (p[0] == 3){                       /*LD A,(nn); Size: 3; Flags: None*/
                    return Z80_STAGE_M1; //Needs two extra bytes
                }
            }

        case Z80_OPCODE_XZ(0, 3):
            //Select by q
            if (!(q[0])){                                 /*INC(rp[p]); Size: 1; Flags: None*/
                ++(*(z80_rp[p[0]]));
                return Z80_STAGE_RESET;
            }
            else{                                         /*DEC(rp[p]); Size: 1; Flags: None*/
                --(*(z80_rp[p[0]]));
                return Z80_STAGE_RESET;
            }

        case Z80_OPCODE_XZ(0, 4):                     /*INC(r[y]); Size: 1; Flags: S,Z,H,V,N*/
        {
            const uint8_t old_r = *z80_r[y[0]];
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
        case Z80_OPCODE_XZ(0, 5):                      /*DEC(r[y]); Size:1; Flags: S,Z,H,P,N*/
        {
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
        case Z80_OPCODE_XZ(0, 6):                         /*LD(r[y],n); Size: 2; Flags: None*/
            return Z80_STAGE_M1; //Needs one extra byte

        case Z80_OPCODE_XZ(0, 7):
            switch (y[0]){
            case 0:                                            /*RLCA; Size: 1; Flags: H,N,C*/
                Z80_A = (Z80_A << 1) | (Z80_A & (1 << 7) ? 1 : 0);
                Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_ADD & Z80_CLRFLAG_CARRY))
                    | (Z80_F & 1 ? Z80_FLAG_CARRY : 0);
                return Z80_STAGE_RESET;

            case 1:                                            /*RRCA; Size: 1; Flags: H,N,C*/
                Z80_A = (Z80_A >> 1) | (Z80_A & (1 >> 7) ? (1 << 7) : 0);
                Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_ADD & Z80_CLRFLAG_CARRY))
                    | (Z80_F & (1 << 7) ? Z80_FLAG_CARRY : 0);
                return Z80_STAGE_RESET;

            case 2:                                             /*RLA; Size: 1; Flags: H,N,C*/
            {
                const uint8_t next_carry = Z80_A & (1 << 7);
                Z80_A = (Z80_A << 1) | (Z80_F & Z80_FLAG_CARRY ? 1 : 0);
                Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_ADD & Z80_CLRFLAG_CARRY))
                    | (next_carry ? Z80_FLAG_CARRY : 0);
                return Z80_STAGE_RESET;
            }
            case 3:                                             /*RRA; Size: 1; Flags: H,N,C*/
            {
                const uint8_t next_carry = Z80_A & (1);
                Z80_A = (Z80_A >> 1) | (Z80_F & Z80_FLAG_CARRY ? (1 << 7) : 0);
                Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_ADD & Z80_CLRFLAG_CARRY))
                    | (next_carry ? Z80_FLAG_CARRY : 0);
                return Z80_STAGE_RESET;
            }
            case 4:                                         /*DAA; Size: 1; Flags: S,Z,H,P,C*/
                assert(0); //Unimplemented
                return Z80_STAGE_RESET;

            case 5:                                               /*CPL; Size: 1; Flags: H,N*/
                Z80_A = ~Z80_A;
                Z80_F = Z80_F | Z80_FLAG_HC | Z80_FLAG_ADD;
                return Z80_STAGE_RESET;

            case 6:                                                 /*SCF; Size: 1; Flags: C*/
                Z80_F = (Z80_F & (Z80_CLRFLAG_HC & Z80_CLRFLAG_ADD)) | Z80_FLAG_CARRY;
                return Z80_STAGE_RESET;

            case 7:                                                 /*CCF; Size: 1; Flags: C*/
                Z80_F = (Z80_F ^ Z80_FLAG_CARRY) & Z80_CLRFLAG_ADD;
                return Z80_STAGE_RESET;
            }

        case Z80_OPCODE_XZ(1, 0):                       /* */
        case Z80_OPCODE_XZ(1, 1):                       /* */
        case Z80_OPCODE_XZ(1, 2):                       /* */
        case Z80_OPCODE_XZ(1, 3):                       /* */
        case Z80_OPCODE_XZ(1, 4):                       /* */
        case Z80_OPCODE_XZ(1, 5):                       /* */
        case Z80_OPCODE_XZ(1, 7):                       /*LD r[y],r[z]; Size: 1; Flags: None*/
            //Source can never be (HL). That implies z[0]==6
            //Target can be (HL)
            if (z80_r[y[0]]){ //If target != (HL)
                *(z80_r[y[0]]) = *(z80_r[z[0]]);
                return Z80_STAGE_RESET;
            }
            else{ //Target is (HL)
                if (z80.write_index == 0){
                    z80.write_address = Z80_HL;
                    z80.write_buffer[0] = *(z80_r[z[0]]);
                    return Z80_STAGE_M3;
                }
                else{
                    return Z80_STAGE_RESET;
                }
            }


        case Z80_OPCODE_XZ(1, 6):
            if (y[0] == 6){                                         /*HALT; Size: 1; Flags: None*/
                assert(0); /*Unimplemented*/ return Z80_STAGE_RESET;
            }
            else{                                       /*LD r[y], (HL); Size:1; Flags: None*/
                //Source is always (HL, Target is never (HL)
                if (z80.read_index == 0){
                    z80.read_address = Z80_HL;
                    return Z80_STAGE_M2;
                }
                else{
                    *(z80_r[y[0]]) = z80.read_buffer[0];
                    return Z80_STAGE_RESET;
                }
            }

        case Z80_OPCODE_XZ(2, 0):                            /* */
        case Z80_OPCODE_XZ(2, 1):                            /* */
        case Z80_OPCODE_XZ(2, 2):                            /* */
        case Z80_OPCODE_XZ(2, 3):                            /* */
        case Z80_OPCODE_XZ(2, 4):                            /* */
        case Z80_OPCODE_XZ(2, 5):                            /* */
        case Z80_OPCODE_XZ(2, 6):                            /* */
        case Z80_OPCODE_XZ(2, 7):                            /*alu r[z]; Size: 1; Flags: ALL*/
            //If source is (HL), make a M2 read  if needed
            if ((z80_r[z[0]] == 0) && (z80.read_index == 0)){
                if (z80.read_index == 0){
                    z80.read_address = Z80_HL;
                    return Z80_STAGE_M2;
                }
                else{
                    Z80_A = Z80_A ^ z80.read_buffer[0];
                }
            }
            //Select ALU operation by 'y'
            switch (y[0]){
            case Z80_ALUOP_ADD:
            case Z80_ALUOP_ADC:
            case Z80_ALUOP_SUB:
            case Z80_ALUOP_SBC:
            case Z80_ALUOP_AND:
            {
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
            case Z80_ALUOP_XOR:                              /*XOR r[z]; Size: 1; Flags: All*/
            {
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
            case Z80_ALUOP_OR:
            {
                const uint8_t orig_a = Z80_A;
                if (z80_r[z[0]]){                              /*OR r[z]; Size: 1; Flags:ALL*/
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
            case Z80_ALUOP_CP:
            default:
                assert(0); /*Unimplemented*/ return Z80_STAGE_RESET;
            }

        case Z80_OPCODE_XZ(3, 0):                          /*RET cc[y]; Size: 1; Flags: None*/
            assert(0); /*Unimplemented*/ return Z80_STAGE_RESET;

        case Z80_OPCODE_XZ(3, 1):
            if (!q[0]){                                   /*POP rp2[p]; Size: 1; Flags: None*/
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
                assert(0); /*Unimplemented*/ return Z80_STAGE_RESET;
            }
            else{
                switch (p[0]){
                case 0:                                          /*RET; Size: 1; Flags: None*/
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
                    assert(0); /*Unimplemented*/ return Z80_STAGE_RESET;
                case 1:                                          /*EXX; Size: 1; Flags: None*/
                {
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
                case 2:                                      /*JP (HL); Size: 1; Flags: None*/
                    assert(0); /*unimplemented*/ return Z80_STAGE_RESET;
                case 3:                                     /*LD SP,HL; Size: 1; Flags: None*/
                    assert(0); /*unimplemented*/ return Z80_STAGE_RESET;
                }
            }

        case Z80_OPCODE_XZ(3, 2):                       /*JP cc[y], nn; Size: 3; Flags: None*/
            return Z80_STAGE_M1; //+2 bytes

        case Z80_OPCODE_XZ(3, 3):
            switch (y[0]){
            case 0:                                            /*JP nn; Size: 3; Flags: None*/
                return Z80_STAGE_M1; //+2 Bytes
            case 1:                                                            /*0xCB prefix*/
                return Z80_STAGE_M1;
            case 2:                                         /*OUT n, A; Size: 2; Flags: None*/
                return Z80_STAGE_M1; //+1 Byte
            case 3:                                          /*IN A, n; Size: 2; Flags: None*/
                return Z80_STAGE_M1; //+1 Byte
            case 4:                                      /*EX (SP), HL; Size: 1; Flags: None*/
                assert(0); /*Unimplemented*/ return Z80_STAGE_RESET;
            case 5:                                        /*EX(DE,HL); Size: 1; Flags: None*/
            {
                const uint16_t old_de = Z80_DE;
                Z80_DE = Z80_HL;
                Z80_HL = old_de;
                return Z80_STAGE_RESET;
            }
            case 6:                                               /*DI; Size: 1; Flags: None*/
                z80.iff[0] = 0;
                z80.iff[1] = 0;
                return Z80_STAGE_RESET;
            case 7:                                               /*EI; Size: 1; Flags: None*/
                z80.iff[0] = 1;
                z80.iff[1] = 1;
                return Z80_STAGE_RESET;
            }

        case Z80_OPCODE_XZ(3, 4):                     /*CALL cc[y], nn; Size: 3; Flags: None*/
            return Z80_STAGE_M1; //+2 bytes

        case Z80_OPCODE_XZ(3, 5):
            if (!q[0]){ /*PUSH rp2[p]; Size: 1; Flags: None*/
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
            else{
                switch (p[0]){
                case 0:                                      /*CALL nn; Size: 3; Flags: None*/
                    return Z80_STAGE_M1; //+2 Bytes
                case 1:
                    //DD prefix
                case 2:
                    //ED prefix
                case 3:
                    //FD prefix
                    return Z80_STAGE_M1;
                }
            }

        case Z80_OPCODE_XZ(3, 6):                /*alu + 8bit immediate; Size: 2; Flags: ALL*/
            return Z80_STAGE_M1; //+1 byte

        case Z80_OPCODE_XZ(3, 7):                            /*RST y*8; Size: 1; Flags: None*/
            assert(0); /*Unimplemented*/
            return Z80_STAGE_RESET;
        }
        assert(0); /*Should never get here */
        return Z80_STAGE_RESET;

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
                if (!(q[1])){                              /*SBC HL,rp[p]; Size: 2; Flags: ?*/
                    assert(0);
                    return Z80_STAGE_RESET;
                }
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
                z80.iff[0] = z80_im[y[1]][0];
                z80.iff[1] = z80_im[y[1]][1];
                return Z80_STAGE_RESET;

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
                case Z80_OPCODE_YZ(4, 0):
                case Z80_OPCODE_YZ(4, 1):
                case Z80_OPCODE_YZ(4, 2):
                case Z80_OPCODE_YZ(4, 3):

                case Z80_OPCODE_YZ(5, 0):
                case Z80_OPCODE_YZ(5, 1):
                case Z80_OPCODE_YZ(5, 2):
                case Z80_OPCODE_YZ(5, 3):
                    assert(0);
                    return Z80_STAGE_RESET;

                case Z80_OPCODE_YZ(6, 0):            /*LDIR; Size: 2; Flags: H,P,N (cleared)*/
                    //(DE) <-- (HL); ++DE; ++HL; --BC; BC? repeat : end;
                    //Test wether we have finished already
                    if (Z80_BC == 0)
                        return Z80_STAGE_RESET;
                    //Perform a read
                    else if (z80.read_index == 0){
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

                case Z80_OPCODE_YZ(6, 1):
                case Z80_OPCODE_YZ(6, 2):
                case Z80_OPCODE_YZ(6, 3):

                case Z80_OPCODE_YZ(7, 0):
                case Z80_OPCODE_YZ(7, 1):
                case Z80_OPCODE_YZ(7, 2):
                case Z80_OPCODE_YZ(7, 3):
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
                case 3:                                           /*JR, e; Size: 2; Flags: None*/
                {
                    const int8_t pc_shift = *((int8_t*)&z80.opcode[1]); ///<-- @bug Endianness
                    const int32_t next_pc = Z80_PC + pc_shift; //Signed relative jump
                    Z80_PC = (next_pc & 0xFFFF);
                    return Z80_STAGE_RESET;
                }
                case 4:
                case 5:
                case 6:
                case 7: //(4,5,6,7)                  /* JR [C,NC,Z,NZ], e; Size: 2; Flags: None*/
                    //Test required flag
                    if ((Z80_F & z80_cc[y[0] - 4]) == (z80_cc_stat[y[0] - 4])){
                        const int8_t pc_shift = *((int8_t*)&z80.opcode[1]); ///<-- @bug Endianness
                        const int32_t next_pc = Z80_PC + pc_shift; //Signed relative jump
                        Z80_PC = (next_pc & 0xFFFF);
                    }
                    return Z80_STAGE_RESET;
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
            case Z80_OPCODE_XZ(0, 6):                      /* LD r, i; Size: 2; Flags: None */
                //If target is HL, perform write
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

            case Z80_OPCODE_XZ(3, 2):                   /* JP cc[y], nn; Size: 3; Flags: None*/
                return Z80_STAGE_M1; //+1 byte
            case Z80_OPCODE_XZ(3, 3):
                switch (y[0]){
                case 0:                                       /* JP nn; Size: 3; Flags: None*/
                    return Z80_STAGE_M1; //+1 byte
                case 2:                                  /* OUT (n), A; Size: 2; Flags: None*/
                    //if no byte has been read, prepare a write
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
                case Z80_ALUOP_ADD:
                case Z80_ALUOP_ADC:
                case Z80_ALUOP_SUB:
                case Z80_ALUOP_SBC:
                case Z80_ALUOP_AND:
                case Z80_ALUOP_XOR:
                case Z80_ALUOP_OR:                                /*OR n; Size: 1; Flags:ALL*/
                {
                    const uint8_t orig_a = Z80_A;
                    Z80_A = Z80_A | z80.opcode[1];
                    Z80_F = 0;
                    Z80_F |= Z80_SETFLAG_SIGN(Z80_A);
                    Z80_F |= Z80_SETFLAG_ZERO(Z80_A);
                    Z80_F |= Z80_SETFLAG_OVERFLOW(orig_a, Z80_A);
                    return Z80_STAGE_RESET;
                }
                case Z80_ALUOP_CP:                               /*CP n; Size: 2; Flags: All*/
                    Z80_F = 0;
                    Z80_F |= Z80_FLAG_ADD; //Flag is set, always
                    Z80_F |= Z80_SETFLAG_SIGN(Z80_A - z80.opcode[1]);
                    Z80_F |= Z80_SETFLAG_ZERO(Z80_A - z80.opcode[1]);
                    Z80_F |= Z80_SETFLAG_HC(Z80_A, Z80_A - z80.opcode[1]);
                    Z80_F |= Z80_SETFLAG_OVERFLOW(Z80_A, Z80_A - z80.opcode[1]);
                    Z80_F |= Z80_SETFLAG_BORROW(Z80_A, Z80_A - z80.opcode[1]);
                    return Z80_STAGE_RESET;
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
                if (!q[0]){                           /* LD rp[n], mm; Size: 3; Flags: None */
                    //mm is stored in the opcode's bytes 1,2.
                    const uint16_t immediate = *((uint16_t*)(z80.opcode + 1));
                    *z80_rp[p[0]] = immediate; ///<-- @bug Endianness!
                    return Z80_STAGE_RESET;
                }
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
                        assert(0);
                        return Z80_STAGE_RESET;
                    case 3:                               /*LD (nn), A; Size: 3; Flags: None*/
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
                }
                else{
                    switch (p[0]){
                    case 2:                              /*LD HL, (nn); Size: 3; Flags: None*/
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
                    case 3:                               /*LD A, (nn); Size: 3; Flags: None*/
                        if (z80.read_index == 0){
                            z80.read_address = *((uint16_t*)(z80.opcode + 1)); ///<-- @bug endianness!
                            return Z80_STAGE_M2;
                        }
                        else{
                            Z80_A = z80.read_buffer[0];
                            return Z80_STAGE_RESET;
                        }
                    default:
                        assert(0); //No opcode should get here
                        return Z80_STAGE_M1;
                    }
                }
            case Z80_OPCODE_XZ(3, 2):                  /* JP cc[y], nn; Size: 3; Flags: None*/
                //Test condition
                if ((Z80_F & (z80_cc[y[0]])) == (z80_cc_stat[y[0]])){
                    const uint16_t new_pc = *((uint16_t*)(z80.opcode + 1));
                    Z80_PC = new_pc; ///<-- @bug Endianness!
                    return Z80_STAGE_RESET;
                }
                else{
                    return Z80_STAGE_RESET;
                }
            case Z80_OPCODE_XZ(3, 3):
                switch (y[0]){
                case 0:                                       /* JP nn; Size: 3; Flags: None*/
                {
                    ///New PC stored in opcode's last bytes. @bug Endianness.
                    const uint16_t new_pc = *((uint16_t*)(z80.opcode + 1));
                    Z80_PC = new_pc;
                    return Z80_STAGE_RESET;
                }
                default:
                    assert(0); //Unimplemented stuff
                    return Z80_STAGE_RESET;
                }
            case Z80_OPCODE_XZ(3, 4):                 /* CALL cc[y] nn; Size: 3; Flags: None*/
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

            case Z80_OPCODE_XZ(3, 5):
                if (!q[0]){
                    assert(0); //unimplemented
                    return Z80_STAGE_RESET;
                }
                else{
                    if (p[0] == 0){                         /* CALL nn; Size: 3; Flags: None*/
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
                        assert(0); //Will never get here
                    }
                }
            }
            assert(0); /*Unimplemented*/ return Z80_STAGE_RESET;
        }
        assert(0); /*Should never get here */ return Z80_STAGE_RESET;

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
        assert(0); /*Should never get here */ return Z80_STAGE_RESET;
        return Z80_STAGE_RESET;
    }
    //Opcode_index > 4
    assert(0); //Opcode overflow!
    return Z80_STAGE_RESET;
}
