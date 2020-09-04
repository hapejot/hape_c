#include "platform.h"
#include "mem_int.h"
#include <malloc.h>
#include <assert.h>

/*---------------------------------------------------------------------------
 * NAME
 *      mem_arena_dispose - 
 *
 * SYNOPSIS
 */
INT
mem_arena_dispose
(
    INOUT   MEM_ARENA     * ap
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
    INT status = RC_OK;
    /* end of local var. */

    /* parameter check */
	assert(ap && *ap);
    
    /* environment check */

    /* processing. */
    if ( status == RC_OK )
    {
	    mem_arena_free(*ap);
        free(*ap);
        *ap = NULL;

        status = RC_OK;
    }

    /* return */
    return status;
}

