#include "platform.h"
#include "cnt_int.h"

/*---------------------------------------------------------------------------
 * NAME
 *      cnt_dump - 
 *
 * SYNOPSIS
 */
INT
cnt_dump
(
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
    
    /* environment check */

    /* processing. */
    if ( status == RC_OK )
    {
        INT         rowcnt     = 0;
        INT         colcnt     = 0;
        INT         cellcnt    = 0;
        INT         i           = 0;
        CNT_HEAD    col         = NULL;
        char      * val         = NULL;
        INT         size        = 0;
        INT         flg         = 0;
        void      * crs         = NULL;

        /* 
         * calculate maximum row number 
         */
        cnt_get_dim( p_cnt, &rowcnt, &colcnt, &cellcnt );
        /*
         *
         */
//        printf("%d rows, %d columns, %d cells\n", 
//                    rowcnt, colcnt, cellcnt);
        cnt_first_name(p_cnt, &crs, &col);
        while( col )
        {
            printf("%-*.*s ", (int)col->width, (int)col->width, col->name);
            cnt_next_name(&crs, &col);
        }
        puts("");

        cnt_first_name(p_cnt, &crs, &col);
        while( col )
        {
            INT i;
            for( i = col->width; i > 0; i-- )  putchar('=');
            putchar(' ');
            cnt_next_name(&crs, &col);
        }
        puts("");
        for( i=0; i < rowcnt; i++ )
        {
            cnt_first_name(p_cnt, &crs, &col);
            while( col )
            {
                status = cnt_get_idx_val(p_cnt, i, col->name, &flg, &size, (void**)&val);
                if( status == RC_OK )
                    printf("%-*.*s ", (int)col->width, (int)col->width, val);
                else
                    printf("%-*.*s ", (int)col->width, (int)col->width, "-");
                cnt_next_name(&crs, &col);
            }
            puts("");
        }
        status = RC_OK;
    }

    /* return */
    return status;
}

