////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
////////////////////////////////////////////////////////////////////////////////
#include <hape/cnt.h>
#include <hape/err.h>
#include "cnt_int.h"

CNT_VECTOR_PAGE page_create( MEM_ARENA arena );

/*
 a container uses three kinds of vector pages.
 1. a vector page for the columns
 2. a vector page for the cells
 3. an optional vector page for cell vector pages, second level pages

vector pages have a general set of operations
1. create 
2. find
3. insert
4. delete

requires a compare function that compares the pointers.

col_compare
col_find
col_insert
col_delete

lv1_compare
lv1_find
lv1_insert
lv1_delete

lv2_compare
lv1_find
lv1_insert
lv1_delete

*/

// returns true if found. The index points to the found position in the vector page or to the next
// higher position if not found. this should be the position to insert the item.
// if the page is empty idx == 0
// if the item needs to be appended as last item idx == used
bool col_find(CNT_VECTOR_PAGE p, CNT_IDX pos, char*name, CNT_IDX*idx);  
int col_compare(void*, void*);

// returns true if found. The index points to the found position in the vector page or to the next
// higher position if not found. this should be the position to insert the item.
// if the page is empty idx == 0
// if the item needs to be appended as last item idx == used
bool lv1_find(CNT_VECTOR_PAGE p, CNT_IDX row, CNT_IDX col, CNT_IDX*idx);

typedef int (*CMP)(void*, void*);

bool gen_find(CNT_VECTOR_PAGE p, CMP f, void* elem, CNT_IDX*idx){
    int r = -1;  // r is negative when it is smaller than anything
    CNT_IDX i = 0;
    while(i < p->used) {
        r = f(elem, p->ptr[i]);
        if( r <= 0 ) break;
        i++;
    }
    if( r == 0 ) { *idx = i ; return true; }
    if( r < 0 ) { *idx = i; return false; }
    if( r > 0 ) { *idx = p->used; return false; }
}


////////////////////////////////////////////////////////////////////////////////
//
// Columns:

int col_compare( void* a, void* b ){
    return ((CNT_COL)a)->pos - ((CNT_COL)b)->pos;
}

CNT_IDX cnt_columns( CNT cnt )
{
    return cnt->max_col;
}

CNT_IDX cnt_lines( CNT cnt )
{
    return cnt->max_row;
}

CNT_COL_NAME cnt_colname( CNT cnt, CNT_IDX col )
{
    if( col < cnt->cols->used )
    {
        CNT_COL c = cnt->cols->ptr[col];
        return c->name;
    }
    else
        return NULL;
}

// 
//
//
//
// uses: strcmp
static CNT_IDX column_pos( CNT cnt, char *name )
{
    CNT_IDX pos = -1;
    for( CNT_IDX i = 0; i < cnt->cols->used; i++ )
    {
        CNT_COL col = cnt->cols->ptr[i];
        if( strcmp( col->name, name ) == 0 )
        {
            pos = col->pos;
            break;
        }
    }
    return pos;
}

// uses: ALLOC, memcpy
CNT_DATA *data_cpy( MEM_ARENA a, CNT_DATA * data )
{
    CNT_DATA *r = ALLOC( a, sizeof( *r ) );
    r->d = ALLOC( a, data->l + 1 );
    r->f = data->f;
    r->l = data->l;
    memcpy( r->d, data->d, r->l + 1 );
    return r;
}

// uses strlen, strcpy, ALLOC
static CNT_DATA *convert_bytes_to_data( MEM_ARENA a, void *d )
{
    CNT_IDX l = strlen( d );
    CNT_DATA *r = ALLOC( a, sizeof( CNT_DATA ) );
    r->d = ALLOC( a, l + 1 );
    r->l = l;
    r->f.cnt = 0;
    strcpy( r->d, d );
    return r;
}

