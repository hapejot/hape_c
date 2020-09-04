#include "platform.h"
#include "mem_int.h"
#include <assert.h>
#include <malloc.h>

/*---------------------------------------------------------------------------
 * NAME
 *      mem_arena_alloc - 
 *
 * SYNOPSIS
 */
void *mem_arena_alloc
        ( IN MEM_ARENA arena, IN INT nbytes, IN char *file, IN INT line )
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
    void *result = 0;
    INT status = RC_OK;
/* end of local var. */
    ( void )file;
    (void)line;
/* parameter check */
    assert( arena );
    assert( nbytes > 0 );

/* environment check */

/* processing. */
    if( status == RC_OK ) {
        INT alignment = sizeof( union align );

        nbytes = ( ( nbytes + alignment - 1 ) / alignment ) * alignment;
        if( nbytes > ( arena->limit - arena->avail ) ) {
            MEM_ARENA ptr = NULL;
            char *limit = NULL;
            INT m = 0;

            m = sizeof( union header ) + nbytes + 10 * 1024;
            ptr = malloc( m );
            assert( ptr != NULL );
            limit = ( char * )ptr + m;
            *ptr = *arena;
            arena->avail = ( char * )( ( union header * )ptr + 1 );
            arena->limit = limit;
            arena->prev = ptr;
        }
        result = arena->avail;
        arena->avail += nbytes;

        if( status == RC_OK )
            g_mem.alloc_cnt += nbytes;
    }

/* return */
    return result;
}
