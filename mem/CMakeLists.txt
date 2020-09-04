# generated file
cmake_minimum_required( VERSION 2.8 )
include_directories(    ../include
                        ../include/${CMAKE_SYSTEM_NAME} )
add_definitions(-DBUILDNO="TEST")
list(APPEND SOURCES mem_alloc_g.c mem_globals.c mem_arena_alloc.c mem_arena_free.c mem_arena_dispose.c mem_arena_calloc.c mem_arena_new.c mem_free_g.c)
list(APPEND SOURCES print_hex_dump.c)
add_library(mem SHARED ${SOURCES})
add_executable( memtst main.c )
target_link_libraries( memtst PUBLIC mem
err
 )
install( TARGETS memtst RUNTIME DESTINATION bin )
