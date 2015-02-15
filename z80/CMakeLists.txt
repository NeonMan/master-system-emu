# z80
project(z80)
set(CFILES z80.c z80_dasm.c z80_decoder.c z80_debug.c z80_opcodes.c z80_opcodes_DD_FD.c z80_opcodes_DDCB_FDCB.c)
add_library(z80 ${CFILES})
