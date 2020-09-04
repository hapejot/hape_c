#include "platform.h"
SCCSID(io_w32_read, "$Id: //depot/peter/w/io/win32/io_w32_read.c#1 $");
#include "io_int.h"

/*---------------------------------------------------------------------------
 * NAME
 *      io_w32_read - 
 *
 * SYNOPSIS
 */
INT
io_w32_read
(
    IN      IO_FILE     p_io,
    IN      void      * p_data,
    IN      INT         p_size,
    OUT     INT       * p_len
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
        INT     n;

        n = ReadFile( (HANDLE)p_io->p, p_data, p_size, &n, NULL );
        if( n >= 0 )
        {
            *p_len = n;
        }
        else
        {
            status = RC_ERROR;
        }
        assert( n > 0 );

    }

    /* return */
    return result;
}

