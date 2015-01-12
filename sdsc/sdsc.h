/**
* @file sdsc.h
* @brief Sample implementations of SDSC callbacks.
*
*/
#ifndef __SDSC_H
#define __SDSC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

///Sample SDSC data callback function.
void sdsc_write(uint8_t b);
///Sample SDSC control callback function.
void sdsc_control(uint8_t b);

#ifdef __cplusplus
}
#endif

#endif