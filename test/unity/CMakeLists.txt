#Unity testing framework
project(unity)
add_library(unity unity.c unity_fixture.c unity.h unity_fixture.h unity_fixture_internals.h unity_fixture_malloc_overrides.h unity_internals.h unity_sms_emu.h)
