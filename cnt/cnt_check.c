#include "platform.h"
#include "cnt_int.h"

/*---------------------------------------------------------------------------
 * NAME
 *      cnt_check - calculates and compares check sum of container.
 *
 * SYNOPSIS
 */
INT
cnt_check
(
    IN      CNT     p_cnt
)
/*
 * DESCRIPTION
 *      First it calculates the checksum of the container base
 *      structure. Then it compares it against the existing 
 *      checksum.
 *      Finally the checksum is set to the calculated one.
 *      Upon return the result of the comparison is returned.
 * 
 * PARAMETERS
 * 
 * RETURN VALUES
 *
 * ERRORS
 *      [RC_OK]         Successful completion.
 *      [RC_ERROR]      Checksum missmatch.
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
        INT     cs_old = p_cnt->check_sum;
        INT     cs_new = 0;
        INT     i;
        INT     n;
        CHAR  * c;

        p_cnt->check_sum = 0;
        n = sizeof( *p_cnt );
        c = (CHAR*)p_cnt;
        for( i = 0; i < n; i++ )
            cs_new += *c++;
        if( cs_old == cs_new )
            status = RC_OK;
        else
        {
            status = RC_ERROR;
        }
        p_cnt->check_sum = cs_new;
    }

    /* return */
    return status;
}

