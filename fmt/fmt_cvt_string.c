#include "platform.h"
#include "fmt_int.h"
#include "io.h"
#include <stdarg.h>
#include <limits.h>
/*---------------------------------------------------------------------------
 * NAME
 *      fmt_cvt_string - 
 *
 * SYNOPSIS
 */
INT
fmt_cvt_string
(
    IN      IO_FILE     p_io,
    IN      va_list   * p_ap,
    IN      char      * p_flags,
    IN      INT         p_width,
    IN      INT         p_scale
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
    INT     status  = RC_OK;
	char  * str     = NULL;
    INT     len     = 0;
    /* end of local var. */
    /* parameter check */
    assert( p_width >= 0 || p_width == INT_MIN );
    assert(p_flags);
    str = va_arg(*p_ap, char *);
    assert( str );
    len = strlen(str);
    /* environment check */

    /* processing. */
    if ( status == RC_OK )
    {
        INT     i;

        if (p_width == INT_MIN)
            p_width = 0;
        if (p_width < 0) 
        {
            p_flags['-'] = 1;
            p_width = -p_width;
        }
        if (p_scale >= 0)
            p_flags['0'] = 0;
        if (p_scale >= 0 && p_scale < len)
            len = p_scale;
        if (!p_flags['-'])
        {
            int nn = p_width - len; 
	        while (nn-- > 0) 
		        io_putc(p_io, ' '); 
        }
        for (i = 0; i < len; i++)
        {
            io_putc( p_io, (unsigned char)*str++ );
        }
        if ( p_flags['-'])
        {
            int nn = p_width - len; 
	        while (nn-- > 0) 
		        io_putc(p_io, ' '); 
        }
    }

    /* return */
    return status;
}

