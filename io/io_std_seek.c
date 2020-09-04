#include "platform.h"
#include "io_int.h"
#include <stdio.h>

/*---------------------------------------------------------------------------
 * NAME
 *      io_std_seek - 
 *
 * SYNOPSIS
 */
INT
io_std_seek
(
    IN  IO_FILE     p_io,
    IN  UINT64      p_pos
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
    fprintf(stderr, "std_seek\n" );

    /* processing. */
    if ( status == RC_OK )
    {
		lseek( p_io->pint, p_pos, SEEK_SET );
    }

    /* return */
    return status;
}

