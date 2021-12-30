#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <hape/platform.h>
#include <hape/err.h>
#include <hape/mem.h>
#include "mem_int.h"



/*---------------------------------------------------------------------------
 * NAME
 *      main - 
 *
 * SYNOPSIS
 */
int main(int argc, char **argv)
/*
 * DESCRIPTION
 * 
 * PARAMETERS
 * 
 * RETURN VALUES
 *
 * ERRORS
 *      [RC_OK]         Successful completion.
 *
 *---------------------------------------------------------------------------*/
{
    (void) argc;
    (void) argv;
    INT status = RC_OK;
/* end of local var. */

/* parameter check */

/* environment check */

/* processing. */
    if( status == RC_OK ) {
        INT i;
        MEM_ARENA arena;
        unsigned char *d[256];
        INT s[256];
        INT j;
        ERR_LOG_ENTRY msg;

/* Arena Testing */
        arena = mem_arena_new(  );
        for( i = 0; i < 256; i++ ) {
            s[i] = i * 12345 + 500;
            d[i] = mem_arena_alloc( arena, s[i], __FILE__, __LINE__ );
            memset( d[i], i, s[i] );
        }
        for( i = 0; i < 256; i++ ) {
            sprintf( msg.msg, "d[%d] = %p", i, ( void * )d[i] );
            msg.typ = 'I';
            strcpy( msg.id, "00" );
            strcpy( msg.pkg, "mem" );
            err_log_add( &msg );
            for( j = 0; j < s[i]; j++ ) {
                if( d[i][j] != i ) {
                    fprintf( stderr, "d[%d][%d] == %d\n", i, j, d[i][j] );
                    exit( 0 );
                }
            }
        }
        puts( "READY." );
        getchar(  );
        mem_arena_free( arena );
        puts( "READY." );
        getchar(  );
        mem_arena_dispose( &arena );
        puts( "READY." );
        getchar(  );
    }
    err_log_dump(  );

    fprintf( stderr, "allocated: %d\n", g_mem.alloc_cnt );

/* return */
    return status;
}
