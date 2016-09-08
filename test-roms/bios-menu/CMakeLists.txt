project(bios C)

# BIOS menu ROM
ADD_EXECUTABLE(bios-menu 
        bios-menu.c
        bm-state.c
        system-info.c
        rom-tools.c
        rom-info.c
        )
ADD_DEPENDENCIES(bios-menu crt0-sms)
TARGET_LINK_LIBRARIES(bios-menu crc16-ccitt sms vdp console io)

ADD_CUSTOM_TARGET(bios-menu-bin ALL
        COMMAND makebin -s 32768 bios-menu.ihx bios-menu.sms
		DEPENDS bios-menu
    )