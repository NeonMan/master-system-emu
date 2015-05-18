#UI
SET(FLTK_DIALOGS "DialogZ80.cxx")
SET(FLTK_DIALOGS ${FLTK_DIALOGS} "DialogBreakpoint.cxx")

#If using MSVC, add an icon to the executable.
if (MSVC)
  SET(VC_RESOURCE_FILES "resource.rc" )
endif (MSVC)

project(sms-ui)
add_executable(smsui smsui.cxx ${VC_RESOURCE_FILES} ${FLTK_DIALOGS})
target_link_libraries(smsui glue fltk smsrom smsio smsram psg z80 peripherial vdp sdsc SDL2 SDL2main)
