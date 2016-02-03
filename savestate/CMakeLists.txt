# Save state
project(savestate)
add_library(savestate savestate.c savestate-save.c savestate-restore.c savestate.h)

add_executable(dump-state dump-state.c)
target_link_libraries(dump-state savestate smsram smsrom smsio peripheral psg z80)
