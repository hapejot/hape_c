#include "platform.h"
#include "io_int.h"
#include <stdio.h>

/*---------------------------------------------------------------------------
 * NAME
 *      io_getc - get one character from file.
 *
 * SYNOPSIS
 */
INT
io_getc
(
    IN      IO_FILE         p_io
)
/*
 * DESCRIPTION
 *      uses the read procedure to get data from the file stream.
 * 
 * PARAMETERS
 *      p_io - file to read from.
 *
 * RETURN VALUES
 *      Character read or
 *      -1, when end of file reached.
 *      -2, when temporarily no char could be read.
 *
 * ERRORS
 *      [RC_OK]         Successful completion.
 *
 *---------------------------------------------------------------------------*/
{
    INT     result  = 0;
    INT     status  = RC_OK;
    /* end of local var. */

    /* parameter check */
    assert( p_io != NULL );
    
    /* environment check */

    /* processing. */
    if ( status == RC_OK )
    {
        assert( p_io->t != NULL );
        assert( p_io->t->read != NULL );
        if( p_io->unget_count > 0 )
        {
            result = p_io->unget_buffer[IO_UNGET_BUFSIZE - p_io->unget_count];
            p_io->unget_count --;
        }
        else
        {
            unsigned char       c;
            INT                 n;

            status = (p_io->t->read)(p_io, &c, sizeof(c), &n);
            if( status == RC_OK )
            {
                assert( n == sizeof(c) );
                result = c;
            }
            else
                result = -1;
        }
    }

    /* return */
    return result;
}

