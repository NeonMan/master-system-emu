project(test_z80)

#Z80 Macros testing
add_executable(zzz_test_z80_macros test_z80_macros.c)
target_link_libraries(zzz_test_z80_macros z80 unity)
add_test(test_z80_macros zzz_test_z80_macros)

#Stack opcode testing (PUSH/POP/CALL/RET/RST)
add_executable(zzz_test_z80_stack test_z80_stack.c)
target_link_libraries(zzz_test_z80_stack z80 smsram unity)
add_test(test_z80_stack zzz_test_z80_stack)

#16-bit ALU
#add_executable(zzz_test_z80_alu16 test_z80_alu16.c)
#target_link_libraries(zzz_test_z80_alu16 z80 smsram unity)
#add_test(test_z80_alu16 zzz_test_z80_alu16)

#8-bit ALU
#add_executable(zzz_test_z80_alu8 test_z80_alu8.c)
#target_link_libraries(zzz_test_z80_alu8 z80 smsram unity)
#add_test(test_z80_alu8 zzz_test_z80_alu8)

#16-bit LD
add_executable(zzz_test_z80_ld16 test_z80_ld16.c)
target_link_libraries(zzz_test_z80_ld16 z80 smsram unity)
add_test(test_z80_ld16 zzz_test_z80_ld16)

#8-bit LD
add_executable(zzz_test_z80_ld8 test_z80_ld8.c)
target_link_libraries(zzz_test_z80_ld8 z80 smsram unity)
add_test(test_z80_ld8 zzz_test_z80_ld8)

#Block copy
add_executable(zzz_test_z80_block test_z80_block.c)
target_link_libraries(zzz_test_z80_block z80 smsram unity)
add_test(test_z80_block zzz_test_z80_block)

#Exchange
#add_executable(zzz_test_z80_exchange test_z80_exchange.c)
#target_link_libraries(zzz_test_z80_exchange z80 smsram unity)
#add_test(test_z80_exchange zzz_test_z80_exchange)