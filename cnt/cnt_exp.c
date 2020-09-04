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
    INT         i           = 0;
    CNT_HEAD    col         = NULL;
    char        name[1024]  = "";   
    INT         cell_cnt    = 0;
    CNT_CELL  * cell_list   = NULL;

    assert( p_file      != NULL );
    assert( p_prefix    != NULL );
    assert( p_cnt       != NULL );

    status = cnt_get_dim( p_cnt, &max_row, &max_col, NULL );
    assert( max_row != 0 || max_col == 0 );
    assert( status == RC_OK );

    cell_cnt  = p_cnt->count_cell;
    cell_list = p_cnt->cell;

    for( i=0; i < cell_cnt; i++ )
    {
        CNT_CELL    cell        = cell_list[i];
        INT         col_no;

        assert( cell != NULL );

        col_no = cell->col;

        col = p_cnt->first_col;
        assert( col != NULL );
        while( col_no )
        {
            col = col->next;
            col_no--;
            assert( col != NULL );
        }
        assert( col->name != NULL );
        if( max_row > 1 )
        {
            sprintf( name, "%s[%ld].%s", p_prefix, cell->row, col->name );
        }
        else
        {
            sprintf( name, "%s%s%s", p_prefix, 
                                (p_prefix[0] == 0) ? "": ".",
                                col->name );
        }
        if( CNT_ISCONT(cell->flg) )
        {
            status = single_exp( p_file, name, (CNT) cell->val );
            assert( status == RC_OK );
        }
        else
        {
            if ( cell->size == 0 )
            {
                char        num[30];

                sprintf(num, "0x%p", cell->val);
                status = fmt_format(p_file, "%s=%s\n", name, num);
            }
            else
            {
                status = fmt_format(p_file, "%s=%.*s\n", name, cell->size, cell->val);
            }
            assert( status == RC_OK );
        }
    }

    return status;
}

/*---------------------------------------------------------------------------
 * NAME
 *      cnt_exp - 
 *
 * SYNOPSIS
 */
INT
cnt_exp
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

        status = single_exp( p_file, "", p_cnt );
        io_write(p_file, ".\n", -1, &n );
    }

    /* return */
    return status;
}

