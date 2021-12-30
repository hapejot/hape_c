////////////////////////////////////////////////////////////////////////////////
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <libunwind.h>
#include <bfd.h>
#include <malloc.h>
////////////////////////////////////////////////////////////////////////////////
#include <hape/cnt.h>
#include <hape/err.h>
#include "cnt_int.h"
#include <execinfo.h>
#include <unistd.h>


ERR_EXC cnt_assign_err = {.msg = "assign error." };
ERR_EXC cnt_unknown_page_type = {.msg = "unknown page type" };

CNT_VECTOR_PAGE page_create( MEM_ARENA arena );

#define BACKTRACE_SIZE 20
void stack_print_old(  )
{

    void *trace[BACKTRACE_SIZE];
    char **messages = NULL;
    size_t size;

    size = backtrace( trace, BACKTRACE_SIZE );

// backtrace_symbols_fd( trace, size, STDERR_FILENO );
    messages = backtrace_symbols( trace, size );

    for( size_t i = 0; i < size; i++ )
    {
        fprintf( stderr, "%zu > %s\n", i, messages[i] );
        static char cmd[1000];
        sprintf( cmd, "addr2line -e build.hp/cnt/libcnt.so %p", trace[i] );
        system( cmd );
    }
}


void print_location( const char *libname, long address )
{
    static unsigned lineno;
    static const char *filename = NULL;
    static const char *functionname = NULL;
    static asymbol **syms = NULL;
    static asection *sect = NULL;
    static bfd *abfd = NULL;
    bool rc;

    if( abfd == NULL )
        bfd_init(  );
    if( filename == NULL || strcmp( libname, filename ) )
    {
        abfd = bfd_openr( libname, NULL );
        rc = bfd_check_format( abfd, bfd_object );
        assert( rc );
        assert( strcmp( bfd_get_target( abfd ), "elf64-x86-64" ) == 0 );
        int storage_needed = bfd_get_symtab_upper_bound( abfd );
        assert( storage_needed > 0 );
        syms = malloc( storage_needed );
        assert( syms != NULL );
        rc = bfd_canonicalize_symtab( abfd, syms );
        sect = bfd_get_section_by_name( abfd, ".text" );
        assert( sect != 0 );


// ck_assert_int_eq( sect->vma, 9168);
// ck_assert_int_eq( sect->size, 16629);
    }
    assert( abfd );
    assert( sect );
    assert( syms );
    rc = bfd_find_nearest_line( abfd, sect, syms,
                                address - sect->vma, &filename,
                                &functionname, &lineno );
    fprintf( stderr, "%-30s: %s[%d]\n", functionname, filename, lineno );
}


void stack_print(  )
{
    unw_cursor_t cursor;
    unw_context_t uc;
    unw_word_t ip;
    unw_proc_info_t pip;

    unw_getcontext( &uc );
    unw_init_local( &cursor, &uc );
    bfd_init(  );
    while( unw_step( &cursor ) > 0 )
    {
        unw_get_reg( &cursor, UNW_REG_IP, &ip );
        unw_get_proc_info( &cursor, &pip );
        char name[100];
        unw_word_t off;
        unw_get_proc_name( &cursor, name, 100, &off );

        Dl_info dlinfo;
        dladdr( ( void * )( pip.start_ip + off ), &dlinfo );
        long pc = ( long )ip - ( long )dlinfo.dli_fbase;
        print_location( dlinfo.dli_fname, pc );
    }
}


/*
 a container uses three kinds of vector pages.
 1. a vector page for the columns
 2. a vector page for the cells
 3. an optional vector page for cell vector pages, second level pages

vector pages have a general set of operations
1. create 
2. find
3. assign (insert or update)
4. delete

requires a compare function that compares the pointers.

col_compare
col_find
col_assign
col_delete

lv1_compare
lv1_find
lv1_assign
lv1_delete

lv2_compare
lv2_find
lv2_assign
lv2_delete

*/

// returns true if found. The index points to the found position in the vector page or to the next
// higher position if not found. this should be the position to insert the item.
// if the page is empty idx == 0
// if the item needs to be appended as last item idx == used
CNT_IDX col_find( CNT_VECTOR_PAGE, char *name );
int col_compare( void *, void * );

