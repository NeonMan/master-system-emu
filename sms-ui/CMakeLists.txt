#UI
SET(FLTK_DIALOGS "DialogZ80.cxx")
SET(FLTK_DIALOGS ${FLTK_DIALOGS} "DialogBreakpoint.cxx")


project(sms-ui)
add_executable(smsui smsui.cxx ${FLTK_DIALOGS})
target_link_libraries(smsui glue fltk smsrom smsio smsram psg z80 peripherial vdp sdsc SDL2 SDL2main)
