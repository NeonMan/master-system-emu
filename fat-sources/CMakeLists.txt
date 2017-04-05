#compiles some projects as a large source file
project(fat-files)

include_directories(../debug ../glue ../io ../peripheral ../psg ../ram ../rom ../savestate ../sdsc ../sms-ui ../sms-ui/zed ../vdp ../z80 ../z80dasm)


add_executable(fat-sms-ui fat-sms-ui.cxx)
target_link_libraries(fat-sms-ui sdl2 sdl2main fltk)