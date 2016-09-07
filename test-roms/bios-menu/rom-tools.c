#include "bios-menu.h"
#include <sms/console.h>
#include <sms/sms.h>

struct sega_header_s {
    char     tmr_sega[8];
    uint16_t checksum;
    uint8_t  code[3];
    uint8_t  version;
    uint8_t  region;
    uint8_t  size_type;
};
typedef struct sega_header_s sega_header_t;

uint8_t rom_buffer [ROM_BUFFER_SIZE];
uint8_t code_buffer[ROM_CODE_BUFFER_SIZE];
 
/**
 * @brief Signature mark for RAM loadable code.
 * Bootloader code searches for this signature at the end of the RAM-loaded
 * function. Any code beyond this point is not guaranteed to be available.
 */
#define RAM_CODE_SIGNATURE \
  __asm \
      jr LABEL \
      .ascii "YAP!" \
    LABEL: \
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

/** Copy bootcode to RAM then execute it. */
void rom_boot(uint8_t rom_media){
    uint16_t i;
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

/** ROM read routine.
 *
 *  This function will copy a 1K block of data from the selected media. This
 *  function is intended to be run from RAM since the BIOS ROM wont be
 *  available during this operation.
 */
void _media_read(uint16_t block_index, uint8_t media){
    uint8_t  old_media;
    uint8_t  bank;
    uint16_t address;
    uint16_t i;
    
    old_media = media;
    
    /*Enable external media*/
    sms_enable_port = media;
    
    /*Calculate bank and address, avoid strange operations like modulus     */
    /*since this code shall run from RAM without any function calls.        */
    /*Make sure this function's size is below ROM_CODE_BUFFER_SIZE, check   */
    /*the .map file and fix the function or buffer sizeaccordingly.         */
    /*Undefined behaviour you say? Yeah, fuck you too ;3                    */
    /*  ------ as of this writting (2016-09-06) size is 164B         ------ */
    
    /*Calculating bank index:                                               */
    /*ROM is divided into 16K banks   0100 0000 0000 0000                   */
    /*Each 'read' slot is 1K          0000 0100 0000 0000                   */
    /*So, for each 16 slots (0001 0000), we increment bank index by one, up */
    /*to bank 255. So the bank index is block_index>>4 truncated to 8bits   */
    bank = (uint8_t)(block_index >> 4);
    
    /*Calculate offset within the bank*/
    /*The lower 4 bits of the block index tell us which of the 16 1K lines  */
    /*from the selected bank we shall copy.*/
    address = block_index & 0b00001111;
    /*Multiply by 1024 (1K) using shifting */
    address = address << 10;
    
    /*Set SEGA mapper SLOT2 to the selected bank*/
    *sms_mapper_slot2 = bank;
    
    /*copy 1K of data from it*/
    for(i = 0; i<1024; i++){
        rom_buffer[i] = (((uint8_t*)SMS_SLOT2_BASE_ADDRESS) + address)[i];
    }
    
    /*Restore mapper bank to the default value (2)*/
    *sms_mapper_slot2 = 2;
    
    /*Re-enable BIOS*/
    sms_enable_port = ROM_BIOS;
}

void media_read(uint16_t block_index, uint8_t media){
    /*Copy media_read code to RAM*/
    uint16_t i;
    for(i=0; i<ROM_CODE_BUFFER_SIZE; i++){
        code_buffer[i] = ((uint8_t*)_media_read)[i];
    }
    bootcode_call(block_index, media);
}

/*Extract SEGA header info*/
#define SEGA_HEADER_OFFSET 0x3f0
#define SH_CHECKSUM_OFFSET (SEGA_HEADER_OFFSET + 0x0A)
#define SH_PRODUCT_CODE_OFFSET (SEGA_HEADER_OFFSET + 0x0C)
#define SH_VERSION_OFFSET (SEGA_HEADER_OFFSET + 0x0E)
#define SH_REGION_OFFSET (SEGA_HEADER_OFFSET + 0x0F)
#define SH_SIZE_OFFSET (SEGA_HEADER_OFFSET + 0x0F)
static const char tmr_sega[8+1] = "TMR SEGA";
static sega_header_t tmp_sega_header;
static sega_header_t* get_sega_header(uint8_t rom_media){
    
    uint8_t i;
    /*Read ROM header*/
    /*It is located at the (1K) line number 31*/
    media_read(31, rom_media);
    
    /*Copy TMR SEGA (8B)*/
    for(i=0;i<8;i++){
        tmp_sega_header.tmr_sega[i] = rom_buffer[SEGA_HEADER_OFFSET + i];
    }
    
    /*Copy checksum (2B)*/
    tmp_sega_header.checksum = 
            rom_buffer[SH_CHECKSUM_OFFSET] +
            (((uint16_t)rom_buffer[SH_CHECKSUM_OFFSET + 1]) << 8)
            ;
    
    /*Copy product code (3B, 20bit)*/
    tmp_sega_header.code[0] = rom_buffer[SH_PRODUCT_CODE_OFFSET];
    tmp_sega_header.code[1] = rom_buffer[SH_PRODUCT_CODE_OFFSET + 1];
    tmp_sega_header.code[2] = rom_buffer[SH_PRODUCT_CODE_OFFSET + 2] & 0xF0;
    
    /*Version*/
    tmp_sega_header.version = rom_buffer[SH_VERSION_OFFSET];
    
    /*Region*/
    tmp_sega_header.region = (rom_buffer[SH_REGION_OFFSET] >> 4) & 0x0F;
    
    /*Rom size*/
    tmp_sega_header.size_type = (rom_buffer[SH_SIZE_OFFSET]) & 0x0F;
    
    return &tmp_sega_header;
}

/*Dump information*/
static const char sdsc[4+1] = "SDSC";
void rom_info(uint8_t rom_media){
    uint8_t i;
    uint8_t is_sega;
    uint8_t is_sdsc;
    sega_header_t* header;
    
    con_gotoxy(0, 4);
    
    /*Read ROM header*/
    header = get_sega_header(rom_media);
    
    /*Check for TMR SEGA*/
    is_sega = 1;
    for(i = 0; i<8; i++){
        if(header->tmr_sega[i] != tmr_sega[i]){
            is_sega = 0;
        }
    }
    
    /*Show SEGA header info*/
    con_put(" SEGA header: ");
    if(is_sega){
        con_put("Yes\n"); /*Dump header info*/
        
        /*Checksum*/
        {
            con_put("   Checksum: ");
            con_puth(header->checksum>>8);
            con_puth(header->checksum   );
            con_put(" (OK?)\n");
        }
        
        /*Product code*/
        {
            con_put("       Code: ");
            con_puth(header->code[0]);
            con_puth(header->code[1]);
            con_puth(header->code[2]);
            con_put("\n");
        }
        
        /*Version*/
        {
            con_put("    Version: ");
            con_puth(header->version);
            con_put("\n");
        }
        
        /*Region*/
        {
            con_put("     Region: ");
            switch(header->region){
                case 3:
                con_put("Japan"); break;
                case 4:
                con_put("Export"); break;
                case 5:
                con_put("Japan (GG)"); break;
                case 6:
                con_put("Export (GG)"); break;
                case 7:
                con_put("Intl. (GG)"); break;
                default:
                con_put("Unknown ");
                con_puth(header->region);
                break;
            }
            con_put("\n");
        }
        
        /*ROM size*/
        {
            con_put("       Size: ");
            switch(header->size_type){
                case 0x00:
                con_put("256KB"); break;
                case 0x01:
                con_put("512KB"); break;
                case 0x02:
                con_put("1MB (!)"); break;
                case 0x0A:
                con_put("8K"); break;
                case 0x0B:
                con_put("16K"); break;
                case 0x0C:
                con_put("32K"); break;
                case 0x0D:
                con_put("48K (!)"); break;
                case 0x0E:
                con_put("64K"); break;
                case 0x0F:
                con_put("128K"); break;
                default:
                con_put("Unknown ");
                con_puth(header->size_type);
                break;
            }
            con_put("\n");
        }
        
    }
    else{
        con_put("No\n");
    }
    con_put("\n");
    
    /*Check for SDSC*/
    is_sdsc = 1;
    for(i = 0; i<4; i++){
        if(rom_buffer[0x3e0 + i] != sdsc[i]){
            is_sdsc = 0;
        }
    }
    
    /*Show SDSC header info*/
    con_put(" SDSC header: ");
    if(is_sdsc){
        con_put("Yes\n");
        
        /*Version*/
        {
            con_put("   Version: ");
            con_puth(rom_buffer[0x3e0 + 0x4]);
            con_putc('.');
            con_puth(rom_buffer[0x3e0 + 0x5]);
            con_put("\n");
        }
        
        /*Date*/
        {
            con_put("      Date: ");
            con_puth(rom_buffer[0x3e0 + 0x6]); con_putc('/');
            con_puth(rom_buffer[0x3e0 + 0x7]); con_putc('/');
            con_puth(rom_buffer[0x3e0 + 0x9]);
            con_puth(rom_buffer[0x3e0 + 0x8]);
            con_put("\n");
        }
        
        /*Author*/
        con_put("    Author: ToDo\n");
        /*Name*/
        con_put("      Name: ToDo\n");
        /*Description*/
        con_put("      Desc: ToDo\n");
        
    }
    else{
        con_put("No\n");
    }
    con_put("\n");
}

/*test-checksum*/

uint8_t* rom_get_buffer(){
    return rom_buffer;
}
