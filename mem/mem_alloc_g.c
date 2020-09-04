#include "platform.h"
#include "mem_int.h"
#include <malloc.h>
#include <assert.h>

/*---------------------------------------------------------------------------
 * NAME
 *      mem_alloc_g - allocates memory with a given size.
 *
 *      in addition to the memory there will be guards written to memory,
 *      so that upon release one can check if any parts of the allocated memory
 *      has been overwritten.
 *
 *      Also there is no need to free each individual memory allocation, instead
 *      one can just release the group. 
 *      therefore a group pointer is passed to every allocation call.
 *
 * SYNOPSIS
 */
void*
mem_alloc_g
(
    INT p_size,
    void **p_group,
    char * p_source,
    int p_line
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
    void* result = NULL;
    INT status = RC_OK;
    void ** ptr = NULL;
    /* end of local var. */

    /* parameter check */
    
    /* environment check */
    if( g_mem.log_flag && (g_mem.log == NULL) )
        g_mem.log = fopen("mem.log", "w");

    /* processing. */
    if ( status == RC_OK )
    {
        INT         n       = p_size + 10*sizeof(void*);

        ptr = malloc(n);
        if( g_mem.log_flag )
        {
            fprintf( g_mem.log, "A %4ld %p %7ld %s[%d]\n", g_mem.cnt++, (void*)ptr, n, p_source, p_line );
            fflush( g_mem.log );
        }
        assert(ptr != NULL);
        memset(ptr, 0, n);
        result = ptr + 6;
        ptr[1] = (void*)p_size;
        ptr[2] = (void*)0x11111111;
        ptr[3] = (void*)0xaaaaaaaa;
        ptr[4] = (void*)0xbbbbbbbb;
        ptr[5] = (void*)0xcccccccc;
        ptr[0] = *p_group;
        *p_group = ptr;
        ptr = (void**)((char*)result+p_size);
        ptr[0] = (void*)0x11111111;
        ptr[1] = (void*)0x22222222;
        ptr[2] = (void*)0x33333333;
        ptr[3] = (void*)0x44444444;
    }

    /* return */
    return result;
}