// returns true if found. The index points to the found position in the vector page or to the next
// higher position if not found. this should be the position to insert the item.
// if the page is empty idx == 0
// if the item needs to be appended as last item idx == used
bool lv1_find( CNT_VECTOR_PAGE p, CNT_IDX row, CNT_IDX col, CNT_IDX * idx );

typedef int ( *CMP ) ( void *, void * );

void check_page( CNT_VECTOR_PAGE p )
{
    assert( p );
    bool r = false;
    if( ( p->type & PAGE_TYPE_MASK ) != PAGE_TYPE )
    {
        fprintf( stderr, "unknown type: %p %x \n", ( void * )p, p->type );
        r = true;
    }
    if( p->used > VECTOR_PAGE_MAX )
    {
        fprintf( stderr, "usage too high: %d\n", p->used );
        r = true;
    }
    for( CNT_IDX j = 0; j < p->used; j++ )
    {
        if( p->ptr[j] == NULL )
        {
            fprintf( stderr, "* Pointer %d is null in %p.\n", j,
                     ( void * )p );
            r = true;
        }
        if( j > 10 )
            break;
    }
    if( r )
    {
        stack_print(  );
        fprintf( stderr, "----\n" );
        exit( -1 );
    }
}

bool gen_find( CNT_VECTOR_PAGE p, CMP f, void *elem, CNT_IDX * idx )
{
// assert( elem );
// assert( idx );
// assert( f );
// assert( p );
// fprintf( stderr, "find %p %p %p\n", ( void * )p, ( void * )f, elem );
    check_page( p );
    int low = 0;
    int high = p->used - 1;
// high = -1, when list empty
    bool rc = false;
    while( low <= high )
    {
        int m = ( low + high ) / 2;
        int r_m = f( elem, p->ptr[m] );
// fprintf( stderr, "find low:%d m:%d high:%d r:%d\n", low, m, high, r_m );
        if( r_m < 0 )
        {                                        // elem < A_m
            high = m - 1;
        }
        else if( r_m > 0 )
        {                                        // elem > A_m
            low = m + 1;
        }
        else
        {                                        // elem == A_m
            *idx = m;
            rc = true;
            break;
        }
    }
    if( !rc )
        *idx = high + 1;
    return rc;
}

bool gen_assign( CNT_VECTOR_PAGE p, CMP f, void *elem, CNT_IDX * idx )
{
    assert( p );
    assert( f );
    assert( elem );
    assert( idx );
    int m;
    bool rc = false;
    if( gen_find( p, f, elem, &m ) )
    {

    }
    else
    {
        if( p->used < VECTOR_PAGE_MAX )
        {
            for( int i = p->used - 1; i >= m; i-- )
            {
                p->ptr[i + 1] = p->ptr[i];
            }
            *idx = m;
            p->ptr[*idx] = elem;
            p->used++;
            rc = true;
        }
    }
    check_page( p );
    return rc;
}

////////////////////////////////////////////////////////////////////////////////
//
// Columns:
//
int col_compare( void *a, void *b )
{
// return ( ( CNT_COL ) a )->pos - ( ( CNT_COL ) b )->pos;
    return strcmp( a, b );
}

CNT_COL col_create( MEM_ARENA a, int pos, char *name )
{
    CNT_COL r = ALLOC( a, sizeof( *r ) );
    assert( strlen( name ) < sizeof( r->name ) );
    strcpy( r->name, name );
    r->pos = pos;
    return r;
}

CNT_IDX col_find( CNT_VECTOR_PAGE p, char *name )
{
    assert( p );
    assert( name );
    assert( strlen( name ) > 0 );
    struct _cnt_col c2;
    CNT_IDX result = 0;
    CNT_IDX idx;
    c2.pos = 0;
    strcpy( c2.name, name );
    if( gen_find( p, col_compare, &c2, &idx ) )
    {
        CNT_COL col = p->ptr[idx];
        result = col->pos;
    }
    return result;
}

