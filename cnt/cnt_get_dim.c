#include "platform.h"
#include "cnt_int.h"

/*---------------------------------------------------------------------------
 * NAME
 *      cnt_get_dim - 
 *
 * SYNOPSIS
 */
INT
cnt_get_dim
(
    IN  CNT     p_cnt,
    OUT INT   * p_rowcnt,
    OUT INT   * p_colcnt,
    OUT INT   * p_cellcnt
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
        CNT_CELL  * acell       = p_cnt->cell;
        INT         rowcnt      = -1;
        INT         colcnt      = -1;
        INT         cellcnt     = p_cnt->count_cell;
        INT         i           = 0;

        assert( acell != NULL );
        while( i < cellcnt )
        {
            if( rowcnt < acell[i]->row )
                rowcnt = acell[i]->row;
            if( colcnt < acell[i]->col )
                colcnt = acell[i]->col;
            i++;
        }

        if( p_rowcnt )  *p_rowcnt  = rowcnt  + 1;
        if( p_colcnt )  *p_colcnt  = colcnt  + 1;
        if( p_cellcnt ) *p_cellcnt = cellcnt + 1;

        status = RC_OK;
    }

    /* return */
    return status;
}

