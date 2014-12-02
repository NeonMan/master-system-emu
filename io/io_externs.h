/** @file io_externs.h
 *  @brief Variable declarations for IO module.
 * 
 *  Provides the variables needed by code using the IO chip outputs.
 *
 */
#ifndef __IO_EXTERNS_H
#define __IO_EXTERNS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//IO mapper enable bits. Active LOW
#define IO_EXPANSION_SLOT (1<<7)
#define IO_CARTRIDGE_SLOT (1<<6)
#define IO_CARD_SLOT      (1<<5)
#define IO_RAM            (1<<4)
#define IO_BIOS           (1<<3)
#define IO_PERIPHERIAL    (1<<2)
#define IO_UNK1           (1<<1)
#define IO_UNK0           (1)

extern uint8_t io_stat; ///<-- The exported #CE lines. #MREQ/#IOREQ dependant

#ifdef __cplusplus
}
#endif

#endif
