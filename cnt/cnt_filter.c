#include "platform.h"
#include "cnt_int.h"

/*---------------------------------------------------------------------------
 * NAME
 *      cnt_filter - Filters rows from one container by values in another.
 *
 * SYNOPSIS
 */
INT
cnt_filter
(
    IN      CNT         p_in,
    IN      CNT         p_filter,
    INOUT   CNT         p_out
)
/*
 * DESCRIPTION
 * 
 * PARAMETERS
 *      p_in - Source container with all data that is to be filtered.
 *      p_filter - filter values. Only the rows with the same names
 *              and identical values as in the filter are copied.
 *      p_out - the output container needs to be allocated before calling
 *              this procedure. The matching rows will be copied into this
 *              container.
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
        INT     row_cnt;
        INT     i;
        INT     j           = 0;

        cnt_get_dim( p_in, &row_cnt, NULL, NULL );
        for( i = 0; i < row_cnt; i++ )
        {
            CNT_HEAD    col;
            void      * crs;
            char      * v_in;
            char      * v_filter;
            INT         equal       = 1;

            cnt_first_name( p_filter, &crs, &col );
            while( col )
            {
                cnt_get_idx_val( p_in, i, col->name, NULL, NULL, (void**)&v_in );
                cnt_get_val( p_filter, col->name, NULL, NULL, (void**)&v_filter );
                if( strcmp(v_in, v_filter) )
                {
                    equal = 0;
                    break;
                }
            }
            if( equal )
            {
                cnt_copy_row( p_in, i, j++, p_out );
            }
        }
    }

    /* return */
    return status;
}

