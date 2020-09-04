#include "platform.h"
#include "mem_int.h"

/*---------------------------------------------------------------------------
 * NAME
 *      mem_free_g - 
 *
 * SYNOPSIS
 */
INT
mem_free_g
(
    void ** p_group
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
    assert( !g_mem.log_flag || g_mem.log != NULL );

    /* processing. */
    if ( status == RC_OK )
    {
        void** ptr;
        void * free_ptr;
        INT     size;
        while(*p_group != NULL)
        {
            ptr = p_group[0];
            free_ptr = ptr;
            size = (INT)ptr[1];
            if( g_mem.log_flag )
            {
                fprintf( g_mem.log, "F %4ld %p %ld\n", g_mem.cnt++, (void*)ptr, size );
                fflush(g_mem.log);
            }
            assert( ptr[2] == (void*)0x11111111 );
            assert( ptr[3] == (void*)0xaaaaaaaa );
            assert( ptr[4] == (void*)0xbbbbbbbb );
            assert( ptr[5] == (void*)0xcccccccc ) ;
            p_group[0] = ptr[0];
            ptr = (void**)(((char*)(ptr+6)) + size);
            if( ptr[0] != (void*)0x11111111 )
            {
                print_hex_dump(size + 10 * sizeof(void*), free_ptr);
            }
            assert( ptr[1] == (void*)0x22222222 );
            assert( ptr[2] == (void*)0x33333333 );
            assert( ptr[3] == (void*)0x44444444 );

            free(free_ptr);
        }

        status = RC_OK;
    }

    /* return */
    return status;
}

