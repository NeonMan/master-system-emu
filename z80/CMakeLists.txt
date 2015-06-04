# z80
project(z80)
set(CFILES z80.c z80_dasm.c z80_decoder.c z80_debug.c z80_opcodes.c)
set(HFILES z80.h z80_dasm.h z80_debug.h z80_decoder.h z80_externs.h z80_macros.h z80_opcodes.h z80_register_lut.h z80_decoder_tables.h)

add_library(z80 ${CFILES} ${HFILES})

#Decoder LUTs
add_custom_command(OUTPUT  ${PROJECT_SOURCE_DIR}/z80_decoder_tables.h
                   COMMAND ${PYTHON_EXECUTABLE} ARGS ${PROJECT_SOURCE_DIR}/z80_make_decoder_tables.py ${PROJECT_SOURCE_DIR}/z80_decoder_tables.csv ${PROJECT_SOURCE_DIR}/z80_decoder_tables.h
                   MAIN_DEPENDENCY z80_decoder_tables.csv z80_make_decoder_tables.py
                   COMMENT "Generating decoder look-up tables."
                   VERBATIM)
