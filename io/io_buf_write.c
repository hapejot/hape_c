#include "platform.h"
#include "io_int.h"

/*---------------------------------------------------------------------------
 * NAME
 *      io_buf_write - 
 *
 * SYNOPSIS
 */
INT
io_buf_write
(
    IN      IO_FILE         p_file,
    IN      void          * p_data,
    IN      INT             p_size,
    IN      INT           * p_count
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
    assert( p_file != NULL );
    assert( p_file->p != NULL );
    
    /* environment check */

    /* processing. */
    if ( status == RC_OK )
    {
        IO_BUFFER       buf         = p_file->p;
        INT             free_space  = 0;
        INT             len         = 0;

        if( p_size == -1 )
            len = strlen(p_data) + 1;
        else
            len = p_size;

        free_space = buf->size - buf->len;
        while( free_space < len )
        {
            io_buf_expand( buf, buf->size * 2 );
            free_space = buf->size - buf->len;
        }
        memcpy( buf->pch + buf->len, p_data, len );
        buf->len += len;

        *p_count = len;
    }

    /* return */
    return status;
}

