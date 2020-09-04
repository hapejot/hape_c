# generated file
cmake_minimum_required( VERSION 2.8 )
include_directories(    ../include
                        ../include/${CMAKE_SYSTEM_NAME} )
add_definitions(-DBUILDNO="DUMMY")
list(APPEND SOURCES cnt_dump.c cnt_split.c cnt_exp_par.c cnt_get_dim.c cnt_cell_cmp.c cnt_from_csv.c cnt_next_name.c cnt_release.c cnt_get_val.c cnt_imp.c cnt_first_name.c cnt_get_idx_val.c cnt_check.c cnt_exp_json.c cnt_copy.c cnt_create.c cnt_copy_row.c cnt_set_val.c cnt_exp.c cnt_set_idx_val.c cnt_filter.c cnt_split_name.c)
add_library(cnt SHARED ${SOURCES})
add_executable( cnttst main.c )
target_link_libraries( cnttst PUBLIC cnt
mem
fmt
io
err
 )
install( TARGETS cnttst RUNTIME DESTINATION bin )
add_executable( cntrecode recode.c )
target_link_libraries( cntrecode PUBLIC cnt
mem
fmt
io
err
 )
install( TARGETS cntrecode RUNTIME DESTINATION bin )
add_executable( cntxtrct xtrct.c )
target_link_libraries( cntxtrct PUBLIC cnt
mem
fmt
io
err
 )
install( TARGETS cntxtrct RUNTIME DESTINATION bin )
add_executable( cntfmt cntfmt_main.c )
target_link_libraries( cntfmt PUBLIC cnt
mem
fmt
io
err
 )
install( TARGETS cntfmt RUNTIME DESTINATION bin )
