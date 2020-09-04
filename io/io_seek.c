#include "platform.h"
#include "io_int.h"

/*---------------------------------------------------------------------------
 * NAME
 *      io_seek -  seek to a specific file position.
 *
 * SYNOPSIS
 */
INT
io_seek
(
    INOUT   IO_FILE     p_io,
    IN      UINT64      p_pos
)
/*
 * DESCRIPTION
 * 
 * PARAMETERS
 *      p_io - file stream.
 *      p_pos - byte position form the beginning of the stream.
 * 
 * RETURN VALUES
 *      error code.
 *
 * ERRORS
 *      [RC_OK]         Successful completion.
 *
 *---------------------------------------------------------------------------*/
{
    INT status = RC_OK;
    /* end of local var. */

    /* parameter check */
    assert( p_io != NULL );
    assert( p_io->t != NULL );
    assert( p_io->t->seek != NULL );
    
    /* environment check */

    /* processing. */
    if ( status == RC_OK )
    {
        status = (p_io->t->seek)(p_io, p_pos);
        p_io->unget_count = 0;
    }

    /* return */
    return status;
}

