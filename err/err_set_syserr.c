#include <hape/platform.h>
#include <hape/err.h>
#include <errno.h>
#include <string.h>

/*---------------------------------------------------------------------------
 * NAME
 *      err_set_syserr - 
 *
 * SYNOPSIS
 */
INT
err_set_syserr
(
    char * p_szCall,
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
        g_err.nSysErr = errno;
        strcpy( g_err.szFile, p_szFile );
        g_err.nLine = p_nLine;
        strcpy( g_err.szSysComp, "unix" );
        strcpy( g_err.szSysCall, p_szCall );
        strcpy( g_err.szSysMsg, strerror(g_err.nSysErr));

        rc = RC_OK;
    }

    /* return */
    return rc;
}

