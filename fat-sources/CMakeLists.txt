#compiles some projects as a large source file
project(fat-files)

include_directories(../debug ../glue ../io ../peripheral ../psg ../ram ../rom ../savestate ../sdsc ../sms-ui ../sms-ui/zed ../vdp ../z80 ../z80dasm)


add_executable(fat-smsui fat-sms-ui.cxx)
target_link_libraries(fat-smsui SDL2 SDL2main fltk)
add_dependencies(fat-smsui smsui)