CNT_COL_NAME cnt_colname( CNT cnt, CNT_IDX col )
{
    CNT_VECTOR_PAGE cols = cnt->cols;
    CNT_COL_NAME result = NULL;
    for( int i = 0; i < cols->used; i++ )
    {
        CNT_COL c = cols->ptr[i];
        if( c->pos == col )
        {
            result = c->name;
            break;
        }
    }
    if( result == NULL )
    {
        fprintf( stderr, "column %d has no name.\n", col );
    }
    return result;
}

bool cnt_columns_all( CNT cnt, CNT_COL * cols, CNT_IDX max )
{
    CNT_VECTOR_PAGE cs = cnt->cols;
    memset( cols, 0, max * sizeof( *cols ) );
    for( int i = 0; i < cs->used; i++ )
    {
        CNT_COL c = cs->ptr[i];
        if( c->pos <= max )
        {
            cols[c->pos] = c;
        }
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//
// Cells:
// a < b <=> < a - b < 0
int lv1_compare( void *a, void *b )
{
    if( NULL == a )
        stack_print(  );
    if( NULL == b )
        stack_print(  );
    assert( a );
    assert( b );
    CNT_CELL ac = ( CNT_CELL ) a;
    CNT_CELL bc = ( CNT_CELL ) b;
    if( ac->row == bc->row )
        return ac->col - bc->col;
    else
        return ac->row - bc->row;
}


int lv2_compare( void *a, void *b )
{
    assert( a );
    assert( b );

    CNT_VECTOR_PAGE ap = ( CNT_VECTOR_PAGE ) a;
    CNT_VECTOR_PAGE bp = ( CNT_VECTOR_PAGE ) b;
    assert( ap->used > 0 );
    assert( bp->used > 0 );

    return lv1_compare( ap->ptr[0], bp->ptr[0] );
}

int lv21_compare( void *page, void *el )
{
// compare level-1-page a and cell b
// returns the comparison of the first cell of page a with cell b
    assert( page );
    assert( el );


    CNT_VECTOR_PAGE ap = ( CNT_VECTOR_PAGE ) page;
    check_page( ap );
    CNT_CELL bc = ( CNT_CELL ) el;
    assert( ap->used > 0 );
    int rv = lv1_compare( ap->ptr[0], bc );
// fprintf( stderr, "lv21compare %p %p -> %d\n", a, b, rv );
    if( rv > 0 )
        return 1;
    else
        return -1;
}

int lv12_compare( void *el, void *page )
{
    return -lv21_compare( page, el );
}

////////////////////////////////////////////////////////////////////////////////

CNT_IDX cnt_columns( CNT cnt )
{
    return cnt->max_col;
}

CNT_IDX cnt_lines( CNT cnt )
{
    return cnt->max_row;
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
static CNT_DATA *convert_bytes_to_data( MEM_ARENA a, CNT_BYTES d )
{
    CNT_IDX l = strlen( d );
    CNT_DATA *r = ALLOC( a, sizeof( CNT_DATA ) );
    r->d = ALLOC( a, l + 1 );
    r->l = l;
    r->f.cnt = 0;
    strcpy( r->d, d );
    return r;
}

CNT_DATA *cnt_create_data( CNT cnt, char *str, ... )
{
    MEM_ARENA a = cnt->arena;
    CNT_DATA *r = ALLOC( a, sizeof( CNT_DATA ) );
    char buf[1000];
    va_list ap;
    va_start( ap, str );
    vsprintf( buf, str, ap );
    CNT_IDX l = strlen( buf );
    r->d = ALLOC( a, l + 1 );
    r->l = l;
    r->f.cnt = 0;
    strcpy( r->d, buf );
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
            fprintf( stderr, _( "too many columns\n" ) );
            exit( -1 );
        }
    }
    return ( CNT_COL ) cols->ptr[pos];
}

CNT_IDX col_assigned( MEM_ARENA a, CNT_VECTOR_PAGE p, char *name )
{
    assert( p );
    assert( name );
    assert( strlen( name ) > 0 );
    struct _cnt_col c2;
    CNT_COL col = NULL;
    CNT_IDX result = 0;
    CNT_IDX idx;
    c2.pos = 0;
    strcpy( c2.name, name );
    if( gen_find( p, col_compare, &c2, &idx ) )
    {
        col = p->ptr[idx];
    }
    else
    {
        col = col_create( a, result, name );
        col->pos = p->used + 1;
        bool rc = gen_assign( p, col_compare, col, &idx );
        assert( rc );
    }
    result = col->pos;
    return result;
}

void cnt_set_val_b( CNT cnt, CNT_COL_NAME name, CNT_BYTES data )
{
    CNT_DATA *d = convert_bytes_to_data( cnt->arena, data );

    CNT_IDX pos = col_assigned( cnt->arena, cnt->cols, name );
    cnt_set_col_idx_d( cnt, pos, 0, d );
}

void cnt_set_idx_b( CNT cnt, CNT_COL_NAME name, CNT_IDX row, CNT_BYTES data )
{
    CNT_DATA *d = convert_bytes_to_data( cnt->arena, data );

    CNT_IDX pos = col_assigned( cnt->arena, cnt->cols, name );
    cnt_set_col_idx_d( cnt, pos, row, d );
}

CNT_IDX cell_pos( CNT cnt, CNT_IDX row, CNT_IDX col )
{
    stack_print(  );
    assert( 1 == 2 );
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
int pair_compare( CNT_IDX x1, CNT_IDX x2, CNT_IDX y1, CNT_IDX y2 )
{
    int r = x1 - y1;
    if( r == 0 )
    {
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
                int cmp = pair_compare( c->row, c->col, row, col );
                if( cmp == 0 )
                {
                    r = j;
                }
                else if( cmp > 0 )
                {
                    r = -j;                      // negative index for not found until this position
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

const CNT_DATA *cnt_idx_d( CNT cnt, CNT_COL_NAME name, CNT_IDX row )
{
    assert( name );
    const CNT_DATA *result = NULL;
    CNT_IDX pos = col_find( cnt->cols, name );

    if( pos > 0 )
    {
        result = cnt_col_idx_d( cnt, pos, row );
    }
    return result;
}


CNT_BYTES cnt_idx_b( CNT cnt, CNT_COL_NAME name, CNT_IDX row )
{
    assert( name );
    CNT_BYTES result = NULL;
    CNT_IDX pos = col_find( cnt->cols, name );

    if( pos > 0 )
    {
        const CNT_DATA *data = cnt_col_idx_d( cnt, pos, row );
        result = data->d;
    }
    return result;
}

CNT_BYTES cnt_idx_val_b( CNT cnt, CNT_IDX row )
{
    CNT_BYTES result = NULL;
    const CNT_DATA *data = cnt_col_idx_d( cnt, 0, row );
    if( data )
        result = data->d;
    return result;
}

CNT_CELL cell_create( MEM_ARENA a, CNT_IDX col, CNT_IDX row )
{
    CNT_CELL cell = NULL;
    cell = ALLOC( a, sizeof( *cell ) );
    cell->row = row;
    cell->col = col;
    return cell;
}

bool cell_page_split( CNT_VECTOR_PAGE * p_new,
                      MEM_ARENA arena, CNT_VECTOR_PAGE p0 )
{
    CNT_VECTOR_PAGE pn;
    pn = page_create( arena );
    pn->type = CELLS_LEVEL_1;
    CNT_IDX half = p0->used / 2;
    for( CNT_IDX i = half; i < p0->used; i++ )
    {
        CNT_IDX i_new = pn->used++;
        pn->ptr[i_new] = p0->ptr[i];
        p0->ptr[i] = NULL;
    }
    p0->used = half;
    *p_new = pn;

    check_page( *p_new );
    check_page( p0 );
    return true;
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
    CNT_IDX pos = col_assigned( cnt->arena, cnt->cols, name );
    cnt_set_col_idx_d( cnt, pos, row, data );
}

void cnt_set_col_idx_b( CNT cnt, CNT_IDX col, CNT_IDX row, CNT_BYTES data )
{
// ERR_LOG_ENTRY e = { .pkg = "cnt", .id = "01" };
// RAISE(e);
    cnt_set_col_idx_d( cnt, col, row,
                       convert_bytes_to_data( cnt->arena, data ) );
}

void cnt_set_idx_val_d( CNT cnt, CNT_IDX row, CNT_DATA * data )
{
    cnt_set_col_idx_d( cnt, 0, row, data );
}

void cnt_set_idx_val_b( CNT cnt, CNT_IDX row, CNT_BYTES data )
{
    cnt_set_idx_val_d( cnt, row, convert_bytes_to_data( cnt->arena, data ) );
}

// 2021-03-29:
// second level insert
CNT_IDX last_index;

bool _snd_lvl_insert( int *diff, CNT_IDX * p_no, CNT_VECTOR_PAGE p,
                      CNT_CELL c )
{
    int rc;
    check_page( p );
    if( p->type != CELLS_LEVEL_2 )
    {
        stack_print(  );
    }
    assert( p->type == CELLS_LEVEL_2 );
    last_index = -2;
    CNT_IDX pidx;
    CNT_IDX idx;
    gen_find( p, lv12_compare, c, &pidx );
    if( pidx > 1 )
        pidx--;
    CNT_VECTOR_PAGE p_sub = p->ptr[pidx];
    *p_no = pidx;
    int used_before = p_sub->used;
    rc = gen_assign( p_sub, lv1_compare, c, &idx );
    if( rc )
    {
        last_index = idx + ( pidx << 8 );
        *diff = p_sub->used - used_before;
        return true;
    }
    else
        return false;
}

// 2021-03-25:
void cnt_set_col_idx_d( CNT cnt, CNT_IDX col, CNT_IDX row, CNT_DATA * data )
{
    bool rc;
    assert( cnt );
    assert( data );
    CNT_VECTOR_PAGE p = cnt->cells;
    assert( p );
    CNT_CELL c = cell_create( cnt->arena, col, row );
    assert( c );
    c->val = ( void * )data_cpy( cnt->arena, data );
    if( p->type == CELLS_LEVEL_1 )
    {
        last_index = -1;
        CNT_IDX idx = -1;
        if( p->used == VECTOR_PAGE_MAX )
        {
// fprintf( stderr, "splitting page.\n" );
            CNT_VECTOR_PAGE p_new;
            rc = cell_page_split( &p_new, cnt->arena, p );
            cnt->cells = page_create( cnt->arena );
            cnt->cells->type = CELLS_LEVEL_2;
            rc = gen_assign( cnt->cells, lv2_compare, p, &idx );
            assert( 0 == idx );
            check_page( p );
            rc = gen_assign( cnt->cells, lv2_compare, p_new, &idx );
            if( !rc )
                RAISE( &cnt_assign_err );
            check_page( p_new );
            check_page( cnt->cells );
            check_page( p );
            assert( 1 == idx );
            int diff_used;
            CNT_IDX p_no;
            rc = _snd_lvl_insert( &diff_used, &p_no, cnt->cells, c );
            assert( rc );
            cnt->used += diff_used;
        }
        else
        {
            rc = gen_assign( p, lv1_compare, c, &idx );
            if( !rc )
                RAISE( &cnt_assign_err );
            cnt->used = p->used;
        }
    }
    else if( p->type == CELLS_LEVEL_2 )
    {
        int diff_used;
        CNT_IDX p_no;
        CNT_IDX idx = -1;
        rc = _snd_lvl_insert( &diff_used, &p_no, p, c );
        if( rc )
            cnt->used += diff_used;
        else
        {
            CNT_VECTOR_PAGE p_new;
            rc = cell_page_split( &p_new, cnt->arena, p->ptr[p_no] );
            assert( rc );
            rc = gen_assign( cnt->cells, lv2_compare, p_new, &idx );
            if( !rc )
            {
                fprintf( stderr, "page fill: %d %d\n",
                         cnt->used, cnt->cells->used );
                RAISE( &cnt_assign_err );
            }
            rc = _snd_lvl_insert( &diff_used, &p_no, p, c );
            assert( rc );
            cnt->used += diff_used;
        }
    }
    else
    {
        RAISE( &cnt_unknown_page_type );
    }


    if( col > cnt->max_col )
        cnt->max_col = col;
    if( row > cnt->max_row )
        cnt->max_row = row;
}

// 2021-03-24:

CNT_CELL page_find_cell( CNT_VECTOR_PAGE p, CNT_IDX col, CNT_IDX row )
{
    CNT_CELL c = alloca( sizeof( *c ) );
    c->row = row;
    c->col = col;
    CNT_IDX idx;
    CNT_CELL r = NULL;
    if( gen_find( p, lv1_compare, c, &idx ) )
    {
        r = p->ptr[idx];
    }
    return r;
}

// 2021-03-29: appended level 2 logic

const CNT_DATA *cnt_col_idx_d( CNT cnt, CNT_IDX col, CNT_IDX row )
{
    assert( cnt );
    CNT_VECTOR_PAGE p = cnt->cells;
    assert( p );
    CNT_CELL c = alloca( sizeof( *c ) );
    c->row = row;
    c->col = col;
    CNT_IDX idx;
    if( p->type == CELLS_LEVEL_1 )
    {
        if( gen_find( p, lv1_compare, c, &idx ) )
        {
            c = p->ptr[idx];
            return c->val;
        }
    }
    else if( p->type == CELLS_LEVEL_2 )
    {
// fprintf( stderr, "lookup cell level 2\n" );
        CNT_IDX pidx;
        gen_find( p, lv12_compare, c, &pidx );
// fprintf( stderr, "lookup cell page %d\n", pidx );
        if( pidx > 0 )
        {
            CNT_VECTOR_PAGE p1 = p->ptr[pidx - 1];
            if( gen_find( p1, lv1_compare, c, &idx ) )
            {
// fprintf( stderr, "lookup cell index %d\n", idx );
                c = p1->ptr[idx];
                return c->val;
            }
        }
    }
    return NULL;
}


void cnt_set_val_d( CNT cnt, CNT_COL_NAME name, CNT_DATA * data )
{
    cnt_set_idx_d( cnt, name, 0, data );
}

void cnt_dump( CNT cnt )
{
    ( void )cnt;
}

void cnt_json( CNT cnt )
{
    printf( "/* cols: %d  rows: %d  */\n", cnt->max_col, cnt->max_row );

    CNT_COL *cs = malloc( sizeof( *cs ) * ( cnt->max_col + 1 ) );
    cnt_columns_all( cnt, cs, cnt->max_col );

    if( cnt->max_row > 0 )
        printf( "[" );

    for( CNT_IDX row = 0; row <= cnt->max_row; row++ )
    {
        if( cnt->max_row > 0 && cnt->max_col > 0 && row == 0 )
            continue;                            // if it is a real table the row 0 is empty, tables start with row 1.
        if( cnt->max_col == 0 )
        {
            const CNT_DATA *x = cnt_col_idx_d( cnt, 0, row );
            if( x )
            {
                printf( "\"%s\"", x->d );
            }
            else
                printf( "null" );
            if( row < cnt->max_row )
                printf( ", " );
        }
        else
        {
            char *sep = "";
            printf( "{ " );
            for( CNT_IDX col = 0; col <= cnt->max_col; col++ )
            {
                const CNT_DATA *x = cnt_col_idx_d( cnt, col, row );
                if( x )
                {
                    printf( "%s\"%s\":\"%s\"", sep, cs[col]->name, x->d );
                    sep = ", ";
                }
            }
            if( row < cnt->max_row )
                printf( " },\n" );
            else
                printf( " }" );
        }
    }
    if( cnt->max_row > 0 )
        printf( "]" );
    printf( "\n" );
    free( cs );
}

const CNT_DATA *cnt_val_d( CNT cnt, char *name )
{
    assert( name );
    const CNT_DATA *result = NULL;
    CNT_IDX pos = col_find( cnt->cols, name );

    if( pos > 0 )
    {
        result = cnt_col_idx_d( cnt, pos, 0 );
    }
    return result;
}

CNT_BYTES cnt_val_b( CNT cnt, char *name )
{
    assert( name );
    void *result = NULL;
    CNT_IDX pos = col_find( cnt->cols, name );

    if( pos > 0 )
    {
        const CNT_DATA *d = cnt_col_idx_d( cnt, pos, 0 );
        result = d->d;
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
// fprintf( stderr, "new page %p\n", ( void * )r );
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
