# z80
project(z80dasm)
set(CFILES z80_dasm.c)
set(HFILES z80_dasm.h z80_dasm_tables.h)

add_library(z80dasm ${CFILES} ${HFILES})

add_custom_command(OUTPUT  ${PROJECT_SOURCE_DIR}/z80_dasm_tables.h
                   COMMAND ${PYTHON_EXECUTABLE} ARGS ${PROJECT_SOURCE_DIR}/../scripts/z80_make_decoder_tables.py ${PROJECT_SOURCE_DIR}/z80_dasm_tables.csv ${PROJECT_SOURCE_DIR}/z80_dasm_tables.h od
                   MAIN_DEPENDENCY z80_dasm_tables.csv ../scripts/z80_make_decoder_tables.py
                   COMMENT "Generating disassembler look-up tables."
                   VERBATIM)
