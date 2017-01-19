project(test_z80)

#Z80 Macros testing
add_executable(zzz_test_z80_macros test_z80_macros.c)
target_link_libraries(zzz_test_z80_macros z80 unity glue sms_debug)
add_test(test_z80_macros zzz_test_z80_macros)

#Stack opcode testing (PUSH/POP/CALL/RET/RST)
add_executable(zzz_test_z80_stack test_z80_stack.c)
target_link_libraries(zzz_test_z80_stack z80 smsram unity glue sms_debug)
add_test(test_z80_stack zzz_test_z80_stack)

#16-bit ALU internals
add_executable(zzz_test_z80_alu16_internal test_z80_alu16_internal.c)
target_link_libraries(zzz_test_z80_alu16_internal z80 smsram unity glue sms_debug)
add_test(test_z80_alu16 zzz_test_z80_alu16_internal)

#8-bit ALU internals
add_executable(zzz_test_z80_alu8_internal test_z80_alu8_internal.c)
target_link_libraries(zzz_test_z80_alu8_internal z80 smsram unity glue sms_debug)
add_test(test_z80_alu8 zzz_test_z80_alu8_internal)

#16-bit LD
add_executable(zzz_test_z80_ld16 test_z80_ld16.c)
target_link_libraries(zzz_test_z80_ld16 z80 smsram unity glue sms_debug)
add_test(test_z80_ld16 zzz_test_z80_ld16)

#8-bit LD
add_executable(zzz_test_z80_ld8 test_z80_ld8.c)
target_link_libraries(zzz_test_z80_ld8 z80 smsram unity glue sms_debug)
add_test(test_z80_ld8 zzz_test_z80_ld8)

#Block copy
add_executable(zzz_test_z80_block test_z80_block.c)
target_link_libraries(zzz_test_z80_block z80 smsram unity glue sms_debug)
add_test(test_z80_block zzz_test_z80_block)

#Rotation operations
add_executable(zzz_test_z80_rotate test_z80_rotate.c)
target_link_libraries(zzz_test_z80_rotate z80 smsram unity glue sms_debug)
add_test(test_z80_rotate zzz_test_z80_rotate)

#Exchange
#add_executable(zzz_test_z80_exchange test_z80_exchange.c)
#target_link_libraries(zzz_test_z80_exchange z80 smsram unity)
#add_test(test_z80_exchange zzz_test_z80_exchange)

#Jumps
add_executable(zzz_test_z80_jumps test_z80_jumps.c)
target_link_libraries(zzz_test_z80_jumps z80 smsram unity glue sms_debug)
add_test(test_z80_jumps zzz_test_z80_jumps)