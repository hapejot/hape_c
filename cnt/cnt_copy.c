#include "platform.h"
#include "cnt_int.h"
#include <stdio.h>

/*---------------------------------------------------------------------------
 * NAME
 *      cnt_copy - copies a container src deeply.
 *
 * SYNOPSIS
 */
INT
cnt_copy
(
    IN      CNT     p_src,
    INOUT   CNT     p_dst
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
    static INT     level    = 0;
    /* end of local var. */

    /* parameter check */
    
    /* environment check */
    level++;

    /* processing. */
    if ( status == RC_OK )
    {
        INT         n;
        char     ** colname;
        CNT_HEAD    col;
        INT         i;

        if( p_src->cell != NULL )
        {

            cnt_get_dim( p_src, NULL, &n, NULL );
            // fprintf( stderr, "\nCNT_COPY[%d]: %d source columns, %d cells from %p to %p", level, n, p_src->count_cell, p_src, p_dst );

            if( p_src->count_cell )
            {
                colname = malloc( sizeof(colname[0]) * n);
                col = p_src->first_col;
                i   = 0;
                while( col )
                {
                    assert( i < n );
                    colname[i] = col->name;
                    col = col->next;
                    i++;
                }

                for( i = p_src->count_cell - 1; i >= 0; i-- )
                {
                    CNT_CELL    c = p_src->cell[i];
                    if( CNT_ISCONT(c->flg) )
                    {
                        CNT     src     = c->val;
                        CNT     tar     = NULL;

                        // if( src->arena == p_dst->arena )
                        //     tar = src;
                        // else
                        {
                            cnt_create( p_dst->arena, &tar );
                            // fprintf( stderr, "\nCNT_COPY[%d]: create cnt", level );
                            cnt_copy( src, tar );
                        }
                        cnt_set_idx_val( p_dst, c->row, colname[c->col], 
                                        c->flg, 0, tar );
                        // fprintf( stderr, "\nCNT_COPY[%d]: %p[%d].%s=%p", level, p_dst, c->row, colname[c->col], tar );
                    }
                    else
                    {
                        cnt_set_idx_val( p_dst, c->row, colname[c->col], 
                                        c->flg, c->size, c->val );
                        // fprintf( stderr, "\nCNT_COPY[%d]: %p[%d].%s=%s",level, p_dst, c->row, colname[c->col], c->val );
                    }
                } 
                free( colname );
            }
        }

        // ensure that the checksum of the resulting container is ok.
        cnt_check( p_dst );
    }

    level--;
    /* return */
    return status;
}

