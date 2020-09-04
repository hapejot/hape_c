#include "platform.h"
#include "cnt_int.h"

/*---------------------------------------------------------------------------
 * NAME
 *      cnt_first_name - Liefert den ersten Spaltenkopf und einen Cursor.
 *
 * SYNOPSIS
 */
INT
cnt_first_name
(
    IN      CNT         p_cnt,
    OUT     void     ** p_crs,
    OUT     CNT_HEAD  * p_head
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
    if( p_cnt == NULL || p_head == NULL || p_crs == NULL )
        status = RC_ERROR;
    
    /* environment check */

    /* processing. */
    if ( status == RC_OK )
    {
        CNT_HEAD        hd = p_cnt->first_col;

        if( hd != NULL )
            *p_crs = hd->next;
        else
            *p_crs = NULL;
        *p_head = hd;
    }

    /* return */
    return status;
}

