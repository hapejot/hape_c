# generated file
cmake_minimum_required( VERSION 2.8 )
include_directories(    ../include
                        ../include/${CMAKE_SYSTEM_NAME} )
add_definitions(-DBUILDNO="DUMMY")
list(APPEND SOURCES cnt_create.c)
list(APPEND SOURCES cnt.c)
add_library(cnt SHARED ${SOURCES})
add_executable( cnttst main.c )
target_link_libraries( cnttst PUBLIC cnt
mem
io
err
 )
install( TARGETS cnttst RUNTIME DESTINATION bin )
