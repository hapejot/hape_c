cmake_minimum_required( VERSION 3.18 )
# generated file
include_directories(    ../include
                        ../include/${CMAKE_SYSTEM_NAME} )
find_package( Check REQUIRED )
add_definitions(-DBUILDNO="TEST")
list(APPEND SOURCES test_suite.c )
add_library(testx SHARED ${SOURCES})
add_executable( test_main main.c )
add_executable( mem_test mem_test.c )

target_link_libraries( test_main PUBLIC 
            testx err mem fmt io cnt ${CHECK_LIBRARIES} 
            pthread m rt 
            subunit
            unwind-x86_64 dl bfd)
install( TARGETS test_main RUNTIME DESTINATION bin )

target_link_libraries( mem_test PUBLIC 
            testx err mem fmt io cnt ${CHECK_LIBRARIES} 
            pthread m rt 
            subunit
            unwind-x86_64 dl bfd)
install( TARGETS mem_test RUNTIME DESTINATION bin )
