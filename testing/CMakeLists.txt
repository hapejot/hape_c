# generated file
cmake_minimum_required( VERSION 2.8 )
include_directories(    ../include
                        ../include/${CMAKE_SYSTEM_NAME} )
find_package( Check )
add_definitions(-DBUILDNO="TEST")
list(APPEND SOURCES test_suite.c test_max.c test_str_fix.c)
add_library(testing SHARED ${SOURCES})
add_executable( test_main main.c )
target_link_libraries( test_main PUBLIC testing
str_fix
${CHECK_LIBRARIES}
pthread
m
rt
subunit
 )
install( TARGETS test_main RUNTIME DESTINATION bin )
add_executable( test_max test_max.c )
target_link_libraries( test_max PUBLIC testing
str_fix
${CHECK_LIBRARIES}
pthread
m
rt
subunit
 )
install( TARGETS test_max RUNTIME DESTINATION bin )
