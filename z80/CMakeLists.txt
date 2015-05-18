# z80
project(z80)
set(CFILES z80.c z80_dasm.c z80_decoder.c z80_debug.c z80_opcodes.c z80_opcodes_DDCB_FDCB.c)
set(HFILES fake_z80.h z80.h z80_dasm.h z80_debug.h z80_decoder.h z80_externs.h z80_macros.h z80_opcodes.h z80_register_lut.h)
add_library(z80 ${CFILES} ${HFILES})
