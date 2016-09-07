#ifndef __ROM_TOOLS_H
#define __ROM_TOOLS_H

/* --- rom-tools.c --- */
struct sega_header_s {
    char     tmr_sega[8];
    uint16_t checksum;
    uint8_t  code[3];
    uint8_t  version;
    uint8_t  region;
    uint8_t  size_type;
};
typedef struct sega_header_s sega_header_t;

struct sdsc_header_s {
    char     sdsc[4];
    uint8_t  version_h;
    uint8_t  version_l;
    uint8_t  date_day;
    uint8_t  date_month;
    uint8_t  date_year[2];
    char*    author;
    char*    name;
    char*    description;
};
typedef struct sdsc_header_s sdsc_header_t;

#define ROM_BUFFER_SIZE      1024
#define ROM_CODE_BUFFER_SIZE 256

#define ROM_CARTRIDGE 0b10101000
#define ROM_CARD_SLOT 0b11001000
#define ROM_EXPANSION 0b01101000
#define ROM_BIOS      0b11100000
#define ROM_RAM       0b00000000 /*<-- No, is not a joke u_u */

/*SEGA header constants*/
#define SEGA_HEADER_OFFSET 0x3f0
#define SH_CHECKSUM_OFFSET (SEGA_HEADER_OFFSET + 0x0A)
#define SH_PRODUCT_CODE_OFFSET (SEGA_HEADER_OFFSET + 0x0C)
#define SH_VERSION_OFFSET (SEGA_HEADER_OFFSET + 0x0E)
#define SH_REGION_OFFSET (SEGA_HEADER_OFFSET + 0x0F)
#define SH_SIZE_OFFSET (SEGA_HEADER_OFFSET + 0x0F)
#define SH_SIZE_8K  0x0A
#define SH_SIZE_16K 0x0B
#define SH_SIZE_32K 0x0C
#define SH_SIZE_48K 0x0D
#define SH_SIZE_64K 0x0E
#define SH_SIZE_128K 0x0f
#define SH_SIZE_256K 0x00
#define SH_SIZE_512K 0x01
#define SH_SIZE_1M   0x02

/*Rom size detection constants*/
#define ROM_SIZE_TEST_DEPTH 4 /*<-- How many mirror locations shall we test for each test.*/
#define ROM_SIZE_UNK  (0   )
#define ROM_SIZE_1K   (1<<0)
#define ROM_SIZE_2K   (1<<1)
#define ROM_SIZE_4K   (1<<2)
#define ROM_SIZE_8K   (1<<3)
#define ROM_SIZE_16K  (1<<4)
#define ROM_SIZE_32K  (1<<5)
#define ROM_SIZE_64K  (1<<6)
#define ROM_SIZE_128K (1<<7)
#define ROM_SIZE_256K (1<<8)
#define ROM_SIZE_512K (1<<9)
#define ROM_SIZE_1M   (1<<10)
#define ROM_SIZE_2M   (1<<11)
#define ROM_SIZE_4M   (1<<12)
#define ROM_SIZE_8M   (1<<13) /*<-- Max size with SEGA mapper */
#define ROM_SIZE_RES1 (1<<14)
#define ROM_SIZE_RES2 (1<<15)
static const char str_tmr_sega[8+1] = "TMR SEGA";
static const char str_sdsc[4+1] = "SDSC";

void     rom_info(uint8_t rom_media);
void     rom_boot(uint8_t rom_media);
uint8_t* rom_get_buffer();
uint16_t rom_checksum(uint8_t rom_media);
uint16_t rom_size(uint8_t rom_media);

sdsc_header_t* get_sdsc_header(uint8_t rom_media);
sega_header_t* get_sega_header(uint8_t rom_media);

#endif