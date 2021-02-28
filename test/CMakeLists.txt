# generated file
include_directories(    ../include
                        ../include/${CMAKE_SYSTEM_NAME} )
find_package( Check REQUIRED )
add_definitions(-DBUILDNO="TEST")
list(APPEND SOURCES test_suite.c )
add_library(testx ${SOURCES})
add_executable( test_main main.c )
target_link_libraries( test_main PUBLIC testx err mem fmt io cnt ${CHECK_LIBRARIES} pthread m rt subunit)
install( TARGETS test_main RUNTIME DESTINATION bin )
