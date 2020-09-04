#include "platform.h"
#include "fmt_int.h"
#include "io.h"
#include "err.h"
#include <limits.h>
#include <ctype.h>

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
