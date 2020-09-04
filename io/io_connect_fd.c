#include "platform.h"
#include "io_int.h"

static IO_CLASS   g_io_file = { 
    io_std_read,    /* read     */
    io_std_write,   /* write    */
    io_std_seek,    /* seek     */
    NULL,           /* tell     */
    io_std_close,    /* close    */
    NULL
    }
    ;
/*---------------------------------------------------------------------------
 * NAME
 *      io_connect_fd - Wraps the fopen function of stdio.
 *
 * SYNOPSIS
 */
INT
io_connect_fd
(
    IN  IO_FILE       * p_file,
    IN  INT             p_fd
)
/*
 * DESCRIPTION
 * 
 * PARAMETERS
 *      p_file - File Handle that will be generated.
 *      p_fd - abstract file descriptor.
 * 
 * RETURN VALUES
 *      Error Status
 *
 * ERRORS
 *      [RC_OK]             Successful completion.
 *      RC_IO_NOSUCHFILE    The file doesn't exist.
 *      RC_IO_NOPERM        The process doesn't have the permissions to open the
 *                          file with the given mode. It might be working in
 *                          another mode though.
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
        IO_FILE     io;

        io = malloc(sizeof(*io));
        assert( io != NULL );

        io->t           = &g_io_file;
        io->pint        = p_fd;
        io->unget_count = 0;

        *p_file = io;
    }

    /* return */
    return status;
}