CNT_COL cnt_column( CNT cnt, char *name )
{
    VECTOR_PAGE *cols = cnt->cols;
    CNT_IDX pos = column_pos( cnt, name );
    if( pos < 0 )
    {
        if( cols->used + 1 < VECTOR_PAGE_MAX )
        {
            pos = cols->used;
            cols->used++;
            CNT_COL col = ALLOC( cnt->arena, sizeof( *col ) );
            strcpy( col->name, name );
            col->pos = pos;
            cols->ptr[pos] = col;
            assert( cols->used < VECTOR_PAGE_MAX );
        }
        else
        {
            fprintf( stderr, _("too many columns\n") );
            exit( -1 );
        }
    }
    return ( CNT_COL ) cols->ptr[pos];
}



void cnt_set_val_b( CNT cnt, CNT_COL_NAME name, void *data )
{
    CNT_DATA *d = convert_bytes_to_data( cnt->arena, data );
    cnt_set_val_d( cnt, name, d );
}

void cnt_set_idx_b( CNT cnt, CNT_COL_NAME name, CNT_IDX row, void *data )
{
    CNT_DATA *d = convert_bytes_to_data( cnt->arena, data );
    cnt_set_idx_d( cnt, name, row, d );
}

CNT_IDX cell_pos( CNT cnt, CNT_IDX row, CNT_IDX col )
{
    VECTOR_PAGE *pg = cnt->cells;
    switch ( pg->type )
    {
        case CELLS_LEVEL_1:
            for( CNT_IDX j = 0; j < pg->used; j++ )
            {
                CNT_CELL c = pg->ptr[j];
                if( c->row == row && c->col == col )
                {
                    return j;
                }
            }
            return -1;

        case CELLS_LEVEL_2:
            for( CNT_IDX j = 0; j < pg->used; j++ )
            {
                VECTOR_PAGE *pg2 = pg->ptr[j];
                for( CNT_IDX i = 0; i < pg2->used; i++ )
                {
                    CNT_CELL c = pg2->ptr[j];
                    if( c->row == row && c->col == col )
                    {
                        return j;
                    }
                }
            }
            return -1;
        default:
            assert( false );
    }
}

////////////////////////////////////////////////////////////////////////////////
// compare:
//    (x1,x2) < (y1,y2) -> < 0
//    (x1,x2) = (y1,y2) -> = 0
//    (x1,x2) > (y1,y2) -> > 0
//    
int pair_compare(CNT_IDX x1, CNT_IDX x2, CNT_IDX y1, CNT_IDX y2){
    int r = x1 - y1;
    if( r == 0 ){
        r = x2 - y2;
    }
    return r;
}

////////////////////////////////////////////////////////////////////////////////
// find cell by row and col number
//
// a cell identified by row/col will be searched in the page
// the page can be a single level page
// or two level pages.
//
// when a cell is found the index will be returned as
// page * VECTOR_PAGE_MAX + entry
// for the single page case the page is just 0 so the 0 based
// entry number in that page will be returned.
//
// when a cell is not found
CNT_IDX cell_find( CNT_VECTOR_PAGE page, CNT_IDX row, CNT_IDX col )
{
    CNT_IDX r = -1;
    switch ( page->type )
    {
        case CELLS_LEVEL_1:
            for( CNT_IDX j = 0; j < page->used; j++ )
            {
                CNT_CELL c = page->ptr[j];
                int cmp = pair_compare(c->row, c->col, row, col);
                if(cmp == 0){
                        r = j;
                }
                else if(cmp > 0) 
                {
                    r = -j; // negative index for not found until this position
                    break;
                }
            }
            break;
        case CELLS_LEVEL_2:
            for( CNT_IDX i = page->used - 1; i >= 0; i-- )
            {
                r = cell_find( page->ptr[i], row, col );
                if( r >= 0 )
                {
                    r += VECTOR_PAGE_MAX * i;
                    break;
                }
            }
            break;
    }

    return r;
}

