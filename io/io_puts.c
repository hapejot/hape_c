#include "platform.h"
#include "io_int.h"

/*---------------------------------------------------------------------------
 * NAME
 *      io_puts - write a 0-terminated string to the output stream
 *
 * SYNOPSIS
 */
INT
io_puts
(
    IN      IO_FILE     p_io,
    IN      char*       p_chars
)
/*
 * DESCRIPTION
 * 
 * PARAMETERS
 *      p_io - sream to write to.
 *      p_chars - string
 * 
 * RETURN VALUES
 *
 * ERRORS
 *      [RC_OK]         Successful completion.
 *
 *---------------------------------------------------------------------------*/
{
    INT     status  = RC_OK;

    assert(p_chars);
    assert( p_io != NULL );
    

    if ( status == RC_OK )
    {
        INT                 n;
        n = strlen(p_chars);

        assert( p_io->t != NULL );
        assert( p_io->t->write != NULL );
        status = (p_io->t->write)(p_io, p_chars, n, &n);
    }

    /* return */
    return status;
}

