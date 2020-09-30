#include "platform.h"
#include "cnt_int.h"

/*---------------------------------------------------------------------------
 * NAME
 *      cnt_set_val - 
 *
 * SYNOPSIS
 */
INT
cnt_set_val
(
    CNT         p_cnt,
    const char      * p_field,
    INT         p_flg,
    INT         p_size,
    const void      * p_val
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
        char    name[1024];
        char    rest[1024];
        INT     k           = 0;

        assert( strlen(p_field) < sizeof(name) );

        cnt_split_name(p_field, name, &k, rest );

        if( rest[0] == 0 )
        {
            status = cnt_set_idx_val(p_cnt, k, name, p_flg, p_size, p_val);
        }
        else
        {
            INT     flg     = 0;
            CNT     cnt     = NULL;

            status = cnt_get_idx_val(p_cnt, k, name, &flg , NULL, (void**)&cnt);
            if( status != RC_OK )
            {
                status = cnt_create(p_cnt->arena, &cnt);
                assert( status == RC_OK );
                status = cnt_set_idx_val( p_cnt, 
                            k, name, CNT_SUBCONT, 0, cnt );
                assert( status == RC_OK );
            }
            else
                assert( CNT_ISCONT(flg) );
            status = cnt_set_val( cnt, rest, p_flg, p_size, p_val );
        }
            
    }

    /* return */
    return status;
}

