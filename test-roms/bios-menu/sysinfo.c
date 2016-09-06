#include "bios-menu.h"
#include <sms/console.h>
#include <sms/sms.h>

#define CRT_SIGNATURE_ADDRESS 0x0050

static void build_info(){
    /*Compiler*/
    con_put("     cc: ");
#ifdef COMPILER_ID 
    con_put(COMPILER_ID "\n");
#else
    con_put("Unknown\n");
#endif
    
    /*Last Commit ID*/
    con_put("    Rev: ");
#ifdef COMMIT_ID
    con_put(COMMIT_ID);
#else
    con_put("Unknown");
#endif
    con_put("\n");
    
    /*Date & Time*/
    con_put("   Date: ");
    con_put(__DATE__ " " __TIME__);
    con_put("\n");
}

static void region_info(){
    con_put("    Reg: ");
    if(sms_region() == SMS_REGION_JAPAN){
        con_put("Japan");
    }
    else{
        con_put("Export");
    }
    con_put("\n");
}

void show_sysinfo(){
    con_put(" Build info:\n");
    build_info();
    con_put("\n");
    con_put(" Console info:\n");
    region_info();
}
