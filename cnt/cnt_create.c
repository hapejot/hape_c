#include "platform.h"
#include "cnt_int.h"

/*---------------------------------------------------------------------------
 * NAME
 *      cnt_create - Create a container stored in a given memory arena.
 *
 * SYNOPSIS
 */
INT
cnt_create
(
    MEM_ARENA   p_arena,
    CNT       * p_cnt
)
/*
 * DESCRIPTION
 *      The container will be allocated using the given memory arena.
 *      The arena may be dropped at once, all the containers and values
 *      that are associated with it will be dropped as well.
 *      The continer values and subcontainers are automatically stored
 *      in the same arena as the main container.
 * 
 * PARAMETERS
 *      p_arena - pointer to arena to use.
 *      p_cnt - returned pointer to the container created.
 * 
 * RETURN VALUES
 *      Returns the status code.
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
        CNT cnt = mem_arena_calloc(p_arena, sizeof(*cnt), 1, __FILE__, __LINE__ );
        assert(cnt != NULL);
        cnt->arena      = p_arena;
        cnt->max_cell   = 4;
        cnt->count_cell = 0;
        cnt->cell       = mem_arena_calloc(cnt->arena, sizeof(CNT_CELL), cnt->max_cell, __FILE__, __LINE__);
        cnt->first_col  = NULL;

        cnt_check( cnt );
        *p_cnt = cnt;
        status = RC_OK;
    }

    /* return */
    return status;
}

