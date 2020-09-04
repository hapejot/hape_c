#include "platform.h"
#include "cnt_int.h"

/*---------------------------------------------------------------------------
 * NAME
 *      cnt_cell_cmp - [INTERNAL] Compares the column and row numbers of two cells.
 *
 * SYNOPSIS
 */
INT
cnt_cell_cmp
(
    CNT_CELL     a,
    CNT_CELL     b
)
/*
 * PARAMETERS
 *
 * DESCRIPTION
 *      This procedure is for internal use, since the cells are ordered
 *      by row, col numbers in a binary list. This is an implementation
 *      detail that shouldn't be used when programming with containers.
 * 
 * RETURN VALUES
 *       1 - a > b
 *       0 - a == b
 *      -1 - a < b
 *
 *
 *---------------------------------------------------------------------------*/
{
    INT     result  = 1;

    if( a->row < b->row )
        result = -1;
    else if( a->row == b->row )
    {
        if( a->col < b->col )
            result = -1;
        else if( a->col == b->col )
            result = 0;
    }

    /* return */
    return result;
}

