#include "platform.h"
#include "io_int.h"

/*---------------------------------------------------------------------------
 * NAME
 *      io_buf_new - 
 *
 * SYNOPSIS
 */
INT
io_buf_new
(
    OUT     IO_BUFFER * p_buf
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
        IO_BUFFER               iobuf;
        iobuf                   = malloc(sizeof(*iobuf));
        assert( iobuf != NULL );
        iobuf->len              = 0;
        iobuf->size             = 20000;
        iobuf->pch              = malloc(iobuf->size);
        assert( iobuf->pch != NULL );
        iobuf->pch[iobuf->len]  = 0;
        iobuf->next             = NULL;
        iobuf->prev             = NULL;

        *p_buf = iobuf;
    }

    /* return */
    return status;
}

