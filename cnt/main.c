#include "platform.h"
#include "cnt.h"
#include "io.h"
#include <string.h>
#include <stdio.h>
#include "malloc.h"

int main(  ) {
    CNT cnt = NULL;
    int status = 0;
    MEM_ARENA arena = NULL;
    CNT_DATA *data;
    IO_FILE std_out;

    arena = mem_arena_new(  );

    if( 0 == io_connect_file( &std_out, NULL, "w" ) ) {

        cnt = cnt_create_a( arena );
/*            / value      */
        cnt_set_val_b( cnt, "a", "value1" );
        cnt_set_val_b( cnt, "b", "value2" );

        data = calloc( 1, sizeof( CNT_DATA ) + 20 );
        sprintf( data->d, "Value3" );
        data->l = strlen( data->d );

        cnt_set_val_d( cnt, "test", data );

        mem_arena_dispose( &arena );
    }

    return status;
}
