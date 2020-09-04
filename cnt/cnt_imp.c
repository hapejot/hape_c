#include "platform.h"
#include "cnt_int.h"
#include <string.h>
#include <stdio.h>
/*---------------------------------------------------------------------------
 * NAME
 *      cnt_imp - 
 *
 * SYNOPSIS
 */
INT
cnt_imp
(
    IN      IO_FILE     p_file,
    INOUT   CNT         p_cnt
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
        char  * linebuf         = NULL;
        INT     linesize        = 80;
        char  * line            = NULL;
        char  * pos             = NULL;
        INT     n               = 0;        /* länge der Zeile */
        INT     n_old           = 0;
        char  * val             = NULL;
        char  * name            = NULL;

        linebuf = malloc(linesize);
        assert( linebuf );

        while( status == RC_OK )
        {
            n_old = 0;
            do 
            {
                status = io_gets( p_file, linebuf + n_old, linesize - n_old, &n );
                n_old += n;
                if( linebuf[n_old - 1] == '\n' )
                    break;
                if( n_old >= linesize - 10 )
                {
                    linesize *= 2;
                    linebuf = realloc( linebuf, linesize );
                }
            } while( RC_OK == status );
            if( n > 1000 )
                fprintf(stderr, "line length: %ld\n", n );
            if( RC_OK != status )
                break;
            line = linebuf;
            if( n == 0 )
                break;
            if( line[n-1] == '\n' )
                line[--n] = 0;
            if( n == 0 )
                continue;
            if( line[n-1] == '\r' )
                line[--n] = 0;

            if ( n == 0 )                   /* Leerzeile */
                continue;

            if( n == 1 && line[0] == '.' )  /* Ende Markierung */
                break;

            if( line[0] == '#' )            /* Kommentar */
                continue;

            pos = strchr(line, '=');
            if( pos == NULL )
                continue;
            *pos = 0;
            val = pos+1;
            name = line;
            status = cnt_set_val(p_cnt, name,  0, -1, val);
        }
        free( linebuf );
    }

    /* return */
    return status;
}

