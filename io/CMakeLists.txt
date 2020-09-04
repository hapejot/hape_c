# generated file
cmake_minimum_required( VERSION 2.8 )
include_directories(    ../include
                        ../include/${CMAKE_SYSTEM_NAME} )
add_definitions(-DBUILDNO="TEST")
list(APPEND SOURCES io_connect_fd.c io_seek.c io_read.c io_attr_get.c io_std_read.c io_buf_write.c io_std_close.c io_buf_read.c io_gets.c io_std_seek.c io_std_write.c io_connect_buf.c io_puts.c io_write.c io_ungetc.c io_close.c io_putc.c io_getc.c io_buf_expand.c io_connect_file.c io_buf_new.c)
list(APPEND SOURCES globals.c)
add_library(io SHARED ${SOURCES})
add_executable( iotst main.c )
target_link_libraries( iotst PUBLIC io
err
mem
 )
install( TARGETS iotst RUNTIME DESTINATION bin )
