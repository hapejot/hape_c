#include "cnt.h"
#include "cnt_int.h"

#define ALLOC(a, s) (mem_arena_calloc(a, (s), 1, __FILE__,__LINE__))

static CNT_DATA *convert_bytes_to_data( MEM_ARENA a, void *d ) {
    int l = strlen( d );
    CNT_DATA *r = ALLOC( a, l + sizeof( CNT_DATA ) );
    r->l = l;
    r->f.cnt = 0;
    strcpy( r->d, d );
    return r;
}

static CNT_HEAD find_col_by_name( CNT cnt, char *name){
    return cnt->first_col;
}

    return cnt->first_col;
void cnt_set_val_b( CNT cnt, char *name, void *data ) {
    ( void )cnt;
    ( void )name;
    ( void )data;
    CNT_DATA *d = convert_bytes_to_data( cnt->arena, data );
    cnt_set_val_d( cnt, name, d );
}

void cnt_set_val_d( CNT cnt, char *name, CNT_DATA * data ) {
    ( void )cnt;
    ( void )name;
    ( void )data;
    CNT_HEAD *col = find_col_by_name( cnt, name );

}
