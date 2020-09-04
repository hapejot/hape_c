#include "platform.h"
#include "cnt_int.h"

/*---------------------------------------------------------------------------
 * NAME
 *      cnt_split - 
 *
 * SYNOPSIS
 */
INT cnt_split
        ( IN char *p_data,
          IN UINT p_dlen,
          IN char p_sep, IN char *p_elempattern, INOUT CNT p_cnt )
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
    if( status == RC_OK ) {
        char buf[1000] = "";
        char elem_name[80] = "";
        INT k = 0;
        UINT i = 0;
        INT argc = 0;

        for( i = 0; i < p_dlen; i++ ) {
            if( p_data[i] == p_sep ) {
                sprintf( elem_name, p_elempattern, argc );
                cnt_set_val( p_cnt, elem_name, 0, -1, buf );
                k = 0;
                argc++;
            }
            else {
                assert( i < sizeof( buf ) );
                buf[k++] = p_data[i];
            }
            buf[k] = 0;
        }
        if( buf[0] ) {
            sprintf( elem_name, p_elempattern, argc );
            cnt_set_val( p_cnt, elem_name, 0, -1, buf );
            argc++;
        }

    }

/* return */
    return status;
}
