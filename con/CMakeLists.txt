# generated file
cmake_minimum_required( VERSION 2.8 )
include_directories(    ../include
                        ../include/${CMAKE_SYSTEM_NAME} )
add_definitions(-DBUILDNO="TEST")
list(APPEND SOURCES con_move.c)
if(MSVC)
list(APPEND SOURCES con_getch_windows.c)
else()
list(APPEND SOURCES con_getch.c)
endif()
add_library(con SHARED ${SOURCES})
add_executable( contst main.c )
target_link_libraries( contst PUBLIC con
 )
install( TARGETS contst RUNTIME DESTINATION bin )
