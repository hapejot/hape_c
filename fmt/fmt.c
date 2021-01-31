////////////////////////////////////////////////////////////////////////////////
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
////////////////////////////////////////////////////////////////////////////////
//
#include <hape/platform.h>
#include <hape/err.h>
#include <hape/io.h>
#include <hape/fmt.h>
#include "fmt_int.h"

////////////////////////////////////////////////////////////////////////////////
//
//

char g_fmt_flags[] = "l-+";
typedef INT (*FMT_CVT)(IO_FILE, va_list*, char*, INT, INT);
extern FMT_CVT     g_fmt_cvt[];

////////////////////////////////////////////////////////////////////////////////
//
INT fmt_vfmt( IN IO_FILE p_io, IN char *p_fmt, IN va_list * p_ap );

//
//
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


/*---------------------------------------------------------------------------
 * NAME
 *      fmt_vfmt - 
 *
 * SYNOPSIS
 */
INT fmt_vfmt( IN IO_FILE p_io, IN char *p_fmt, IN va_list * p_ap )
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
    if( status == RC_OK ) {
        assert( p_fmt != NULL );
        while( *p_fmt ) {
            if( *p_fmt != '%' || *++p_fmt == '%' )
                io_putc( p_io, ( unsigned char )*p_fmt++ );
            else {
                unsigned char c;
                char flags[256];
                int width = INT_MIN;
                int precision = INT_MIN;

                memset( flags, '\0', sizeof( flags ) );
                c = *p_fmt;

                for( ; c && strchr( g_fmt_flags, c ); c = *++p_fmt ) {
                    flags[c]++;
                }
                if( *p_fmt == '*' || isdigit( *p_fmt ) ) {
                    int n;

                    if( *p_fmt == '*' ) {
                        n = va_arg( *p_ap, int );
                        assert( n != INT_MIN );
                        p_fmt++;
                    }
                    else {
                        for( n = 0; isdigit( *p_fmt ); p_fmt++ ) {
                            int d = *p_fmt - '0';
                            assert( n <= ( INT_MAX - d ) / 10 );
                            n = 10 * n + d;
                        }
                    }
                    width = n;
                }
                if( *p_fmt == '.'
                    && ( *++p_fmt == '*' || isdigit( *p_fmt ) ) ) {
                    int n;

                    if( *p_fmt == '*' ) {
                        n = va_arg( *p_ap, int );
                        assert( n != INT_MIN );
                        p_fmt++;
                    }
                    else {
                        for( n = 0; isdigit( *p_fmt ); p_fmt++ ) {
                            int d = *p_fmt - '0';
                            assert( n <= ( INT_MAX - d ) / 10 );
                            n = 10 * n + d;
                        }
                    }
                    precision = n;
                }
                c = *p_fmt++;
                if( g_fmt_cvt[c] == NULL ) {
                    ERR_LOG_ENTRY e;
                    e.typ = 'E';
                    sprintf( e.pkg, "FMT" );
                    sprintf( e.id, "100" );
                    sprintf( e.msg, "format %c not defined.", c );
                    err_log_add( &e );

                    for( int i = 0; i < 256; i++ ) {
                        if( g_fmt_cvt[i] ) {
                            sprintf( e.msg, "format %c available.", i );
                            e.typ = 'I';
                            err_log_add( &e );
                        }
                    }

                    err_log_dump(  );
                }
                assert( g_fmt_cvt[c] );
                ( *g_fmt_cvt[c] ) ( p_io, p_ap, flags, width, precision );
            }
        }
    }

/* return */
    return result;
}



////////////////////////////////////////////////////////////////////////////////
FMT_CVT     g_fmt_cvt[256] = {
    /* 000 - 007 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 008 - 015 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 016 - 023 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 024 - 031 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 032 - 039 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 040 - 047 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 048 - 055 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 056 - 063 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 064 - 071 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 072 - 079 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 080 - 087 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 088 - 095 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 096 - 103 */ NULL,   NULL,   NULL,   NULL,   fmt_cvt_number,   NULL,   NULL,   NULL,
    /* 104 - 111 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 112 - 119 */ NULL,   NULL,   NULL, fmt_cvt_string,   NULL,   NULL,   NULL,   NULL,
    /* 120 - 127 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
};
//
//
//
