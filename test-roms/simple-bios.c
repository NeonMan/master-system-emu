/**
 * @file  test-roms/simple-bios.c
 * @brief Most simple BIOS, will switch to cartridge ROM.
 *
 */

#include <stdint.h>
#include "sms/io.h"
#include "sms/sms.h"
#include "sms/sdsc.h"
#include "sms/mapper.h"
#include "sms/intv-dummy.h"

#define CLEANUP_RAM 

#define RAM_CODE_SIZE 0x1000
#define BOOTJUMP_SIZE 0x0100

#define BOOT_HELLO        "Simple SMS bootloader."
#define BOOT_NO_SIGNATURE "Failed to find bootloader signature."
#define BOOT_NO_CHAINLOAD "Failed to start program."

/**
 * @brief Signature mark for RAM loadable code.
 * Bootloader code searches for this signature at the end of the RAM-loaded
 * function. Any code beyond this point is not guaranteed to be available.
 */
#define RAM_CODE_SIGNATURE \
  __asm \
      jr boot_signature_end \
      .ascii "YAP!" \
    boot_signature_end: \
  __endasm; 

uint8_t bootjump_buff[BOOTJUMP_SIZE];
uint8_t ram_code_buff[RAM_CODE_SIZE];

/** Prints a string.
 *
 *  Convenient place to make the bootloader prints its stuff to SDSC or
 *  to the VDP.
 */
void print(const char* str){
  io_disable(IO_DISABLE_PERIPHERAL);
  sdsc_puts(str);
  io_enable(IO_ENABLE_PERIPHERAL);
}

/** Makes jumps to the memory copy of boot_jump.
 *
 *  This function takes the same parameters as boot_jump and exists only
 *  to receive the stack parameters for boot_jump and immediatly jump to
 *  its memory copy .
 */
void boot_call(uint16_t address, uint8_t io){
  (void) address; /*                                   */
  (void) io;      /* <-- Remove 'unused param' warning */
  __asm__("JP _bootjump_buff");
}

/** Bootloader jump code.
 *
 *  This function is copied into memory then executed from there. It must give
 *  the IO chip its final config like disable the BIOS, enable ROM madia 
 *  (cartridge, card, expansion, none) and jump to the new program entry point
 *  (Usually 0x0000 for ROMs).
 *
 *  Warning!
 *  Since this code is copied to ram, the resulting assembler must be
 *  relocatable and not dependant on ROM contents beyond BIOS deactivation, so
 *  function calls are forbidden and any kind of (non-local) variable access is
 *  discouraged.
 *
 *  @param address Address to jump to load the program, usually 0x0000.
 *  @param io      Value of the IO chip register.
 */
void boot_jump(uint16_t address, uint8_t io){
  (void) address; /*                                 */
  (void) io;      /* <-- Remove unused param warning */
  
  /*Set the IO chip register*/
  __io_chip = io;
  
  /* Jump to the selected address
   * Parameters are passed on the Stack:
   * SP --> Return addres (Low byte)
   * +1                   (High byte)
   * +2     arg: address (Low byte)
   * +3                  (High byte)
   * +4     arg: io
   *
   * Moving SP to point to address_L then returning should jump to the selected
   * ROM entry point
   */
  __asm__("POP HL"); /* <-- Jump address pointed by SP */
  __asm__("RET");    /* <-- Jump to ROM entry point */
  
  RAM_CODE_SIGNATURE; /* <-- Mark this function for RAM execution.*/
}

/** ROM entry point.*/
void main(){
  uint16_t i;
  uint8_t* boot_jump_bytes = (void*) boot_jump;
  
  /* --- Configure IO chip --- */
  io_set(IO_ENABLE_BIOS & IO_ENABLE_RAM & UNDEFINED_BIT_MASK);
  
  /* --- Clean bootloader buffers --- */
#ifdef CLEANUP_RAM
  for(i=0; i<BOOTJUMP_SIZE; ++i){
    bootjump_buff[i] = 0xFF;
  }
  for(i=0; i<RAM_CODE_SIZE; i++){
    ram_code_buff[i] = 0xFF;
  }
#endif
  
  /* --- Say Hello! --- */
  print(BOOT_HELLO);
  
  /* --- Do stuff --- */
  
  /* --- Prepare to chainload the next program --- */
  /* Copy boot_jump code to its buffer */
  for(i = 0; i<BOOTJUMP_SIZE; i++){
    bootjump_buff[i] = boot_jump_bytes[i];
  }
  
  /* Check for bootjump signature 'YAP!' */
  for(i = 0; i<(BOOTJUMP_SIZE-4); i++){
    if(
      (boot_jump_bytes[i+0] == 'Y') &&
      (boot_jump_bytes[i+1] == 'A') &&
      (boot_jump_bytes[i+2] == 'P') &&
      (boot_jump_bytes[i+3] == '!') 
      )
    {
      /*Signature found. Execute it!*/
      boot_call(0x000, 0xA8); /* <-- Chainload a ROM cartridge */
      break;
    }
  }
  
  /* Bootjump managed to return or bootjump not found.
   * Something bad happened D:
   */
   
  if(i == (BOOTJUMP_SIZE-4)){
    /*Failed to find bootloader signature*/
    print(BOOT_NO_SIGNATURE);
  }
  else{
    /*boot_jump returned. Failed to chainload program*/
    print(BOOT_NO_CHAINLOAD);
  }
   
  while(1){
    
  }
}
