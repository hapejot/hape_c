# generated file
cmake_minimum_required( VERSION 2.8 )
include_directories(    ../include
                        ../include/${CMAKE_SYSTEM_NAME} )
add_definitions(-DBUILDNO="TEST")
list(APPEND SOURCES fmt_cvt_number.c fmt_cvt_string.c fmt_vfmt.c fmt_format.c)
list(APPEND SOURCES globals.c)
add_library(fmt SHARED ${SOURCES})
add_executable( fmttst main.c )
target_link_libraries( fmttst PUBLIC fmt
io
err
mem
 )
install( TARGETS fmttst RUNTIME DESTINATION bin )
