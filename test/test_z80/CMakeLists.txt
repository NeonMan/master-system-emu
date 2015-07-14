project(test_z80)

#Z80 Macros testing
add_executable(zzz_test_z80_macros test_z80_macros.c test_z80.h)
target_link_libraries(zzz_test_z80_macros z80 unity)
add_test(test_z80_macros zzz_test_z80_macros)

#Stack opcode testing (PUSH/POP/CALL/RET/RST)
add_executable(zzz_test_z80_stack test_z80_stack.c test_z80.h)
target_link_libraries(zzz_test_z80_stack z80 smsram unity)
add_test(test_z80_stack zzz_test_z80_stack)

#16-bit ALU
add_executable(zzz_test_z80_alu16 test_z80_alu16.c test_z80.h)
target_link_libraries(zzz_test_z80_alu16 z80 smsram unity)
add_test(test_z80_alu16 zzz_test_z80_alu16)

#8-bit ALU
add_executable(zzz_test_z80_alu8 test_z80_alu8.c test_z80.h)
target_link_libraries(zzz_test_z80_alu8 z80 smsram unity)
add_test(test_z80_alu8 zzz_test_z80_alu8)

#16-bit LD
add_executable(zzz_test_z80_ld16 test_z80_ld16.c test_z80.h)
target_link_libraries(zzz_test_z80_ld16 z80 smsram unity)
add_test(test_z80_ld16 zzz_test_z80_ld16)

#8-bit LD
add_executable(zzz_test_z80_ld8 test_z80_ld8.c test_z80.h)
target_link_libraries(zzz_test_z80_ld8 z80 smsram unity)
add_test(test_z80_ld8 zzz_test_z80_ld8)
