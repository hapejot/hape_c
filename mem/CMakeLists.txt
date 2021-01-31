# generated file
cmake_minimum_required( VERSION 2.8 )
include_directories(    ../include
                        ../include/${CMAKE_SYSTEM_NAME} )
add_definitions(-DBUILDNO="TEST")
list(APPEND SOURCES mem.c)
add_library(mem SHARED ${SOURCES})
install( TARGETS mem LIBRARY DESTINATION lib )
add_executable( memtst main.c )
target_link_libraries( memtst PUBLIC mem
err
 )
install( TARGETS memtst RUNTIME DESTINATION bin )
