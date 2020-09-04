#include "platform.h"
#include "cnt_int.h"

/*---------------------------------------------------------------------------
 * NAME
 *      cnt_next_name - 
 *
 * SYNOPSIS
 */
INT
cnt_next_name
(
    INOUT   void     ** p_crs,
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
    
    /* environment check */

    /* processing. */
    if ( status == RC_OK )
    {
        CNT_HEAD    hd = (CNT_HEAD)*p_crs;
        *p_head = hd;
        if( hd != NULL )
            *p_crs  = hd->next;
        else
            status = RC_ERROR;
    }

    /* return */
    return status;
}

