#include "platform.h"
#include "cnt_int.h"
#include "io.h"
#include <malloc.h>

/*---------------------------------------------------------------------------
 * NAME
 *      cnt_from_csv - 
 *
 * SYNOPSIS
 */
INT
cnt_from_csv
(
    IN      IO_FILE         p_file,
    IN      CNT_CSVOPTS     p_opts,
    INOUT   CNT             p_cnt
)
/*
 * DESCRIPTION
 * 
 * PARAMETERS
 * 
 * RETURN VALUES
 *
 * ERRORS
 *      RC_OK           Successful completion.
 *
 *---------------------------------------------------------------------------*/
{
    INT                         status          = RC_OK;
    /* end of local var. */

    /* parameter check */
    
    /* environment check */

    /* processing. */
    if ( status == RC_OK )
    {
        CHAR                  * dbuf            = NULL;
        INT                     dbufsize        = 200;
        INT                     string_mode     = 0;
        INT                     pos             = 0;
        INT                     row_no          = 0;
        INT                     field_no        = 0;
        INT                     c;
        INT                     flg;
        INT                     size;
        void                  * val;
        char                  * t;

        /* 
         * Zeile in Felder aufspalten. 
         * Felder werden durch ',' getrennt.
         * Strings sind zusätzlich in '"' eingeschlossen, für den Fall, 
         * daß sie ',' enthalten. Wenn sie auch noch '"' enthalten, dann
         * muß da eigentlich '\"' stehen. 
         */
        dbuf = malloc( dbufsize );
        assert( dbuf != NULL );
        for(;;)
        {
            if( pos >= dbufsize - 1 )
            {
                dbufsize *= 2;
                dbuf = realloc( dbuf, dbufsize );
                assert( dbuf != NULL );
                assert( dbufsize - 1 > pos );
            }
            assert( pos < dbufsize - 1 );
            do
            {
                c = io_getc( p_file );
            } while( c == '\r' );
            if( c == -1 )
                break;
            if( c == p_opts.sep_char || c == '\n' )
            {
                if (string_mode)
                    dbuf[pos++] = (char)c;
                else
                {
                    dbuf[pos++] = '\0';
                    field_no++;
                    if( row_no == 0 && !p_opts.no_title )
                    {
                        char    elembuf[100];

                        sprintf( elembuf, "meta[%ld].name", field_no );
                        assert( strlen(elembuf) < sizeof(elembuf) );
                        status = cnt_set_val(p_cnt, elembuf,  0, -1, dbuf );
                    }
                    else
                    {
                        char    elembuf[100];

                        sprintf( elembuf, "meta[%ld].name", field_no );
                        assert( strlen(elembuf) < sizeof(elembuf) );
                        status = cnt_get_val( p_cnt, elembuf, &flg, &size, &val );
                        if( status == RC_OK )
                            sprintf( elembuf, "row[%ld].%s", row_no, (char*)val);
                        else
                            sprintf( elembuf, "row[%ld].f%ld", row_no, field_no );
                        assert( strlen(elembuf) < sizeof(elembuf) );
                        status = cnt_set_val(p_cnt, elembuf,  0, -1, dbuf );
                        assert( status == RC_OK );
                    }
                    if( c == '\n' )
                    {
                        row_no++;
                        field_no = 0;
                    }
                    pos = 0;
                };
            }
            else if( c == '"' )
            {
                string_mode = !string_mode;
            }
            else if( c == '\\' )
            {
                c = io_getc( p_file );
                t = strchr ( "b\bf\fn\nr\rt\t", (char)c );
                dbuf[pos++] = (char)(t ? t[1] : c);
            }
            else
            {
                dbuf[pos++] = (char)c;
            }
        }
    }

    /* return */
    return status;
}

