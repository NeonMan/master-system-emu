# Save state
project(savestate)
add_library(savestate savestate.c savestate.h)

add_executable(dump-state dump-state.c)
target_link_libraries(dump-state savestate smsram smsrom smsio peripheral z80)
