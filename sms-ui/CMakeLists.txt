#UI
SET(FLTK_DIALOGS "DialogZ80.cxx" "DialogZ80.h")
SET(FLTK_DIALOGS ${FLTK_DIALOGS} "DialogBreakpoint.cxx" "DialogBreakpoint.h")

#If using MSVC, add an icon to the executable.
if (MSVC)
  SET(VC_RESOURCE_FILES "resource.rc" "resource.h")
endif (MSVC)

if (MINGW)
	ADD_DEFINITIONS(-DWIN32)
endif()

project(sms-ui)
add_executable(smsui smsui.cxx sms-emu.h ${VC_RESOURCE_FILES} ${FLTK_DIALOGS})
target_link_libraries(smsui glue fltk smsrom smsio smsram psg z80 peripherial vdp sdsc SDL2 SDL2main)
