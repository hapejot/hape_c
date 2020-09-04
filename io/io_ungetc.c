#include "platform.h"
#include "io_int.h"

/*---------------------------------------------------------------------------
 * NAME
 *      io_ungetc - 
 *
 * SYNOPSIS
 */
INT
io_ungetc
(
        IN      IO_FILE                 p_io,
        IN      INT                     p_char
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
    assert( p_io );
    assert( p_char > 0 );
    
    /* environment check */
    assert( p_io->unget_count < IO_UNGET_BUFSIZE );
    assert( p_io->unget_count >= 0 );

    /* processing. */
    if ( status == RC_OK )
    {
        p_io->unget_count++;
        p_io->unget_buffer[ IO_UNGET_BUFSIZE - p_io->unget_count ] = p_char;
    }

    /* return */
    return status;
}

