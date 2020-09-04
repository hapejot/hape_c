#include "platform.h"
#include "cnt_int.h"
#include <string.h>

int main(
    int     argc,
    char  * argv[]
        )
{
    CNT         cnt             = NULL;
    INT         status          = 0;
    MEM_ARENA   arena           = NULL;
    IO_FILE     std_in          = NULL;
    IO_FILE     std_out         = NULL;
    INT         n               = 0;

    arena = mem_arena_new();

    status = io_connect_file( &std_in, NULL, "r" );
    assert( status == RC_OK );
    status = io_connect_file( &std_out, NULL, "w" );
    assert( status == RC_OK );


    status = cnt_create( arena, &cnt );
    status = cnt_imp( std_in, cnt );
    if( argc >= 2 ) 
    {
        CNT     subcnt  = NULL;
        INT     flg     = 0;
        INT     i       = 0;

        for( i = 1; i < argc; i++ )
        {
            status = cnt_get_val( cnt, argv[i], &flg, NULL, (void**)&subcnt );
            if( CNT_ISCONT(flg) )
                status = cnt_dump( subcnt );
            else
                status = RC_ERROR;
            status = io_write( std_out, "\n", -1, &n );
            assert( status == RC_OK );
        }
    }
    else
        status = cnt_dump( cnt );
    status = io_write( std_out, "\n", -1, &n );
    mem_arena_dispose( &arena );
    return status;
}
