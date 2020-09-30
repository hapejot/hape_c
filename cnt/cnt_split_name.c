#include "platform.h"
#include "cnt_int.h"
#include <ctype.h>

/*---------------------------------------------------------------------------
 * NAME
 *      cnt_split_name - 
 *
 * SYNOPSIS
 */
INT
cnt_split_name
(
    const char      * p_str,
    char      * p_name,
    INT       * p_idx,
    char      * p_rest
)
/*
 * PARAMETERS
 *
 * DESCRIPTION
 * 
 * RETURN VALUES
 *
 * ERRORS
 *      [RC_OK]         Successful completion.
 *
 *---------------------------------------------------------------------------*/
{
    INT rc = RC_OK;
    /* end of local var. */

    /* parameter check */
    
    /* environment check */

    /* processing. */
    if ( rc == RC_OK )
    {
        INT     k           = 0;
        const char  * s           = NULL;     /* source char pointer into p_str */
        char  * t           = NULL;     /* target char ptr into p_name */

        s = p_str;
        /* 
         * read optional '[' <digit>+ ']' '.'
         */
        if( *s == '[' )
        {
            s++; 
            while(isdigit(*s))
            {
                k *= 10;
                k += *s - '0';
                s++;
            }
            assert( *s == ']' );
            s++;
            if( *s == '.' )
                s++;
            else
                assert( *s == 0 );
        }
        /*
         * read name
         */
        t = p_name;
        while( *s && *s != '.' && *s != '[' )
        {
            *t++ = *s++;
        }
        *t++ = 0;
        if( *s == '.' )
            s++;
        strcpy( p_rest, s );
        *p_idx = k;

        rc = RC_OK;
    }
    /* return */
    return rc;
}

