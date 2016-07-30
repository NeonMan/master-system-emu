#ifndef __EMU_HOOKS_H
#define __EMU_HOOKS_H
/**
 * @file  emu-hooks.h
 * @brief Header file for emu-hooks.
 */

#include <stdint.h>

/* Hook operation IDs */
#define EMU_BREAK 0xFF

/* Generic hook call. */
void emu_hook(uint8_t op, uint16_t arg1, uint16_t arg2);

#endif
