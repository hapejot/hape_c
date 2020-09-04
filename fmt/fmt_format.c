#include "platform.h"
#include <stdarg.h>
#include "fmt_int.h"

/*---------------------------------------------------------------------------
 * NAME
 *      fmt_format - 
 *
 * SYNOPSIS
 */
INT
fmt_format
(
    IN      IO_FILE     p_io,
    IN      char      * p_fmt,
    ...
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
    
    /* environment check */

    /* processing. */
    if ( status == RC_OK )
    {
        va_list ap;
        va_start(ap, p_fmt);
	    status = fmt_vfmt(p_io, p_fmt, &ap);
        va_end(ap);
    }

    /* return */
    return status;
}

