#Glue. Platform-dependent code goes here.
project(glue)
add_library(glue glue.c glue.h)
target_link_libraries(glue)
