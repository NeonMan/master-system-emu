/* This file contains functions to dump ROM info to console.
 *
 */
#include "bios-menu.h"
#include "rom-tools.h"
#include <sms/console.h>

/*Dump information*/
void rom_info(uint8_t rom_media){
    uint8_t i;
    uint8_t is_sega;
    uint8_t is_sdsc;
    sega_header_t* header;
    sdsc_header_t* s_header;
    
    con_gotoxy(0, 4);
    
    /*Read ROM header*/
    header = get_sega_header(rom_media);
    
    /*Check for TMR SEGA*/
    is_sega = 1;
    for(i = 0; i<8; i++){
        if(header->tmr_sega[i] != str_tmr_sega[i]){
            is_sega = 0;
        }
    }
    
    /*Show SEGA header info*/
    con_put(" SEGA header: ");
    if(is_sega){
        con_put("Yes\n"); /*Dump header info*/
        
        /*Checksum*/
        {
            uint16_t checksum;
            con_put("   Checksum: ");
            con_put(" ****");
            /*Calculate sum*/
            checksum = rom_checksum(rom_media);
            con_relxy(-5,0);
            if(checksum == header->checksum){
                con_puth(checksum>>8);
                con_puth(checksum   );
                con_put(" OK");
            }
            else{
                con_puth(checksum>>8);
                con_puth(checksum   );
                con_put(" ERROR!\n");
                con_put("   Expected: ");
                con_puth(header->checksum>>8);
                con_puth(header->checksum   );
            }
            con_put("\n");
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
    {
        s_header = get_sdsc_header(rom_media);
        is_sdsc = 1;
        for(i = 0; i<4; i++){
            if(s_header->sdsc[i] != str_sdsc[i]){
                is_sdsc = 0;
            }
        }
    }

    
    /*Show SDSC header info*/
    con_put(" SDSC header: ");
    if(is_sdsc){
        con_put("Yes\n");
        
        /*Version*/
        {
            con_put("   Version: ");
            con_puth(s_header->version_h);
            con_putc('.');
            con_puth(s_header->version_l);
            con_put("\n");
        }
        
        /*Date*/
        {
            con_put("      Date: ");
            con_puth(s_header->date_day); con_putc('/');
            con_puth(s_header->date_month); con_putc('/');
            con_puth(s_header->date_year[1]);
            con_puth(s_header->date_year[0]);
            con_put("\n");
        }
        
        /*Author*/
        con_put("    Author: ToDo\n");
        /*Name*/
        con_put("      Name: ");
        /*
        if((s_header->name != (void*)0xFFFF) && (s_header->name != (void*)0x0000)){
            con_put("\n ");
            con_put((char*)(s_header->name));
            con_put("\n");
        }
        else{
            con_put("      Name: *None*\n");
        }
        */
        con_put("ToDo\n");
        /*Description*/
        con_put("      Desc: ToDo\n");
        
    }
    else{
        con_put("No\n");
    }
    con_put("\n");
    
    /*Calculate real ROM size*/
    con_put("   Real size: ***");
    {
        uint16_t r_size;
        r_size = rom_size(rom_media);
        con_relxy(-3,0);
        switch(r_size){
            case ROM_SIZE_1K:
            con_put("1K "); break;
            case ROM_SIZE_2K:
            con_put("2K "); break;
            case ROM_SIZE_4K:
            con_put("4K "); break;
            case ROM_SIZE_8K:
            con_put("8K "); break;
            case ROM_SIZE_16K:
            con_put("16K"); break;
            case ROM_SIZE_32K:
            con_put("32K"); break;
            case ROM_SIZE_64K:
            con_put("64K"); break;
            case ROM_SIZE_128K:
            con_put("128K"); break;
            case ROM_SIZE_256K:
            con_put("256K"); break;
            case ROM_SIZE_512K:
            con_put("512K"); break;
            case ROM_SIZE_1M:
            con_put("1M "); break;
            case ROM_SIZE_2M:
            con_put("2M "); break;
            case ROM_SIZE_4M:
            con_put("4M "); break;
            case ROM_SIZE_8M:
            con_put("8M "); break;
            default:
            con_put("Unknown"); break;
        }
        con_putc('\n');
    }
}