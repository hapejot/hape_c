#include "platform.h"
#include "io_int.h"

/*---------------------------------------------------------------------------
 * NAME
 *      io_attr_get - reads attributes of a file structure.
 *
 * SYNOPSIS
 */
INT
io_attr_get
(
    IN      IO_FILE     p_f, 
    IN      char*       p_an, 
    OUT     char*       p_av, 
    IN      INT         p_avsize
)
/*
 * DESCRIPTION
 * 
 * PARAMETERS
 *      p_f - The file object to read the attribute from.
 *      p_an - the name of attribute to read from. The detailed list
 *          of available names must be defined by the specific file
 *          implementation. Therefore it a necessary knowledge which
 *          concrete file implementation is used.
*       p_av - pointer to the value. Values are allways returned as
*           zero terminated strings.
*       p_avsize - denotes the amount of space available for storing
*           the value. The caller has to reserve space for storing the
*           value in.
 * 
 * RETURN VALUES
 *      return code and value, copied to the prepared storage area.
 *
 * ERRORS
 *      [RC_OK]         Successful completion.
 *
 *---------------------------------------------------------------------------*/
{
    INT status = RC_OK;
    /* end of local var. */

    /* parameter check */
    if( p_f->t->attr_get == NULL )
        status = RC_ERROR;
    assert( p_f->t->attr_get != NULL );
    
    /* environment check */

    /* processing. */
    if ( status == RC_OK )
    {
        status = (p_f->t->attr_get)( p_f, p_an, p_av, p_avsize );
    }

    /* return */
    return status;
}

