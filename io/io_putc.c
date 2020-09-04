#include "platform.h"
#include "io_int.h"

/*---------------------------------------------------------------------------
 * NAME
 *      io_putc - write a character to a stream.
 *
 * SYNOPSIS
 */
INT
io_putc
(
    IN      IO_FILE     p_io,
    IN      INT         p_char
)
/*
 * DESCRIPTION
 * 
 * PARAMETERS
 *      p_io - sream to write to.
 *      p_char - character to write.
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
    INT     status  = RC_OK;
    /* end of local var. */

    /* parameter check */
    assert( p_io != NULL );
    
    /* environment check */

    /* processing. */
    if ( status == RC_OK )
    {
        unsigned char       c;
        INT                 n;

        c = p_char;

        assert( p_io->t != NULL );
        assert( p_io->t->write != NULL );
        status = (p_io->t->write)(p_io, &c, sizeof(c), &n);
    }

    /* return */
    return status;
}

