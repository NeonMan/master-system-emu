/** @file z80.h
 *  @brief The z80 module functions.
 */
#ifndef __Z80_H
#define __Z80_h

#include "z80_externs.h"

void z80_tick();
void z80_init(void(*data_f) (uint8_t), void(*ctrl_f) (uint8_t));

#endif