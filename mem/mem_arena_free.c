#include "platform.h"
#include "mem_int.h"
#include <malloc.h>
#include <assert.h>

/*---------------------------------------------------------------------------
 * NAME
 *      mem_arena_free - 
 *
 * SYNOPSIS
 */
INT
mem_arena_free
(
    IN      MEM_ARENA       arena
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
    
    /* environment check */

    /* processing. */
    if ( status == RC_OK )
    {
        assert(arena);
        while (arena->prev) 
        {
            struct MEM_ARENA tmp = *arena->prev;
            free(arena->prev);
            *arena = tmp;
        }
        assert(arena->limit == NULL);
        assert(arena->avail == NULL);
    }

    /* return */
    return status;
}

