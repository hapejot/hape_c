#include "platform.h"
#include "io.h"

/*---------------------------------------------------------------------------
 * NAME
 *      io_gets - Read one string from input stream.
 *
 * SYNOPSIS
 */
INT
io_gets
(
    IN      IO_FILE         p_file,
    INOUT   INT8          * p_buf,
    IN      INT             p_bufsize,
    OUT     INT           * p_bytes
)
/*
 * DESCRIPTION
 * 
 * PARAMETERS
 *      p_file - input stream.
 *      p_buf - memory to store string.
 *      p_bufsize - amount of available space for storing the string.
 *      p_bytes - returns the number of bytes used for the string.
 * 
 * RETURN VALUES
 *      error code and the number of bytes copied.
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
        INT         i = 0;
        INT         c = 0;

        while( i < (p_bufsize - 1) )
        {
            c = io_getc( p_file );
            if( c == -1 )
                break;
            p_buf[i++] = (INT8)c;
            p_buf[i] = 0;
            if( c == '\n' )
                break;
        }
        *p_bytes = i;
        if( i == 0 && c == -1 )
            status = RC_IO_EOF;
    }

    /* return */
    return status;
}

