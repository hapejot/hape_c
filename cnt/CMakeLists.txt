# generated file
cmake_minimum_required( VERSION 2.8 )
find_package( lemon )
find_package( re2c )
include_directories(    ../include
                        .
                        ../include/${CMAKE_SYSTEM_NAME} )
add_definitions(-DBUILDNO="DUMMY")
list(APPEND SOURCES cnt.c)
list(APPEND SOURCES cnt_json_parser.c)

set( JSON_IMP_RE ${PROJECT_SOURCE_DIR}/cnt/cnt_json_imp.re)
set( PARSER      ${PROJECT_SOURCE_DIR}/cnt/cnt_json_parser.y)

set( JSON_IMP ${CMAKE_CURRENT_BINARY_DIR}/cnt_json_imp.c)
set( PARSER_C ${CMAKE_CURRENT_BINARY_DIR}/cnt_json_parser.c)

list(APPEND SOURCES ${JSON_IMP} ${PARSER_C})

add_library(cnt SHARED ${SOURCES} )
add_executable( cnttst main.c )
target_link_libraries( cnttst PUBLIC cnt mem io err)
install( TARGETS cnttst RUNTIME DESTINATION bin )
install( TARGETS cnt LIBRARY DESTINATION lib PUBLIC_HEADER DESTINATION include)
install( FILES ../include/hape/cnt.h DESTINATION include/hape)
# install( EXPORT hape_test DESTINATION hape/lib )
add_custom_command( OUTPUT ${JSON_IMP}
                    COMMAND ${RE2C} -o ${JSON_IMP} ${JSON_IMP_RE}
                    DEPENDS ${JSON_IMP_RE} )

add_custom_command( OUTPUT ${PARSER_C}
                    COMMAND ${LEMON} -d${CMAKE_CURRENT_BINARY_DIR} -T${PROJECT_SOURCE_DIR}/lempar.c ${PARSER} 
                    DEPENDS ${PARSER} )
