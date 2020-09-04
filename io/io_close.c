#include "platform.h"
#include "io_int.h"

/*---------------------------------------------------------------------------
 * NAME
 *      io_close - 
 *
 * SYNOPSIS
 */
INT
io_close
(
    INOUT   IO_FILE   * p_io
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
        assert( (*p_io)->t->close != NULL );
        status = ((*p_io)->t->close)(*p_io);
        free( *p_io );
        *p_io = NULL;

        status = RC_OK;
    }

    /* return */
    return status;
}

