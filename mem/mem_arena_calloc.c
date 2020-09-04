#include "platform.h"
#include "mem_int.h"
#include <assert.h>
#include <malloc.h>

/*---------------------------------------------------------------------------
 * NAME
 *      mem_arena_calloc - 
 *
 * SYNOPSIS
 */
void *
mem_arena_calloc
(
    IN      MEM_ARENA       arena,
    IN      INT             count,
    IN      INT             nbytes,
    IN      char          * file,
    IN      INT             line
)
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
    void      * result = NULL;
    INT         status = RC_OK;
    /* end of local var. */

    /* parameter check */
    assert(count > 0);
    
    /* environment check */

    /* processing. */
    if ( status == RC_OK )
    {
	    result = mem_arena_alloc(arena, count * nbytes, file, line);
        memset( result, '\0', count * nbytes);
    }

    /* return */
    return result;
}

