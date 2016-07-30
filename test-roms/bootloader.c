#include <stdint.h>
#include "sms/sms.h"
#include "sms/sdsc.h"
#include "sms/intv-dummy.h"
#include "sms/io.h"

#define RAM_CODE_SIZE 0x1000
#define BOOTJUMP_SIZE 0x0100

#define BOOT_HELLO        "Simple SMS bootloader."
#define BOOT_NO_SIGNATURE "Failed to find bootloader signature."
#define BOOT_NO_CHAINLOAD "Failed to start program."

#undef CLEANUP_RAM

uint8_t bootjump_buff[BOOTJUMP_SIZE];
uint8_t ram_code_buff[RAM_CODE_SIZE];

/** Prints a string.
 *
 *  Convenient place to make the bootloader prints its stuff to SDSC or
 *  to the screen.
 */
void print(const char* str){
  /*SDSC messages disabled since IO functions don't seem to work :/*/
  /*
  io_disable(IO_DISABLE_PERIPHERAL);
  sdsc_puts(str);
  io_enable(IO_ENABLE_PERIPHERAL);
  */
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
  (void) address;
  (void) io;
  /* Signature at the start of the boot_jump code so _I_ can easily find it on
   * the ROM dumps. It can be safely removed
   */
  __asm
      jr boot_signature_begin
      .ascii "YIP!"
    boot_signature_begin:
  __endasm;
  /* ----------------------------------------------------------------------- */
  
  /** @todo Implement the chainloader Address/Media functionality. */
  
  /* Just load the cartridge ROM for now */
  __io_chip = 0xA8;     /* <-- Enable RAM, pads and cartridge*/
  __asm__("JP 0x0000"); /* <-- Jump to ROM entry point */
  
  /* Bootloader code searches for this signature at the end of the boot_jump
   * code. Any code beyond this point is not guaranteed to be available.
   */
  __asm
      jr boot_signature_end
      .ascii "YAP!"
    boot_signature_end:
  __endasm;
  /* ----------------------------------------------------------------------- */
}

/** ROM entry point.*/
void main(){
  uint16_t i;
  uint8_t* boot_jump_bytes = (void*) boot_jump;
  /* --- Configure the SEGA mapper --- */
  __mapper_bank0 = 0x00;
  __mapper_bank1 = 0x01;
  __mapper_bank2 = 0x02;
  
  /* --- Clean bootloader buffers --- */
#ifdef CLEANUP_RAM
  for(i=0; i<BOOTJUMP_SIZE; ++i){
    bootjump_buff[i] = 0;
  }
  for(i=0; i<RAM_CODE_SIZE; i++){
    ram_code_buff[i] = 0;
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
      boot_call(0x000, 0xFF);
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