#include "platform.h"
#include "cnt_int.h"
#include <stdio.h>

/*---------------------------------------------------------------------------
 * NAME
 *      cnt_copy_row - 
 *
 * SYNOPSIS
 */
INT
cnt_copy_row
(
    IN      CNT         p_cnt,
    IN      INT         p_rownum,
    IN      INT         p_dest_rownum,
    OUT     CNT         p_row
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
    INT result = 0;
    INT status = RC_OK;
    /* end of local var. */

    /* parameter check */
    
    /* environment check */

    /* processing. */
    if ( status == RC_OK )
    {
        INT         max_row     = 0;
        CNT_HEAD    col         = NULL;
        char      * val         = NULL;
        INT         size        = 0;
        INT         flg         = 0;

        cnt_get_dim( p_cnt, &max_row, NULL, NULL );
        assert( p_rownum <= max_row );


        col = p_cnt->first_col;
        while( col )
        {
            status = cnt_get_idx_val(p_cnt, p_rownum, col->name, &flg, &size, (void**)&val);
            if( status == RC_OK )
            {
                status = cnt_set_idx_val(p_row, p_dest_rownum, col->name, flg, size, val);
            }
            col = col->next;
        }
    }

    /* return */
    return result;
}

