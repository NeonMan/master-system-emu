#UI
project(sms-ui)

SET(FLTK_DIALOGS "DialogZ80.cxx" "DialogZ80.h")
SET(FLTK_DIALOGS ${FLTK_DIALOGS} "DialogBreakpoint.cxx" "DialogBreakpoint.h")

#If using MSVC, add an icon to the executable.
if (MSVC)
  SET(VC_RESOURCE_FILES "resource.rc" "resource.h")
endif (MSVC)

if (MINGW)
	ADD_DEFINITIONS(-DWIN32)
endif()

add_executable(smsui smsui.cxx sms-emu.h ${VC_RESOURCE_FILES} ${FLTK_DIALOGS})
target_link_libraries(smsui glue fltk smsrom smsio smsram psg z80 peripherial vdp sdsc SDL2 SDL2main)

#Compile FL files
add_custom_command(OUTPUT "DialogZ80.h" "DialogZ80.cxx"
                   COMMAND fluid ARGS
                   -c
                   -o DialogZ80.cxx
                   -h ${PROJECT_SOURCE_DIR}/DialogZ80.h
                   ${PROJECT_SOURCE_DIR}/DialogZ80.fl
                   MAIN_DEPENDENCY "DialogZ80.fl"
                   COMMENT "Generating Z80 Dialog"
                   VERBATIM)

add_custom_command(OUTPUT "DialogBreakpoint.h" "DialogBreakpoint.cxx"
                   COMMAND fluid ARGS
                   -c
                   -o DialogBreakpoint.cxx
                   -h ${PROJECT_SOURCE_DIR}/DialogBreakpoint.h
                   ${PROJECT_SOURCE_DIR}/DialogBreakpoint.fl
                   MAIN_DEPENDENCY "DialogBreakpoint.fl"
                   COMMENT "Generating Breakpoint Dialog"
                   VERBATIM)
