#include "platform.h"
#include "cnt_int.h"

static MEM_ARENA g_arena = NULL;

/*---------------------------------------------------------------------------
 * NAME
 *      cnt_create - Create a container stored in a given memory arena.
 *
 * SYNOPSIS
 */
CNT
cnt_create
(
)
{
    if(!g_arena){
        g_arena = mem_arena_new();
    }

    return cnt_create_a(g_arena);
}

CNT cnt_create_a(MEM_ARENA p_arena)
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
 * 
 * RETURN VALUES
 *      p_cnt - returned pointer to the container created.
 *
 * ERRORS
 *
 *---------------------------------------------------------------------------*/
{
    /* environment check */
    CNT cnt = NULL;
    /* processing. */
        cnt = mem_arena_calloc(p_arena, sizeof(*cnt), 1, __FILE__, __LINE__ );
        assert(cnt != NULL);
        cnt->arena      = p_arena;
        cnt->cols  = ALLOC(p_arena, sizeof(*cnt->cols));
        cnt->cells  = ALLOC(p_arena, sizeof(*cnt->cells));

        cnt->cols->used = 0;
        cnt->cols->type = 0;
        cnt->cells->used = 0;
        cnt->cells->type = 0;


    /* return */
    return cnt;
}

