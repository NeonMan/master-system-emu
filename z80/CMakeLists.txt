# z80
project(z80)
set(CFILES z80.c z80_decoder.c z80_debug.c z80_opcodes.c z80_opcodes_ld.c z80_opcodes_jumps.c z80_opcodes_alu8.c z80_opcodes_alu16.c z80_opcodes_bit.c z80_opcodes_rotshift.c)
set(HFILES z80.h z80_debug.h z80_decoder.h z80_externs.h z80_macros.h z80_opcodes.h z80_register_lut.h z80_decoder_tables.h)

add_library(z80 ${CFILES} ${HFILES})

#Some debug stuff requires access to memory
#release z80 requires no additional libraries
target_link_libraries(z80 smsram smsio z80dasm)

#Decoder LUTs
add_custom_command(OUTPUT  ${PROJECT_SOURCE_DIR}/z80_decoder_tables.h
                   COMMAND ${PYTHON_EXECUTABLE} ARGS ${PROJECT_SOURCE_DIR}/../scripts/z80_make_decoder_tables.py ${PROJECT_SOURCE_DIR}/z80_decoder_tables.csv ${PROJECT_SOURCE_DIR}/z80_decoder_tables.h
                   MAIN_DEPENDENCY z80_decoder_tables.csv ../scripts/z80_make_decoder_tables.py
                   COMMENT "Generating decoder look-up tables."
                   VERBATIM)
                   