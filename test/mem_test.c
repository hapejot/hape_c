#include <stdio.h>
#include <stdbool.h>
#include <hape/cnt.h>
#include <hape/err.h>
#include <bfd.h>
#include <malloc.h>
#include <stdlib.h>

#include "../cnt/cnt_int.h"

/* cnt.c */
CNT_CELL cell_create( MEM_ARENA, CNT_IDX col, CNT_IDX row );
bool gen_find( CNT_VECTOR_PAGE p, CMP f, void *elem, CNT_IDX * idx );

int col_compare( void *a, void *b );
CNT_COL col_create( MEM_ARENA a, int pos, char *name );
CNT_IDX col_find( CNT_VECTOR_PAGE, char * );

CNT_IDX cnt_columns( CNT cnt );
CNT_IDX cnt_lines( CNT cnt );
CNT_COL_NAME cnt_colname( CNT cnt, CNT_IDX col );
CNT_DATA *data_cpy( MEM_ARENA a, CNT_DATA * data );
CNT_COL cnt_column( CNT cnt, char *name );
void cnt_set_val_b( CNT cnt, CNT_COL_NAME name, CNT_BYTES data );
void cnt_set_idx_b( CNT cnt, CNT_COL_NAME name, CNT_IDX row, CNT_BYTES data );
CNT_IDX cell_pos( CNT cnt, CNT_IDX row, CNT_IDX col );
int pair_compare( CNT_IDX x1, CNT_IDX x2, CNT_IDX y1, CNT_IDX y2 );
CNT_IDX cell_find( CNT_VECTOR_PAGE page, CNT_IDX row, CNT_IDX col );
CNT_BYTES cnt_idx_b( CNT cnt, CNT_COL_NAME name, CNT_IDX row );
void cell_page_insert( CNT_VECTOR_PAGE p, CNT_IDX idx, CNT_CELL cell );
void cnt_set_idx_d( CNT cnt, CNT_COL_NAME name, CNT_IDX row,
                    CNT_DATA * data );
void cnt_set_val_d( CNT cnt, CNT_COL_NAME name, CNT_DATA * data );
void cnt_dump( CNT cnt );
void cnt_json( CNT cnt );
CNT_BYTES cnt_val_b( CNT cnt, char *name );
CNT cnt_create( void );
CNT_VECTOR_PAGE page_create( MEM_ARENA arena );
CNT cnt_create_a( MEM_ARENA p_arena );

int lv1_compare( void *a, void *b );
int lv21_compare( void *a, void *b );
int lv2_compare( void *a, void *b );
int lv12_compare( void *a, void *b );

bool gen_assign( CNT_VECTOR_PAGE p, CMP f, void *elem, CNT_IDX * idx );
bool cell_page_split( CNT_VECTOR_PAGE * p_new,
                      MEM_ARENA arena, CNT_VECTOR_PAGE p0 );

CNT_IDX col_assigned( MEM_ARENA, CNT_VECTOR_PAGE p, char *name );


MEM_ARENA a;

CNT cnt_set_tst( int max_col, int max_row ) {
    char colname[100];
    CNT cnt = cnt_create_a( a );
    TRY for( int i = 1; i <= max_row; i++ ) {
        CNT_DATA *y;
        const CNT_DATA *x;
        for( int j = 1; j <= max_col; j++ ) {
            y = cnt_create_data( cnt, "cell %d %d", j, i );
            if( i == 1 ) {
                sprintf( colname, "COL%d", j );
                cnt_set_idx_d( cnt, colname, i, y );
            }
            else {
                cnt_set_col_idx_d( cnt, j, i, y );
            }
            x = cnt_col_idx_d( cnt, j, i );
            if( x == NULL )
                break;
        }
        if( x == NULL ) {
            fprintf( stderr, "aborting\n" );
            break;
        }
    }
    CATCH( ex )
            fprintf( stderr, "error found %s\n", ex->msg );
    END_TRY return cnt;
}

////////////////////////////////////////////////////////////////////////////////
//
// SETUP
//
void setup(  ) {
    a = mem_arena_new(  );
}

//
// TEARDOWN
//
void teardown(  ) {
    mem_arena_free( a );
    mem_arena_dispose( &a );
}

int main( int argc, char **argv ) {
    ( void )argc;
    ( void )argv;
    if( argc != 3 ) {
        fprintf( stderr, "usage: %s <max rows> <max cols>\n", argv[0] );
        exit( -1 );
    }
    int max_cols = atoi( argv[2] );
    int max_rows = atoi( argv[1] );
    setup(  );
    CNT cnt = cnt_set_tst( max_cols, max_rows );
    cnt_json( cnt );
    teardown(  );
}
