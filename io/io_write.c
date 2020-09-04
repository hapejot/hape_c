#include "platform.h"
#include "io_int.h"

/*---------------------------------------------------------------------------
 * NAME
 *      io_write - write data to stream.
 *
 * SYNOPSIS
 */
INT
io_write
(
    IN      IO_FILE         p_io,
    IN      void          * p_data,
    IN      INT             p_len,
    OUT     INT           * p_n
)
/*
 * DESCRIPTION
 * 
 * PARAMETERS
 *      p_io - output stream.
 *      p_data - data to write.
 *      p_len - size of data to write.
 *      p_n - return value of acutally written amount of data.
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
    
    /* environment check */
    assert( p_io != NULL );
    assert( p_io->t != NULL );
    assert( p_io->t->write != NULL );

    /* processing. */
    if ( status == RC_OK )
    {
        INT         len = p_len;
        *p_n = 0;
        if( len == -1 )
            len = strlen(p_data);
        status = (p_io->t->write)(p_io, p_data, len, p_n);
        assert( len >= *p_n );
    }

    /* return */
    return status;
}

