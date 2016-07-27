#ifndef __UNITY_SMS_EMU_H
#define __UNITY_SMS_EMU_H

#include <stdint.h>

#include "unity.h"
#include "unity_fixture.h"
#include "z80/z80_externs.h"
#include "z80/z80_macros.h"
#include "z80/z80.h"
#include "ram/ram.h"

/*Access to z80 internals*/
extern struct z80_s z80; //<-- Requires access to the z80 internals

/* --- Helper functions --- */
static uint8_t _get_ram(uint16_t address, int line){
    if (address < RAM_BASE_ADDRESS){
        UNITY_TEST_FAIL(line, "RAM address out of range.");
        return 0;
    }
    else{
        return ((uint8_t*)ramdbg_get_mem())[(address - RAM_BASE_ADDRESS) % RAM_SIZE];
    }
}

/* --- Memory macros --- */
#define TEST_ASSERT_RAM_EQUAL(expected, address)                   {uint8_t b; b=_get_ram((address), __LINE__); TEST_ASSERT_EQUAL_HEX8((expected), b);}
#define TEST_ASSERT_RAM_EQUAL16(expected, address)                 {uint8_t h,l; h=_get_ram((address), __LINE__); l=_get_ram((address) + 1, __LINE__); TEST_ASSERT_EQUAL_HEX16((expected), (((uint16_t)l)<<8)|h);}
#define TEST_ASSERT_STACKTOP_EQUAL(expected)                       TEST_ASSERT_RAM_EQUAL((expected), Z80_SP)
#define TEST_ASSERT_STACKTOP_EQUAL16(expected)                     TEST_ASSERT_RAM_EQUAL16((expected), Z80_SP)
#define TEST_ASSERT_STACK_EQUAL16(expected, index)                 TEST_ASSERT_RAM_EQUAL16((expected), (Z80_SP + (2 * index)))
#define TEST_ASSERT_STACK_EQUAL(expected, index)                   TEST_ASSERT_RAM_EQUAL((expected), (Z80_SP + (index)))

/* --- Register macros --- */

/*Flags register*/
#define TEST_ASSERT_FLAG_SET(flag_mask)                            (TEST_ASSERT_TRUE(Z80_F & (flag_mask)))
#define TEST_ASSERT_FLAG_CLEAR(flag_mask)                          (TEST_ASSERT_FALSE(Z80_F & (flag_mask)))

/*8-Bit*/
#define TEST_ASSERT_A_EQUAL(expected)                              (TEST_ASSERT_EQUAL_HEX8((expected), Z80_A))
#define TEST_ASSERT_F_EQUAL(expected)                              (TEST_ASSERT_EQUAL_HEX8((expected), Z80_F))
#define TEST_ASSERT_Ap_EQUAL(expected)                             (TEST_ASSERT_EQUAL_HEX8((expected), Z80_Ap))
#define TEST_ASSERT_Fp_EQUAL(expected)                             (TEST_ASSERT_EQUAL_HEX8((expected), Z80_Fp))
#define TEST_ASSERT_B_EQUAL(expected)                              (TEST_ASSERT_EQUAL_HEX8((expected), Z80_B))
#define TEST_ASSERT_C_EQUAL(expected)                              (TEST_ASSERT_EQUAL_HEX8((expected), Z80_C))
#define TEST_ASSERT_Bp_EQUAL(expected)                             (TEST_ASSERT_EQUAL_HEX8((expected), Z80_Bp))
#define TEST_ASSERT_Cp_EQUAL(expected)                             (TEST_ASSERT_EQUAL_HEX8((expected), Z80_Cp))
#define TEST_ASSERT_B_EQUAL(expected)                              (TEST_ASSERT_EQUAL_HEX8((expected), Z80_B))
#define TEST_ASSERT_C_EQUAL(expected)                              (TEST_ASSERT_EQUAL_HEX8((expected), Z80_C))
#define TEST_ASSERT_Bp_EQUAL(expected)                             (TEST_ASSERT_EQUAL_HEX8((expected), Z80_Bp))
#define TEST_ASSERT_Cp_EQUAL(expected)                             (TEST_ASSERT_EQUAL_HEX8((expected), Z80_Cp))
#define TEST_ASSERT_H_EQUAL(expected)                              (TEST_ASSERT_EQUAL_HEX8((expected), Z80_H))
#define TEST_ASSERT_L_EQUAL(expected)                              (TEST_ASSERT_EQUAL_HEX8((expected), Z80_L))
#define TEST_ASSERT_Hp_EQUAL(expected)                             (TEST_ASSERT_EQUAL_HEX8((expected), Z80_Hp))
#define TEST_ASSERT_Lp_EQUAL(expected)                             (TEST_ASSERT_EQUAL_HEX8((expected), Z80_Lp))
#define TEST_ASSERT_IXL_EQUAL(expected)                            (TEST_ASSERT_EQUAL_HEX8((expected), Z80_IXL))
#define TEST_ASSERT_IXH_EQUAL(expected)                            (TEST_ASSERT_EQUAL_HEX8((expected), Z80_IXH))
#define TEST_ASSERT_IYL_EQUAL(expected)                            (TEST_ASSERT_EQUAL_HEX8((expected), Z80_IYL))
#define TEST_ASSERT_IYH_EQUAL(expected)                            (TEST_ASSERT_EQUAL_HEX8((expected), Z80_IYH))

/*16bit*/
#define TEST_ASSERT_AF_EQUAL(expected)                             (TEST_ASSERT_EQUAL_HEX16((expected), Z80_AF))
#define TEST_ASSERT_AFp_EQUAL(expected)                            (TEST_ASSERT_EQUAL_HEX16((expected), Z80_AFp))
#define TEST_ASSERT_BC_EQUAL(expected)                             (TEST_ASSERT_EQUAL_HEX16((expected), Z80_BC))
#define TEST_ASSERT_BCp_EQUAL(expected)                            (TEST_ASSERT_EQUAL_HEX16((expected), Z80_BCp))
#define TEST_ASSERT_DE_EQUAL(expected)                             (TEST_ASSERT_EQUAL_HEX16((expected), Z80_DE))
#define TEST_ASSERT_DEp_EQUAL(expected)                            (TEST_ASSERT_EQUAL_HEX16((expected), Z80_DEp))
#define TEST_ASSERT_HL_EQUAL(expected)                             (TEST_ASSERT_EQUAL_HEX16((expected), Z80_HL))
#define TEST_ASSERT_HLp_EQUAL(expected)                            (TEST_ASSERT_EQUAL_HEX16((expected), Z80_HLp))
#define TEST_ASSERT_IX_EQUAL(expected)                             (TEST_ASSERT_EQUAL_HEX16((expected), Z80_IX))
#define TEST_ASSERT_IY_EQUAL(expected)                             (TEST_ASSERT_EQUAL_HEX16((expected), Z80_IY))
#define TEST_ASSERT_SP_EQUAL(expected)                             (TEST_ASSERT_EQUAL_HEX16((expected), Z80_SP))
#define TEST_ASSERT_PC_EQUAL(expected)                             (TEST_ASSERT_EQUAL_HEX16((expected), Z80_PC))

#endif
