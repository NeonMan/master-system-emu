#ifndef __MAPPER_H
#define __MAPPER_H
/**
 * @file  mapper.h
 * @brief Address and register definitions of the SEGA memory mapper.
 */

#define SMS_MAPPER_CONFIG_ADDRESS 0xFFFC
#define SMS_MAPPER_BANK0_ADDRESS  0xFFFD
#define SMS_MAPPER_BANK1_ADDRESS  0xFFFE
#define SMS_MAPPER_BANK2_ADDRESS  0xFFFF

/*Memory-mapped peripherals*/
__sfr __at SMS_MAPPER_CONFIG_ADDRESS __mapper_config;
__sfr __at SMS_MAPPER_BANK0_ADDRESS  __mapper_bank0;
__sfr __at SMS_MAPPER_BANK1_ADDRESS  __mapper_bank1;
__sfr __at SMS_MAPPER_BANK2_ADDRESS  __mapper_bank2;

#endif
