project(sms-libs C)

#crt0
#add_custom_command(OUTPUT  crt0-sms.rel
#                   COMMAND sdasz80 ARGS -g -l -o -s crt0-sms.s ${PROJECT_SOURCE_DIR}/crt0-sms.s
#                   MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/crt0-sms.s
#                   COMMENT "Assembling crt0..."
#                   VERBATIM)

add_custom_command(OUTPUT crt0-sms.rel
        COMMAND sdasz80 ARGS -g -l -o -s crt0-sms.rel ${PROJECT_SOURCE_DIR}/crt0-sms.s
		COMMENT "Building crt0..."
    )
add_custom_target(crt0-sms DEPENDS crt0-sms.rel)
	
#Libraries
ADD_LIBRARY(sdsc sdsc.c sdsc.h)
ADD_LIBRARY(io io.c io.h)
ADD_LIBRARY(emu-hooks emu-hooks.c emu-hooks.h)
ADD_LIBRARY(vdp vdp.c vdp.h)
ADD_LIBRARY(console console.c console.h)
ADD_LIBRARY(bios-lib bios-lib.c bios-lib.h)
