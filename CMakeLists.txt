# Main
cmake_minimum_required (VERSION 2.8)

# --- Dependencies ---
# --- PSG
include_directories ("${PROJECT_SOURCE_DIR}/psg")
add_subdirectory (psg)
# --- RAM
include_directories ("${PROJECT_SOURCE_DIR}/ram")
add_subdirectory (ram)
# --- ROM
include_directories ("${PROJECT_SOURCE_DIR}/rom")
add_subdirectory (rom)
# --- z80
include_directories ("${PROJECT_SOURCE_DIR}/z80")
add_subdirectory (z80)

# --- The project executables ---
project (SMS-Emu)
# --- psg-play
add_executable (psg-play psg-play.c)
target_link_libraries (psg-play psg)
# --- test-rom
add_executable (test-rom test-rom.c)
target_link_libraries (test-rom rom)
