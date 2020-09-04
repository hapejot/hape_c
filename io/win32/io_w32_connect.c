#include "platform.h"
SCCSID(io_w32_connect, "$Id: //depot/peter/w/io/win32/io_w32_connect.c#1 $");

#include "io_int.h"

extern INT io_w32_read ( IO_FILE, void*, INT, INT* );
extern INT io_w32_write( IO_FILE, void*, INT, INT* );

static IO_CLASS   g_w32_file = { 
    io_w32_read,    /* read     */
    io_w32_write,   /* write    */
    NULL,           /* seek     */
    NULL,           /* tell     */
    NULL            /* close    */
    }
    ;

/*---------------------------------------------------------------------------
 * NAME
 *      io_w32_connect - 
 *
 * SYNOPSIS
 */
INT
io_w32_connect
(
    INOUT   IO_FILE     p_io,
    IN      HANDLE      p_handle
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
        IO_FILE     io;

        io = malloc(sizeof(*io));
        assert( io != NULL );
        io->t   = &g_w32_file;
        io->p   = p_handle;
    }

    /* return */
    return result;
}

