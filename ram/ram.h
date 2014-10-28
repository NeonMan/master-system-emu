/**
 *  @file ram.h
 *  @brief RAM module functions.
 */
#ifndef __RAM_H
#define __RAM_H
#include <stddef.h>

void ram_tick();

//Debug functions
///Returns a pointer to the ram image
void* ramdbg_get_mem();

#endif
