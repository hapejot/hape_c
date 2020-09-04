#include "platform.h"
#include "io_int.h"

/*---------------------------------------------------------------------------
 * NAME
 *      io_read - read a block of characters from a stream.
 *
 * SYNOPSIS
 */
INT
io_read
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
 *      p_io - input stream.
 *      p_data - data to store the data.
 *      p_len - amount of space available for storing the data.
 *      p_n - number of bytes acually stored.
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
    assert( p_io != NULL );
    assert( p_io->t != NULL );
    assert( p_io->t->read != NULL );
    assert( p_data != NULL );
    assert( p_len >= 0 );

    /* processing. */
    if ( status == RC_OK )
    {
        INT     n   = 0;
        INT     read_count;

        if( p_io->unget_count > 0 )
        {
            if( p_io->unget_count >= p_len )
            {
                memcpy( p_data, 
                        p_io->unget_buffer + IO_UNGET_BUFSIZE - p_io->unget_count,
                        p_len );
                p_io->unget_count -= p_len;
                n = p_len;
            }
            else
            {
                memcpy( p_data,
                        p_io->unget_buffer + IO_UNGET_BUFSIZE - p_io->unget_count,
                        p_io->unget_count );
                n = p_io->unget_count;
                p_io->unget_count = 0;
            }
        }
        if( n < p_len )
        {
            status = (p_io->t->read)(p_io, ((CHAR*)p_data) + n, p_len - n, &read_count);
            n += read_count;
        }
        assert( p_len >= n );
        *p_n = n;
    }

    /* return */
    return status;
}

