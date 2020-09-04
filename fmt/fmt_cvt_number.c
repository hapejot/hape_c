#include "platform.h"
#include "io.h"
#include "fmt_int.h"
/*---------------------------------------------------------------------------
 * NAME
 *      fmt_cvt_number - 
 *
 * SYNOPSIS
 */

void out_leader( IO_FILE p_io, int w, char leader ) {
    if( w > 1 ) {
        out_leader( p_io, w - 1, leader );
    }
    io_putc( p_io, leader );
}
void out_digit( IO_FILE p_io, int n, int w, char leader ) {
    if( n > 10 )
        out_digit( p_io, n / 10, w - 1, leader );
    if( w > 1 && n < 10 )
        out_leader( p_io, w - 1, leader );
    io_putc( p_io, '0' + ( n % 10 ) );
}

INT fmt_cvt_number
        ( IN IO_FILE p_io,
          IN va_list * p_ap,
          IN char *p_flags, IN INT p_width, IN INT p_scale )
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
    int num;
    INT len = 1;
/* end of local var. */
/* parameter check */
    assert( p_width >= 0 || p_width == INT_MIN );
    assert( p_flags );
    num = va_arg( *p_ap, int );
/* environment check */

/* processing. */
    if( status == RC_OK ) {
        if( p_width == INT_MIN )
            p_width = 0;
        if( p_width < 0 ) {
            p_flags['-'] = 1;
            p_width = -p_width;
        }
        if( p_scale >= 0 )
            p_flags['0'] = 0;
        if( p_scale >= 0 && p_scale < len )
            len = p_scale;
        out_digit( p_io, num, p_width, ' ' );
    }

/* return */
    return status;
}
