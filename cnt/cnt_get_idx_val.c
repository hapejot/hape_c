#include "platform.h"
#include "cnt_int.h"
#include <string.h>

/*---------------------------------------------------------------------------
 * NAME
 *      cnt_get_idx_val - 
 *
 * SYNOPSIS
 */
INT
cnt_get_idx_val
(
    CNT         p_cnt,
    INT         p_row,
    char      * p_field,
    INT       * p_flg,
    INT       * p_size,
    void     ** p_val
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
 *      RC_ERROR        Wenn nicht gefunden.
 *
 *---------------------------------------------------------------------------*/
{
    INT status = RC_OK;
    /* end of local var. */

    /* parameter check */
    assert( p_cnt != NULL );
    if( p_cnt->max_row > 10000 
            || p_cnt->max_col > 10000)
    {
        puts("\n-----");
        print_hex_dump( sizeof(*p_cnt), (CHAR*)p_cnt );
        puts("-----");
    }
    assert( p_cnt->max_row < 500000 );
    assert( p_cnt->max_col < 500000 );
    assert( 0 <= p_row );
    assert( p_field != NULL );
    
    /* environment check */

    /* processing. */
    if ( status == RC_OK )
    {
        CNT_HEAD    col         = p_cnt->first_col;
        INT         col_no      = -1;

        while( col )
        {
            col_no = col->col;
            assert( col->name != NULL );
            if( strcmp(col->name, p_field) == 0 )
                break;
            col = col->next;
        }

        if( col )
        {
            INT                 max         = p_cnt->count_cell;
            struct _cnt_cell    cell_data;
            CNT_CELL            cell        = &cell_data;
            CNT_CELL          * acell       = p_cnt->cell;
            INT                 min         = 0;
            INT                 pos         = -1;
            INT                 cmp         = 0;

            cell->row = p_row;
            cell->col = col_no;
            while( min < max )
            {
                pos = (min + max) / 2;
                cmp = cnt_cell_cmp(acell[pos], cell);
                switch( cmp )
                {
                case -1:
                    min = pos + 1;
                    break;
                case 0:
                    min = pos;
                    max = pos;
                    cell = acell[pos];
                    break;
                case 1:
                    max = pos;
                    break;
                }
            }

            if( cmp == 0 )
            {
                if( p_flg ) 
                    *p_flg  = cell->flg;
                if( p_size )
                    *p_size = cell->size;
                if( p_val )
                    *p_val  = cell->val;
                status  = RC_OK;
            }
            else
            {
                status = RC_ERROR;
            }
        }
        else
        {
            status = RC_ERROR;
        }

    }

    /* return */
    return status;
}

