#include "platform.h"
#include "cnt_int.h"
#include <string.h>

/*---------------------------------------------------------------------------
 * NAME
 *      cnt_set_idx_val - 
 *
 * SYNOPSIS
 */
INT
cnt_set_idx_val
(
    CNT         p_cnt,
    INT         p_row,
    char      * p_field,
    INT         p_flg,
    INT         p_size,
    void      * p_val
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
        struct _cnt_cell    cell_data;
        CNT_CELL    cell    = &cell_data; 
        CNT_CELL  * acell   = p_cnt->cell;
        CNT_HEAD  * colp    = &p_cnt->first_col;
        CNT_HEAD    col     = NULL;
        INT         col_no  = -1;
        INT         min     = 0;
        INT         max     = 0;
        INT         cmp     = -1;
        INT         pos     = 0;


        assert( *colp == NULL || (*colp)->name != NULL );
        while( *colp )
        {
            col_no = (*colp)->col;
            if( strcmp((*colp)->name,p_field) == 0 )
                break;
            colp = &(*colp)->next;
        };

        if( *colp == NULL )
        {
            INT     name_len        = 0;

            col_no++;
            *colp = mem_arena_calloc(p_cnt->arena, sizeof(**colp), 1, __FILE__, __LINE__ );
            name_len = strlen(p_field);
            (*colp)->name = mem_arena_calloc(p_cnt->arena, name_len+1, 1, __FILE__, __LINE__ );
            assert( (*colp)->name != NULL );
            memcpy((*colp)->name, p_field, name_len+1);
            (*colp)->col  = col_no;
        };
        col = *colp;
        assert( col != NULL );
        assert( col_no >= 0 );

        max = p_cnt->count_cell;
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
                break;
            case 1:
                max = pos;
                break;
            }
        }
        pos = min;

        if( cmp == 0 )
        {
            cell = acell[pos];
        }
        else
        {
            INT         n = 0;

            cell = mem_arena_calloc( p_cnt->arena, sizeof(*cell), 1, __FILE__, __LINE__ );
            cell->row   = p_row;
            cell->col   = col_no;
            n = p_cnt->count_cell - pos;
            p_cnt->count_cell++;
            if( p_cnt->count_cell >= p_cnt->max_cell )
            {
                CNT_CELL  * nacell = NULL;
                INT         nmax    = p_cnt->max_cell * 2;

                nacell = mem_arena_calloc( p_cnt->arena, sizeof(cell), nmax, __FILE__, __LINE__ );
                assert( nacell != NULL );
                memmove( nacell, acell, sizeof(cell) * p_cnt->max_cell );
                acell = nacell;
                p_cnt->cell = acell;
                p_cnt->max_cell = nmax;
            }
            if( n > 0 )
            {
                memmove( acell + pos + 1, 
                            acell + pos, 
                            n * sizeof(cell) );
            }
            acell[pos] = cell;
        }
        cell->flg = p_flg;
        if( p_size < 0 )
            cell->size = strlen(p_val);
        else
            cell->size = p_size;
        if( cell->size > 0 )
        {
            cell->val = mem_arena_calloc( p_cnt->arena, cell->size + 1, 1, __FILE__, __LINE__ );
            memcpy(cell->val, p_val, cell->size);
            ((char*)cell->val)[cell->size] = 0;
        }
        else
            cell->val = p_val;

        if( col->width < cell->size )
            col->width = cell->size;

        assert( *colp != NULL && (*colp)->name != NULL );

        cnt_check( p_cnt );
    }

    /* return */
    return status;
}

