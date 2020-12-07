# generated file
cmake_minimum_required( VERSION 2.8 )
include_directories(    ../include
                        ../include/${CMAKE_SYSTEM_NAME} )
find_package( SQLite3 )
add_definitions(-DBUILDNO="1")
list(APPEND SOURCES storage_init.c)
add_library(storage SHARED ${SOURCES})
install( TARGETS storage LIBRARY DESTINATION lib )
add_executable( test_storage test_storage.c )
target_link_libraries( test_storage PUBLIC storage
sqlite3
cnt
mem
io
fmt
err
 )
install( TARGETS test_storage RUNTIME DESTINATION bin )
