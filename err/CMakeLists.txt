# generated file
cmake_minimum_required( VERSION 2.8 )
include_directories(    ../include
                        ../include/${CMAKE_SYSTEM_NAME} )
add_definitions(-DBUILDNO="TEST")
list(APPEND SOURCES err.c)
add_library(err SHARED ${SOURCES})
install( TARGETS err LIBRARY DESTINATION lib )
add_executable( errtst main.c )
target_link_libraries( errtst PUBLIC err
 )
install( TARGETS errtst RUNTIME DESTINATION bin )
