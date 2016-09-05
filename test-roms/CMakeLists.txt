cmake_minimum_required (VERSION 2.8)
project(test-roms C)
ENABLE_LANGUAGE(C)

#Configure SDCC to make z80 binaries
set(CMAKE_C_FLAGS "-mz80 --std-sdcc89 --xram-size 0x4000 --code-loc 0x0090 --data-loc 0xc000 --no-std-crt0 sms/crt0-sms.rel")
set(CMAKE_EXE_LINKER_FLAGS "-Wl -w")

ADD_DEFINITIONS(-DCOMPILER_ID="${CMAKE_C_COMPILER_ID}")
ADD_DEFINITIONS(-DEXTENDED_ASCII)

#Libraries
ADD_SUBDIRECTORY(sms)

#ROM "executables" ;)
ADD_EXECUTABLE(empty empty.c)
ADD_DEPENDENCIES(empty crt0-sms)

ADD_EXECUTABLE(sample sample.c)
ADD_DEPENDENCIES(sample crt0-sms)
TARGET_LINK_LIBRARIES(sample sdsc emu-hooks )

ADD_EXECUTABLE(ram-test ram-test.c)
ADD_DEPENDENCIES(ram-test crt0-sms)
TARGET_LINK_LIBRARIES(ram-test sdsc)

ADD_EXECUTABLE(simple-bios simple-bios.c)
ADD_DEPENDENCIES(simple-bios crt0-sms)
TARGET_LINK_LIBRARIES(simple-bios sdsc io)

ADD_EXECUTABLE(menu-bios menu-bios.c)
ADD_DEPENDENCIES(menu-bios crt0-sms)
TARGET_LINK_LIBRARIES(menu-bios sms vdp console io)

ADD_EXECUTABLE(console-test console-test.c)
ADD_DEPENDENCIES(console-test crt0-sms)
TARGET_LINK_LIBRARIES(console-test vdp console)

ADD_EXECUTABLE(sys-info sys-info.c)
ADD_DEPENDENCIES(sys-info crt0-sms)
TARGET_LINK_LIBRARIES(sys-info sms vdp console)

#Make .bin ROMs from the .ihx files
ADD_CUSTOM_TARGET(sample-bin ALL
        COMMAND makebin -s 32768 sample.ihx sample.sms
		DEPENDS sample
    )
ADD_CUSTOM_TARGET(empty-bin ALL
        COMMAND makebin -s 32768 empty.ihx empty.sms
		DEPENDS empty
    )
ADD_CUSTOM_TARGET(ram-test-bin ALL
        COMMAND makebin -s 32768 ram-test.ihx ram-test.sms
		DEPENDS ram-test
    )
ADD_CUSTOM_TARGET(simple-bios-bin ALL
        COMMAND makebin -s 32768 simple-bios.ihx simple-bios.sms
		DEPENDS simple-bios
    )
ADD_CUSTOM_TARGET(menu-bios-bin ALL
        COMMAND makebin -s 32768 menu-bios.ihx menu-bios.sms
		DEPENDS menu-bios
    )
ADD_CUSTOM_TARGET(console-test-bin ALL
        COMMAND makebin -s 32768 console-test.ihx console-test.sms
		DEPENDS console-test
    )
ADD_CUSTOM_TARGET(sys-info-bin ALL
        COMMAND makebin -s 32768 sys-info.ihx sys-info.sms
		DEPENDS sys-info
    )
