/**
 *  @file ram.h
 *  @brief RAM module functions.
 */
#ifndef __RAM_H
#define __RAM_H
#include <stddef.h>

#define RAM_SIZE (1024 * 8)
#define RAM_BASE_ADDRESS 0xC000

void ram_tick();

//Debug functions
///Returns a pointer to the ram image
void* ramdbg_get_mem();

#endif
