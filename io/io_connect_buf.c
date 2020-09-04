#include "platform.h"
#include "io_int.h"

static IO_CLASS   g_io_buffer = {
    io_buf_read,    /* read     */
    io_buf_write,   /* write    */
    NULL,           /* seek     */
    NULL,           /* tell     */
    NULL,           /* close    */
    NULL,           /* get_attr */
    }
    ;


/*---------------------------------------------------------------------------
 * NAME
 *      io_connect_buf - create a file based on an IO_BUFFER.
 *
 * SYNOPSIS
 */
INT
io_connect_buf
(
    INOUT   IO_FILE   * p_file,
    IN      IO_BUFFER   p_buf
)
/*
 * DESCRIPTION
 *      The IO_FILE that is created will wrap a IO_BUFFER structure
 *      as the file implementation. Every read or write operation will go
 *      to the buffer. And so all operations go into memory only.
 *      This is an easy method create a string by using io procedures
 *      only. The fmt module relies on this.
 * 
 * PARAMETERS
 *      p_file - file structure to be initialized. The structure will
 *              be allocated by this function. It will be released 
 *              upon an io_close call.
 *
 *      p_buf - buffer to wrap with a file structure. The buffer must
 *              be initialized and fully functional.
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
        // pint - wird als Bufferpointer zweckentfremdet.
        // p - wird für den IO_BUFFER verwendet.
        IO_FILE     io;

        io = malloc(sizeof(*io));
        assert( io != NULL );

        io->t           = &g_io_buffer;
        io->pint        = 0;
        io->p           = p_buf;
        io->unget_count = 0;

        *p_file = io;
    }

    /* return */
    return status;
}

