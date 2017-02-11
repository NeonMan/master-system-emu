#UI
project(sms-ui)

SET(FLTK_DIALOGS 
    ${PROJECT_SOURCE_DIR}/_DialogDebug.cxx
    ${PROJECT_SOURCE_DIR}/_DialogDebug.h
    )

SET(CFILES
    ${PROJECT_SOURCE_DIR}/DialogDebug.cxx
    ${PROJECT_SOURCE_DIR}/DialogDebug.h
	${PROJECT_SOURCE_DIR}/WidgetVdp.cxx
	${PROJECT_SOURCE_DIR}/WidgetVdp.h
	
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