////////////////////////////////////////////////////////////////////////////////
//
//
void *cnt_idx_b( CNT cnt, CNT_COL_NAME name, CNT_IDX row )
{
    CNT_COL col = cnt_column( cnt, name );
    assert( col );
    void *result = NULL;

    CNT_VECTOR_PAGE p0 = cnt->cells;
    switch ( p0->type )
    {
        case CELLS_LEVEL_1:
            {
                CNT_IDX cell_no = cell_find( p0, row, col->pos );
                if( cell_no < cnt->cells->used )
                {
                    CNT_CELL c = cnt->cells->ptr[cell_no];
                    result = c->val->d;
                }
            }
            break;

        case CELLS_LEVEL_2:
            {
                for( CNT_IDX i = 0; i < p0->used; i++ )
                {
                    CNT_IDX cell_no = cell_find( p0->ptr[i], row, col->pos );
                    if( cell_no < cnt->cells->used )
                    {
                        CNT_CELL c = cnt->cells->ptr[cell_no];
                        result = c->val->d;
                    }
                }
            }
            break;
    }
    return result;
}

CNT_VECTOR_PAGE cell_page_split( MEM_ARENA arena, CNT_VECTOR_PAGE p0 )
{
    CNT_VECTOR_PAGE r = page_create( arena );
    r->type = CELLS_LEVEL_1;
    CNT_IDX half = p0->used / 2;
    for( CNT_IDX i = half; i < p0->used; i++ )
    {
        r->ptr[r->used++] = p0->ptr[i];
        p0->ptr[i] = NULL;
    }
    p0->used = half;
    return r;
}

void cell_page_insert( CNT_VECTOR_PAGE p, CNT_IDX idx, CNT_CELL cell )
{
    p->used++;
    for( CNT_IDX i = p->used; i >= idx; i-- )
        p->ptr[i] = p->ptr[i - 1];
    p->ptr[idx] = cell;
}

void cnt_set_idx_d( CNT cnt, CNT_COL_NAME name, CNT_IDX row, CNT_DATA * data )
{
    CNT_COL col = cnt_column( cnt, name );
    if( cnt->max_col <= col->pos )
        cnt->max_col = 1 + col->pos;
    if( cnt->max_row <= row )
        cnt->max_row = 1 + row;
    CNT_IDX cellpos = cell_pos( cnt, row, col->pos );
    CNT_VECTOR_PAGE v = cnt->cells;
    CNT_CELL cell = NULL;
    if( cellpos < 0 )
    {
        switch ( v->type )
        {
            case CELLS_LEVEL_1:
                {
                    CNT_IDX idx = cell_find( cnt->cells, row, col->pos );
// if page is full split it into two halfs
                    if( idx >= VECTOR_PAGE_MAX )
                    {
                        CNT_VECTOR_PAGE p0 = page_create( cnt->arena );
                        p0->type = CELLS_LEVEL_2;
                        p0->used = 2;
                        p0->ptr[0] = cnt->cells;
                        p0->ptr[1] =
                                cell_page_split( cnt->arena, cnt->cells );
                        cnt->cells = p0;
                        cnt_set_idx_d( cnt, name, row, data );
                    }
                    else
                    {
                        v->used++;
                        for( CNT_IDX i = v->used; i >= idx; i-- )
                            v->ptr[i] = v->ptr[i - 1];
                        cell = ALLOC( cnt->arena, sizeof( *cell ) );
                        cell->row = row;
                        cell->col = col->pos;
                        v->ptr[idx] = cell;
                    }
                }
                break;

            case CELLS_LEVEL_2:
                {
                    CNT_IDX idx = cell_find( cnt->cells, row, col->pos );
                    CNT_IDX pidx = idx / VECTOR_PAGE_MAX;
                    CNT_IDX lidx = idx % VECTOR_PAGE_MAX;
                    assert(idx < VECTOR_PAGE_MAX);
                    cell = ALLOC( cnt->arena, sizeof( *cell ) );
                    cell->row = row;
                    cell->col = col->pos;
                    cell_page_insert( cnt->cells->ptr[idx / VECTOR_PAGE_MAX],
                                      idx % VECTOR_PAGE_MAX, cell );
                }
                break;
        }
    }
    else
    {
        cell = ( CNT_CELL ) v->ptr[cellpos];
    }
    cell->val = ( void * )data_cpy( cnt->arena, data );
}

void cnt_set_col_idx_b( CNT cnt, CNT_IDX col, CNT_IDX row, CNT_BYTES data)
{
    ERR_LOG_ENTRY e = { .pkg = "cnt", .id = "01" };
    RAISE(e);
    cnt_set_col_idx_d( cnt, col, row, convert_bytes_to_data( cnt->arena, data) );
}

