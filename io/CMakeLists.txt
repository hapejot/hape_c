# generated file
cmake_minimum_required( VERSION 3.18 )
include_directories(    ../include
                        ../include/${CMAKE_SYSTEM_NAME} )
add_definitions(-DBUILDNO="TEST")
list(APPEND SOURCES io.c)
add_library(io SHARED ${SOURCES})
install( TARGETS io LIBRARY DESTINATION lib )
add_executable( iotst main.c )
target_link_libraries( iotst PUBLIC io
err
mem
 )
install( TARGETS iotst RUNTIME DESTINATION bin )
install( FILES ../include/hape/io.h DESTINATION include/hape)
