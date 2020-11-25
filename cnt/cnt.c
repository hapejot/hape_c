#include "cnt.h"
#include "cnt_int.h"
#include <stdlib.h>

// uses: strcmp
static int column_pos( CNT cnt, char *name ) {
    int pos = -1;
    for( int i = 0; i < cnt->cols->used; i++ ) {
        CNT_COL col = cnt->cols->ptr[i];
        if( strcmp( col->name, name ) == 0 ) {
            pos = col->pos;
            break;
        }
    }
    return pos;
}

// uses: ALLOC, memcpy
CNT_DATA *data_cpy( MEM_ARENA a, CNT_DATA * data ) {
    CNT_DATA *r = ALLOC( a, sizeof( *r ) + data->l );
    r->f = data->f;
    r->l = data->l;
    memcpy( r->d, data->d, r->l + 1 );
    return r;
}

// uses strlen, strcpy, ALLOC
static CNT_DATA *convert_bytes_to_data( MEM_ARENA a, void *d ) {
    int l = strlen( d );
    CNT_DATA *r = ALLOC( a, l + sizeof( CNT_DATA ) );
    r->l = l;
    r->f.cnt = 0;
    strcpy( r->d, d );
    return r;
}

CNT_COL cnt_column( CNT cnt, char *name ) {
    VECTOR_PAGE *cols = cnt->cols;
    int pos = column_pos( cnt, name );
    if( pos < 0 ) {
        if( cols->used + 1 < VECTOR_PAGE_MAX ) {
            pos = cols->used;
            cols->used++;
            CNT_COL col = ALLOC( cnt->arena, sizeof( *col ) );
            strcpy( col->name, name );
            col->pos = pos;
            cols->ptr[pos] = col;
            assert( cols->used < VECTOR_PAGE_MAX );
        }
        else {
            fprintf( stderr, "too many columns\n" );
            exit( -1 );
        }
    }
    return ( CNT_COL ) cols->ptr[pos];
}



void cnt_set_val_b( CNT cnt, CNT_COL_NAME name, void *data ) {
    CNT_DATA *d = convert_bytes_to_data( cnt->arena, data );
    cnt_set_val_d( cnt, name, d );
}

void cnt_set_idx_b( CNT cnt, CNT_COL_NAME name, CNT_IDX row, void *data ) {
    CNT_DATA *d = convert_bytes_to_data( cnt->arena, data );
    cnt_set_idx_d( cnt, name, row, d );
}

int cell_pos( CNT cnt, int row, int col ) {
    for( int j = 0; j < cnt->cells->used; j++ ) {
        CNT_CELL c = cnt->cells->ptr[j];
        if( c->row == row && c->col == col ) {
            return j;
        }
    }
    return -1;
}



void cnt_set_idx_d( CNT cnt, CNT_COL_NAME name, CNT_IDX row, CNT_DATA * data ) {
    CNT_COL col = cnt_column( cnt, name );
    int cellpos = cell_pos( cnt, row, col->pos );
    CNT_VECTOR_PAGE v = cnt->cells;
    CNT_CELL cell = NULL;
    if( cellpos < 0 ) {
        int idx = v->used;
        v->used++;
        cell = ALLOC( cnt->arena, sizeof( *cell ) );
        cell->row = row;
        cell->col = col->pos;
        v->ptr[idx] = cell;
    }
    else {
        cell = ( CNT_CELL ) v->ptr[cellpos];
    }
    cell->val = ( void * )data_cpy( cnt->arena, data );
}

void cnt_set_val_d( CNT cnt, CNT_COL_NAME name, CNT_DATA * data ) {
    cnt_set_idx_d( cnt, name, 0, data );
}

void cnt_dump( CNT cnt ) {
    for( int i = 0; i < cnt->cols->used; i++ ) {
        CNT_COL col = cnt->cols->ptr[i];
        printf( "COL: %d %s %d\n", i, col->name, col->pos );
    }
    for( int i = 0; i < cnt->cells->used; i++ ) {
        CNT_CELL c = cnt->cells->ptr[i];
        printf( "CELL: %d %d %d %s\n", i, c->row, c->col, c->val->d );
    }
}


void *cnt_val_b( CNT cnt, char *name ) {
    void *result = NULL;
    int pos = column_pos( cnt, name );


    for( int j = 0; j < cnt->cells->used; j++ ) {
        CNT_CELL c = cnt->cells->ptr[j];
        if( c->row == 0 && c->col == pos ) {
            result = c->val->d;
            break;
        }
    }

    return result;
}
