#include "platform.h"
#include "io_int.h"
#include "err.h"
// #include <io.h>
#include <stdio.h>

/*---------------------------------------------------------------------------
 * NAME
 *      io_std_write - 
 *
 * SYNOPSIS
 */
INT
io_std_write
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

        n = write( p_io->pint, p_data, p_size );
        if( n >= 0 )
        {
            *p_len = n;
        }
        else
        {
            err_set_syserr( "write", __FILE__, __LINE__ );
            status = RC_ERROR;
        }
    }

    /* return */
    return status;
}

