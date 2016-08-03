# Save state
project(savestate)
add_library(savestate savestate.c savestate-save.c savestate-restore.c savestate.h)
