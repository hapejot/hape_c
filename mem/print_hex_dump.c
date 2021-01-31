#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <hape/platform.h>
#include <hape/mem.h>

/*---------------------------------------------------------------------------
 * NAME
 *      print_hex_dump - 
 *
 * SYNOPSIS
 */
INT
print_hex_dump
(
    INT     p_len,
    CHAR   *p_data
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
        INT     i;
        INT     cnt             = 16;
        INT     j;
        CHAR    digits[]        ="0123456789abcdef";
        char    line_out[100];
        INT     idx_line;

        for(j = 0; j < p_len; j += cnt)
        {
            cnt = ((p_len - j) > 16) ? 16:(p_len-j);

            sprintf(line_out, "%p  ",(void*)p_data);
            idx_line = strlen(line_out);

            for(i=0;i<16;i++) 
            {
                if( i < cnt )
                {
                    line_out[idx_line++] = digits[p_data[i] >> 4 & 0x0f];
                    line_out[idx_line++] = digits[p_data[i]      & 0x0f];
                    if(i == 7) 
                    {
                        line_out[idx_line++] = ' ';
                    } 
                    line_out[idx_line++] = ' ';
                }
                else
                {
                    if(i == 7) 
                    {
                        line_out[idx_line++] = ' ';
                    } 
                    line_out[idx_line++] = ' ';
                    line_out[idx_line++] = ' ';
                    line_out[idx_line++] = ' ';
                }
            }
            line_out[idx_line++] = ' ';

            for(i = 0; i < cnt; i++) 
            {
                if(!isprint(p_data[i])) 
                {
                    line_out[idx_line++] = '.';
                } 
                else 
                {
                    line_out[idx_line++] = p_data[i];
                }
            }

            line_out[idx_line] = 0;
            puts(line_out);
            p_data += cnt;
        }
        status = RC_OK;
    }

    /* return */
    return status;
}

