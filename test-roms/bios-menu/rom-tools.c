#include "bios-menu.h"
#include <sms/sms.h>

/*
 * Most boot code lifted from simple-bios.c
 * Read that file for a better explanation on how
 * ROMs are chainloaded.
 */

uint8_t rom_buffer [ROM_BUFFER_SIZE];
uint8_t code_buffer[ROM_CODE_BUFFER_SIZE];
 
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
  
/** Makes a jump to the chainload code.
 *
 *  This function takes the same parameters as boot_jump and exists only
 *  to receive the stack parameters for boot_jump and immediatly jump to
 *  its memory copy .
 *
 *  @param address reset verctor address, 0x0000 for regular ROMS
 *  @param io      value of the media selector register.
 */
static void bootcode_call(uint16_t address, uint8_t io){
  (void) address; /*                                   */
  (void) io;      /* <-- Remove 'unused param' warning */
  __asm__("JP _code_buffer");
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
 *  @param io      Value of the media selector register.
 */
static void boot_jump(uint16_t address, uint8_t io){
  (void) address; /* <-- Remove unused param warning */
  
  /*Set the media enable chip register*/
  sms_enable_port = io;
  
  /*Protect from accidental RAM execution putting a RST at base addr*/
  /*
  __asm
      ld hl, #0xc000
      ld (hl), #0xFF
  __endasm;
  */
  
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

void rom_boot(uint8_t rom_media){
    uint8_t i;
    /*Copy boot code to RAM*/
    for(i=0; i<ROM_CODE_BUFFER_SIZE; i++){
        code_buffer[i] = ((uint8_t*)boot_jump)[i];
    }
    
    /*Call bootcode*/
    if(rom_media != ROM_RAM)    {
        bootcode_call(0x0000, rom_media);
    }
    else{
        bootcode_call((uint16_t)rom_buffer, ROM_BIOS);
    }
}

uint8_t* rom_get_buffer(){
    return rom_buffer;
}
