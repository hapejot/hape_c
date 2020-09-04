#include "platform.h"
#include "cnt_int.h"
#include "fmt.h"
// für sprintf
#include <stdio.h>

static INT
single_exp(
        IN  IO_FILE     p_file,
        IN  char      * p_prefix,
        IN  CNT         p_cnt
)
{
    INT         status      = RC_OK;
    INT         max_row     = 0;
    INT         max_col     = 0;
    INT         row_no      = 0;
    CNT_HEAD    col         = NULL;

    assert( p_file      != NULL );
    assert( p_prefix    != NULL );
    assert( p_cnt       != NULL );

    status = cnt_get_dim( p_cnt, &max_row, &max_col, NULL );
    assert( max_row != 0 || max_col == 0 );
    assert( status == RC_OK );

    for( row_no = 0; row_no < max_row; row_no++ )
    {
        status = fmt_format(p_file, "(%s\n", p_prefix);
        col = p_cnt->first_col;
        assert( col != NULL );
        while( col )
        {
            INT         etype;
            INT         elen;
            VOID      * eval;

            status = cnt_get_idx_val( p_cnt, row_no, col->name, &etype, &elen, &eval );
            if( RC_OK == status )
            {
                switch (etype)
                {
                    case CNT_SUBCONT:
                        single_exp( p_file, col->name, eval );
                        break;

                    case CNT_ATOM:
                        status = fmt_format(p_file, "A%s CDATA %*.*s\n", col->name, elen, elen, eval);
                        break;
                }
            }
            col = col->next;
        }
        status = fmt_format(p_file, ")%s\n", p_prefix);
    }

    return status;
}

/*---------------------------------------------------------------------------
 * NAME
 *      cnt_exp_par - 
 *
 * SYNOPSIS
 */
INT
cnt_exp_par
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
        INT         n;

        status = single_exp( p_file, "DATA", p_cnt );
        io_write(p_file, "C\n", -1, &n );
    }

    /* return */
    return status;
}

