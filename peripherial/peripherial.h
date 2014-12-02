/**
 * @file peripherial.h
 * @brief Peripherial ports module.
 */
#ifndef __PERIPHERIAL_H
#define __PERIPHERIAL_H

#include <stdint.h>
#include "../z80/z80_externs.h"
#include "../io/io_externs.h"

#ifdef __cplusplus
extern "C" {
#endif

void per_tick();

#ifdef __cplusplus
}
#endif
#endif