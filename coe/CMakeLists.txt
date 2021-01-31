# generated file
cmake_minimum_required( VERSION 2.8 )
include_directories(    ../include
                        ../include/${CMAKE_SYSTEM_NAME} )
add_definitions(-DBUILDNO="TEST")
list(APPEND SOURCES coe.c )
add_library(coe SHARED ${SOURCES})
install( TARGETS coe LIBRARY DESTINATION lib )
add_executable( contst main.c )
target_link_libraries( contst PUBLIC coe
 )
install( TARGETS contst RUNTIME DESTINATION bin )
