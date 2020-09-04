#include "platform.h"
#include "io_int.h"
// #include <io.h>

/*---------------------------------------------------------------------------
 * NAME
 *      io_std_read - 
 *
 * SYNOPSIS
 */
INT
io_std_read
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
    INT status = RC_OK;
    /* end of local var. */

    /* parameter check */
    
    /* environment check */

    /* processing. */
    if ( status == RC_OK )
    {
        INT     n;

        n = read( p_io->pint, p_data, p_size );
        if( n >= 0 )
        {
            *p_len = n;
            if( n == 0 )
                status = RC_IO_EOF;
        }
        else
        {
            status = RC_ERROR;
        }
    }

    /* return */
    return status;
}

