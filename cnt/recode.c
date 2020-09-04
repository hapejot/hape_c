#include "platform.h"
#include "cnt_int.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(
    int     argc,
    char  * argv[]
        )
{
    CNT             cnt             = NULL;
    INT             status          = 0;
    MEM_ARENA       arena           = NULL;
    IO_FILE         f               = NULL;
    IO_FILE         std_out         = NULL;
    char      *     filename        = NULL;
    INT             skip            = 0;
    INT             output_type     = 0;
    INT             input_type      = 0;
    CNT_CSVOPTS     opts;

    opts.sep_char = ',';
    opts.no_title = 0;

    arena = mem_arena_new();
    argv++;
    argc--;
    while ( argc )
    {
        if ( strcmp(argv[0], "--skip") == 0 )
        {
            skip = atol(argv[1]);
            argv +=2;
            argc -=2;
        }
        else if( strcmp(argv[0], "--intype") == 0 )
        {
            if( strcmp(argv[1], "cnt") == 0 )
                input_type = 0;
            else if( strcmp(argv[1], "csv") == 0 )
                input_type = 1;
            else 
            {
                input_type = -1;
                fprintf( stderr, "unknown input type '%s'\n", argv[1] );
            }
            argv += 2;
            argc -= 2;
        }
        else if( strcmp(argv[0], "--outtype") == 0 )
        {
            if( strcmp(argv[1], "cnt") == 0 )
                output_type = 0;
            else if( strcmp(argv[1], "sgmls") == 0 )
                output_type = 1;
            else if( strcmp(argv[1], "json") == 0 )
                output_type = 2;
            else 
            {
                input_type = -1;
                fprintf( stderr, "unknown output type '%s'\n", argv[1] );
            }
            argv += 2;
            argc -= 2;
        }
    }


    if ( argc == 2  )
    {
        filename = argv[1];
    }

    status = io_connect_file( &f, filename, "r" );
    assert( status == RC_OK );
    status = io_connect_file( &std_out, NULL, "w" );
    assert( status == RC_OK );
    status = cnt_create( arena, &cnt );
    while( skip > 0 )
    {
        status = cnt_imp(f, cnt);
        skip--;
    }
    mem_arena_free( arena );
    arena = mem_arena_new();
    status = cnt_create( arena, &cnt );
    switch( input_type )
    {
        case 0:
            status = cnt_imp(f, cnt);
            break;
        case 1:
            status = cnt_from_csv( f, opts, cnt );
            break;
    }
    switch( output_type )
    {
        case 0:
            status = cnt_exp(std_out, cnt);
            break;
        case 1:
            status = cnt_exp_par(std_out, cnt);
            break;
        case 2:
            status = cnt_exp_json(std_out, cnt);
            break;
    }

    io_close(&f);
    return status;
}
