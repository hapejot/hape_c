#include "platform.h"
#include "err.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/
INT
err_print (
    )
/*---------------------------------------------------------------------------*/
{
    INT rc = RC_OK;

    /* end of local var. */

    /* parameter check */

    /* environment check */

    /* processing. */
    if (rc == RC_OK)
    {
        printf ("*************************************************\n");
        printf ("System Error:      %ld\n", g_err.nSysErr);
        printf ("System Component:  %s\n", g_err.szSysComp);
        printf ("System Call:       %s\n", g_err.szSysCall);
        printf ("Line:              %ld\n", g_err.nLine);
        printf ("File:              %s\n", g_err.szFile);
        printf ("System Message:    %s\n", g_err.szSysMsg);
        printf ("*************************************************\n");
    }

    /* return */
    return rc;
}
