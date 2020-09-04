#include "platform.h"
#include "io_int.h"

/*---------------------------------------------------------------------------
 * NAME
 *      io_buf_expand - 
 *
 * SYNOPSIS
 */
INT
io_buf_expand
(
    INOUT   IO_BUFFER       p_buf,
    IN      INT             p_size
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
    assert( p_buf != NULL );
    
    /* environment check */

    /* processing. */
    if ( status == RC_OK )
    {
        p_buf->pch = realloc(p_buf->pch, p_size);
        assert( p_buf->pch != NULL );
    }

    /* return */
    return status;
}

