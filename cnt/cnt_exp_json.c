#include "platform.h"
#include "cnt_int.h"
#include "fmt.h"
// für sprintf
#include <stdio.h>

static INT
single_exp(
        IN  IO_FILE     p_file,
        IN  CNT         p_cnt
)
{
    INT         status      = RC_OK;
    INT         max_row     = 0;
    INT         max_col     = 0;
    INT         row_no      = 0;
    CNT_HEAD    col         = NULL;

    assert( p_file      != NULL );
    assert( p_cnt       != NULL );

    status = cnt_get_dim( p_cnt, &max_row, &max_col, NULL );
    assert( max_row != 0 || max_col == 0 );
    assert( status == RC_OK );

    if( max_row > 1 )
        io_putc( p_file, '[' );

    for( row_no = 0; row_no < max_row; row_no++ )
    {
        io_putc( p_file, '{' );
        col = p_cnt->first_col;
        assert( col != NULL );
        while( col )
        {
            INT         etype;
            INT         elen;
            VOID      * eval;

            fmt_format( p_file, "%s: ", col->name );

            status = cnt_get_idx_val( p_cnt, row_no, col->name, &etype, &elen, &eval );
            if( RC_OK == status )
            {
                switch (etype)
                {
                    case CNT_SUBCONT:
                        single_exp( p_file, eval );
                        break;

                    case CNT_ATOM:
                        status = fmt_format(p_file, "\"%*.*s\"", elen, elen, eval);
                        break;
                }
            }
            col = col->next;
            if( col )
            {
                io_putc( p_file, ',' );
                io_putc( p_file, '\n' );
            }
        }
        io_putc( p_file, '}' );
        if( row_no > 0 && row_no < max_row - 1 )
        {
            io_putc( p_file, ',' );
            io_putc( p_file, '\n' );
        }
    }

    if( max_row > 1 )
        io_putc( p_file, ']' );

    return status;
}

/*---------------------------------------------------------------------------
 * NAME
 *      cnt_exp_json - 
 *
 * SYNOPSIS
 */
INT
cnt_exp_json
(
    IO_FILE     p_file,
    CNT         p_cnt
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
    assert( p_cnt != NULL );
    
    /* environment check */

    /* processing. */
    if ( status == RC_OK )
    {

        status = single_exp( p_file, p_cnt );
    }

    /* return */
    return status;
}

