cmake_minimum_required (VERSION 2.8)
project(test-roms C)
ENABLE_LANGUAGE(C)

#Configure SDCC to make z80 binaries
set(CMAKE_C_FLAGS "-mz80 --std-sdcc89 --xram-size 0x4000 --code-loc 0x0090 --data-loc 0xc000 --no-std-crt0 sms/crt0-sms.rel")
set(CMAKE_EXE_LINKER_FLAGS "-Wl -w")

ADD_DEFINITIONS(-DCOMPILER_ID="${CMAKE_C_COMPILER_ID}")

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
ADD_CUSTOM_TARGET(bootloader-bin ALL
        COMMAND makebin -s 32768 bootloader.ihx bootloader.sms
		DEPENDS bootloader
    )
ADD_CUSTOM_TARGET(console-test-bin ALL
        COMMAND makebin -s 32768 console-test.ihx console-test.sms
		DEPENDS console-test
    )
ADD_CUSTOM_TARGET(sys-info-bin ALL
        COMMAND makebin -s 32768 sys-info.ihx sys-info.sms
		DEPENDS sys-info
    )
