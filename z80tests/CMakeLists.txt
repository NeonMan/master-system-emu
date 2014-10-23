# z80tests
project(z80tests)

# Setup CMake to build z80 binaries

# the name of the target operating system (None)
SET(CMAKE_SYSTEM_NAME Generic)
# which compilers to use for ASM
SET(CMAKE_ASM_COMPILER z80asm)
# Compiler command line
SET(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER> -o <OBJECT> <SOURCE>")
# The 'linker' just copies the objet
SET(CMAKE_ASM_LINK_EXECUTABLE "cp <OBJECTS> <TARGET>")

# Enable asm
enable_language (ASM)

add_executable(nop-loop.bin nop-loop.asm)
