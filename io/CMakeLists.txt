#IO chip
project(smsio)
add_library(smsio io.c fake_io.h io.h io_externs.h)
