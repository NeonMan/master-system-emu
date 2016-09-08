project(relocatable-roms C)
ENABLE_LANGUAGE(C)

#Configure SDCC to make z80 binaries
set(CMAKE_C_FLAGS "-mz80 --std-sdcc89 --xram-size 0x1000 --code-loc 0xc060 --data-loc 0xd000 --no-std-crt0 \"${CMAKE_BINARY_DIR}/sms/crt0-sms-ram.rel\"")
set(CMAKE_EXE_LINKER_FLAGS "-Wl -w")

ADD_EXECUTABLE(rel-sample rel-sample.c)
ADD_DEPENDENCIES(rel-sample crt0-sms-ram)
TARGET_LINK_LIBRARIES(rel-sample console vdp)

#Make .bin ROMs from the .ihx files
ADD_CUSTOM_TARGET(rel-sample-bin ALL
        COMMAND makebin -s 53248 rel-sample.ihx rel-sample.sms
		DEPENDS rel-sample
    )
