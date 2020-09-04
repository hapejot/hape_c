#include "platform.h"
#include "mem_int.h"

/*---------------------------------------------------------------------------
 * NAME
 *      mem_arena_new - 
 *
 * SYNOPSIS
 */
MEM_ARENA
mem_arena_new
(
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
    MEM_ARENA result = 0;
    INT status = RC_OK;
    /* end of local var. */

    /* parameter check */
    
    /* environment check */

    /* processing. */
    if ( status == RC_OK )
    {
        result = malloc(sizeof (*result));

        assert( result != NULL );

        result->prev  = NULL;
        result->limit = NULL;
        result->avail = NULL;
    }

    /* return */
    return result;
}

