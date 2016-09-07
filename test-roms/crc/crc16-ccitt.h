/**
 * \file crc16-ccitt.h
 * Functions and types for CRC checks.
 *
 * Generated on Wed Sep  7 22:12:21 2016,
 * by pycrc v0.9, https://pycrc.org
 * using the configuration:
 *    Width         = 16
 *    Poly          = 0x1021
 *    Xor_In        = 0x1d0f
 *    ReflectIn     = False
 *    Xor_Out       = 0x0000
 *    ReflectOut    = False
 *    Algorithm     = bit-by-bit-fast
 *****************************************************************************/
#ifndef __CRC16_CCITT__
#define __CRC16_CCITT__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * The definition of the used algorithm.
 *
 * This is not used anywhere in the generated code, but it may be used by the
 * application code to call algoritm-specific code, is desired.
 *****************************************************************************/
#define CRC_ALGO_BIT_BY_BIT_FAST 1


/**
 * The type of the CRC values.
 *
 * This type must be big enough to contain at least 16 bits.
 *****************************************************************************/
typedef uint16_t crc_t;


/**
 * Calculate the initial crc value.
 *
 * \return     The initial crc value.
 *****************************************************************************/
#define crc16_ccitt_init()      (0x1d0f)


/**
 * Update the crc value with new data.
 *
 * \param crc      The current crc value.
 * \param data     Pointer to a buffer of \a data_len bytes.
 * \param data_len Number of bytes in the \a data buffer.
 * \return         The updated crc value.
 *****************************************************************************/
crc_t crc16_ccitt_update(crc_t crc, const void *data, uint16_t data_len);


/**
 * Calculate the final crc value.
 *
 * \param crc  The current crc value.
 * \return     The final crc value.
 *****************************************************************************/
#define crc16_ccitt_finalize(crc)      (crc ^ 0x0000)


#ifdef __cplusplus
}           /* closing brace for extern "C" */
#endif

#endif      /* __CRC16_CCITT__ */
