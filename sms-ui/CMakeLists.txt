#UI
project(sms-ui)

SET(FLTK_DIALOGS 
    ${PROJECT_SOURCE_DIR}/_DialogZ80.cxx
    ${PROJECT_SOURCE_DIR}/_DialogZ80.h
    ${PROJECT_SOURCE_DIR}/_DialogBreakpoint.cxx
    ${PROJECT_SOURCE_DIR}/_DialogBreakpoint.h
    ${PROJECT_SOURCE_DIR}/_DialogDebug.cxx
    ${PROJECT_SOURCE_DIR}/_DialogDebug.h
    )

SET(CFILES
    ${PROJECT_SOURCE_DIR}/DialogZ80.cxx
    ${PROJECT_SOURCE_DIR}/DialogZ80.h
    ${PROJECT_SOURCE_DIR}/DialogBreakpoint.cxx
    ${PROJECT_SOURCE_DIR}/DialogBreakpoint.h
    ${PROJECT_SOURCE_DIR}/DialogDebug.cxx
    ${PROJECT_SOURCE_DIR}/DialogDebug.h
	
	#Maybe make the debugger internals its own project
	${PROJECT_SOURCE_DIR}/zed/zedeighty.cxx
	${PROJECT_SOURCE_DIR}/zed/zedeighty.h
	${PROJECT_SOURCE_DIR}/zed/opcode.cxx
	${PROJECT_SOURCE_DIR}/zed/opcode.h
	)
	
#If using MSVC, add an icon to the executable.
if (MSVC)
  SET(VC_RESOURCE_FILES "resource.rc" "resource.h")
endif (MSVC)

if (MINGW)
	ADD_DEFINITIONS(-DWIN32)
endif()

add_executable(smsui smsui.cxx sms-emu.h ${VC_RESOURCE_FILES} ${CFILES} ${FLTK_DIALOGS})
target_link_libraries(smsui glue sms_debug_dump fltk smsrom smsio smsram psg z80 peripheral vdp sdsc sms_debug_dump savestate SDL2 SDL2main)

#Compile FL files
add_custom_command(OUTPUT 
                   ${PROJECT_SOURCE_DIR}/_DialogZ80.h
                   ${PROJECT_SOURCE_DIR}/_DialogZ80.cxx
                   COMMAND fluid ARGS
                   -c
                   -o ${PROJECT_SOURCE_DIR}/_DialogZ80.cxx
                   -h ${PROJECT_SOURCE_DIR}/_DialogZ80.h
                   ${PROJECT_SOURCE_DIR}/DialogZ80.fl
                   MAIN_DEPENDENCY "DialogZ80.fl"
                   COMMENT "Generating Z80 Dialog"
                   VERBATIM)

add_custom_command(OUTPUT 
                   ${PROJECT_SOURCE_DIR}/_DialogBreakpoint.h
                   ${PROJECT_SOURCE_DIR}/_DialogBreakpoint.cxx
                   COMMAND fluid ARGS
                   -c
                   -o ${PROJECT_SOURCE_DIR}/_DialogBreakpoint.cxx
                   -h ${PROJECT_SOURCE_DIR}/_DialogBreakpoint.h
                   ${PROJECT_SOURCE_DIR}/DialogBreakpoint.fl
                   MAIN_DEPENDENCY "DialogBreakpoint.fl"
                   COMMENT "Generating Breakpoint Dialog"
                   VERBATIM)
                   
add_custom_command(OUTPUT 
                   ${PROJECT_SOURCE_DIR}/_DialogDebug.h
                   ${PROJECT_SOURCE_DIR}/_DialogDebug.cxx
                   COMMAND fluid ARGS
                   -c
                   -o ${PROJECT_SOURCE_DIR}/_DialogDebug.cxx
                   -h ${PROJECT_SOURCE_DIR}/_DialogDebug.h
                   ${PROJECT_SOURCE_DIR}/DialogDebug.fl
                   MAIN_DEPENDENCY "DialogDebug.fl"
                   COMMENT "Generating Debugger Dialog"
                   VERBATIM)
