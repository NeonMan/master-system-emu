cmake_minimum_required (VERSION 2.8)
project(test-roms C)
ENABLE_LANGUAGE(C)

#Configure SDCC to make z80 binaries
set(CMAKE_C_FLAGS "-mz80 --std-c89 --xram-size 0x4000 --code-loc 0x0090 --data-loc 0xc000 --no-std-crt0 sms/crt0-sms.rel")
set(CMAKE_EXE_LINKER_FLAGS "-Wl -w")

#crt0 for Sega Master System
# -- ToDo. Using a prebuilt one

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

ADD_EXECUTABLE(bootloader bootloader.c)
ADD_DEPENDENCIES(bootloader crt0-sms)
TARGET_LINK_LIBRARIES(bootloader sdsc io)

ADD_EXECUTABLE(vdp-test vdp-test.c)
ADD_DEPENDENCIES(vdp-test crt0-sms)
TARGET_LINK_LIBRARIES(vdp-test vdp)


#Make .bin ROMs from the .ihx files
ADD_CUSTOM_TARGET(sample-bin ALL
        COMMAND makebin -p sample.ihx sample.sms
		DEPENDS sample
    )
ADD_CUSTOM_TARGET(empty-bin ALL
        COMMAND makebin -p empty.ihx empty.sms
		DEPENDS empty
    )
ADD_CUSTOM_TARGET(ram-test-bin ALL
        COMMAND makebin -p ram-test.ihx ram-test.sms
		DEPENDS ram-test
    )
ADD_CUSTOM_TARGET(bootloader-bin ALL
        COMMAND makebin -s 16384 bootloader.ihx bootloader.sms
		DEPENDS bootloader
    )
ADD_CUSTOM_TARGET(vdp-test-bin ALL
        COMMAND makebin -p vdp-test.ihx vdp-test.sms
		DEPENDS vdp-test
    )