void cnt_set_col_idx_d( CNT cnt, CNT_IDX col, CNT_IDX row, CNT_DATA *data)
{
}

void cnt_set_val_d( CNT cnt, CNT_COL_NAME name, CNT_DATA * data )
{
    cnt_set_idx_d( cnt, name, 0, data );
}

void cnt_dump( CNT cnt )
{
    for( CNT_IDX i = 0; i < cnt->cols->used; i++ )
    {
        CNT_COL col = cnt->cols->ptr[i];
        printf( "COL: %d %s %d\n", i, col->name, col->pos );
    }
    for( CNT_IDX i = 0; i < cnt->cells->used; i++ )
    {
        CNT_CELL c = cnt->cells->ptr[i];
        printf( "CELL: %d %d %d %s[%d]%02x\n", i,
                c->row, c->col, c->val->d, c->val->l, c->val->d[0] );
    }
}

void cnt_json( CNT cnt )
{
    char *sep = "";
    printf( "[" );
    CNT_IDX current_row = -1;
    for( CNT_IDX i = 0; i < cnt->cells->used; i++ )
    {
        CNT_CELL c = cnt->cells->ptr[i];
        if( current_row != c->row )
        {
            if( current_row >= 0 )
                printf( "}, {" );
            else
                printf( "{" );
            current_row = c->row;
            sep = "";
        }
        for( CNT_IDX j = 0; j < cnt->cols->used; j++ )
        {
            CNT_COL col = cnt->cols->ptr[j];
            if( col->pos == c->col )
            {
                printf( "%s\"%s\":", sep, col->name );
                break;
            }
        }
        printf( "\"%s\"", c->val->d );
        sep = ", ";
    }
    if( current_row >= 0 )
        printf( "}" );
    printf( "]\n" );
}


void *cnt_val_b( CNT cnt, char *name )
{
    void *result = NULL;
    CNT_IDX pos = column_pos( cnt, name );


    for( CNT_IDX j = 0; j < cnt->cells->used; j++ )
    {
        CNT_CELL c = cnt->cells->ptr[j];
        if( c->row == 0 && c->col == pos )
        {
            result = c->val->d;
            break;
        }
    }

    return result;
}

static MEM_ARENA g_arena = NULL;

/*---------------------------------------------------------------------------
 * NAME
 *      cnt_create - Create a container stored in a given memory arena.
 *
 * SYNOPSIS
 */
CNT cnt_create(  )
{
    if( !g_arena )
    {
        g_arena = mem_arena_new(  );
    }

    return cnt_create_a( g_arena );
}

CNT_VECTOR_PAGE page_create( MEM_ARENA arena )
{
    CNT_VECTOR_PAGE r = NULL;
    r = ALLOC( arena, sizeof( *r ) );
    r->used = 0;
    r->type = 0;
    return r;
}

CNT cnt_create_a( MEM_ARENA p_arena )
/*
 * DESCRIPTION
 *      The container will be allocated using the given memory arena.
 *      The arena may be dropped at once, all the containers and values
 *      that are associated with it will be dropped as well.
 *      The continer values and subcontainers are automatically stored
 *      in the same arena as the main container.
 * 
 * PARAMETERS
 *      p_arenindent: Standard input:351: Error:Unexpected end of file
a - pointer to arena to use.
 * 
 * RETURN VALUES
 *      p_cnt - returned pointer to the container created.
 *
 * ERRORS
 *
 *---------------------------------------------------------------------------*/
{
/* environment check */
    CNT cnt = NULL;
/* processing. */
    cnt = mem_arena_calloc( p_arena, sizeof( *cnt ), 1, __FILE__, __LINE__ );
    assert( cnt != NULL );
    cnt->arena = p_arena;
    cnt->cols = page_create( p_arena );
    cnt->cols->type = COLLS;
    cnt->cells = page_create( p_arena );
    cnt->cells->type = CELLS_LEVEL_1;


    cnt->max_row = 0;
    cnt->max_col = 0;

/* return */
    return cnt;
}
