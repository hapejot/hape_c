#include "err_int.h"
#include <string.h>

/*---------------------------------------------------------------------------
 * NAME
 *      err_set_err - 
 *
 * SYNOPSIS
 */
INT
err_set_err
(
    INT    p_errno,
    char * p_szMsg,
    char * p_szComp,
    char * p_szFile,
    INT p_nLine
)
/*
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
        g_err.nSysErr = p_errno;
        strcpy( g_err.szFile, p_szFile );
        g_err.nLine = p_nLine;
        strcpy( g_err.szSysComp, p_szComp );
        strcpy( g_err.szSysMsg, p_szMsg);
        g_err.szSysCall[0] = ENDSTR;

        rc = RC_OK;
    }

    /* return */
    return rc;
}

