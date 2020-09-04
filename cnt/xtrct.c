#include "platform.h"
#include "cnt_int.h"
#include <string.h>
#include "fmt.h"
#include <ctype.h>

int main(
    int     argc,
    char  * argv[]
        )
{
    CNT         cnt             = NULL;
    INT         status          = 0;
    MEM_ARENA   arena           = NULL;
    IO_FILE     cfile           = NULL;
    IO_FILE     afile           = NULL;
    IO_FILE     std_out         = NULL;
    char        name[1024]      = "";
    INT         n               = 0;
    INT         flg             = 0;
    INT         size            = 0;
    INT8      * s               = NULL;

    arena = mem_arena_new();
    status = io_connect_file( &std_out, NULL, "w" );
    assert( status == RC_OK );
    assert( std_out != NULL );

    if ( argc < 3 )
    {
        fmt_format( std_out, "%s <cnt file> <name list file>\n", argv[0] );
        return 1;
    }
    else
    {
        status = io_connect_file( &cfile, argv[1], "r" );
        assert( status == RC_OK );
        assert( cfile != NULL );

        status = io_connect_file( &afile, argv[2], "r" );
        assert( status == RC_OK );
        assert( afile != NULL );


        status = cnt_create( arena, &cnt );
        status = cnt_imp(cfile, cnt);
        for(;;)
        {
            status = io_gets( afile, name, sizeof(name), &n );
            if( n == 0 )
                break;
            while( !isprint(name[--n]) )
                name[n] = 0;
            status = cnt_get_val( cnt, name, &flg, &size, (void**)&s );
            if( status == RC_OK )
            {
                if( CNT_ISCONT(flg) )
                    cnt_exp( std_out, (CNT)s );
                else
                    fmt_format( std_out, "%s=%.*s\n", name, size, s );
            }
        }

        io_close( &afile );
        io_close( &cfile );
        mem_arena_dispose( &arena );
    }

    return status;
}
