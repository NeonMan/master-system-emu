/** @file io.h
 *  @brief IO chip function declarations.
 */
#ifndef __IO_H
#define __IO_H

#include <stdint.h>
#include "io_externs.h"
#include "../z80/z80_externs.h"

#ifdef __cplusplus
extern "C" {
#endif

void io_tick(); ///<-- Updates the IO chip state.

#ifdef __cplusplus
}
#endif

#endif