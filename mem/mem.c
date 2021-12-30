////////// GENERAL INCLUDES ////////////////////////////////////////////////////
#include <assert.h>
#include <ctype.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
////////// PROJECT INCLUDES ////////////////////////////////////////////////////
#include <hape/platform.h>
#include <hape/err.h>
#include <hape/mem.h>
#include "mem_int.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


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
            fprintf( g_mem.log, "A %4d %p %7d %s[%d]\n", g_mem.cnt++, (void*)ptr, n, p_source, p_line );
            fflush( g_mem.log );
        }
        assert(ptr != NULL);
        memset(ptr, 0, n);
        result = ptr + 6;
        ptr[1] = (void*)((long)p_size);
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


/*---------------------------------------------------------------------------
 * NAME
 *      mem_arena_alloc - 
 *
 * SYNOPSIS
 */
void *mem_arena_alloc
        ( IN MEM_ARENA arena, IN INT nbytes, IN char *file, IN INT line )
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
    void *result = 0;
    INT status = RC_OK;
/* end of local var. */
    ( void )file;
    (void)line;
/* parameter check */
    assert( arena );
    assert( nbytes > 0 );

/* environment check */

/* processing. */
    if( status == RC_OK ) {
        INT alignment = sizeof( union align );

        nbytes = ( ( nbytes + alignment - 1 ) / alignment ) * alignment;
        if( nbytes > ( arena->limit - arena->avail ) ) {
            MEM_ARENA ptr = NULL;
            char *limit = NULL;
            INT m = 0;

            m = sizeof( union header ) + nbytes + 10 * 1024;
            ptr = malloc( m );
            assert( ptr != NULL );
            limit = ( char * )ptr + m;
            *ptr = *arena;
            arena->avail = ( char * )( ( union header * )ptr + 1 );
            arena->limit = limit;
            arena->prev = ptr;
        }
        result = arena->avail;
        arena->avail += nbytes;

        if( status == RC_OK )
            g_mem.alloc_cnt += nbytes;
    }

/* return */
    return result;
}

/*---------------------------------------------------------------------------
 * NAME
 *      mem_arena_calloc - 
 *
 * SYNOPSIS
 */
void *
mem_arena_calloc
(
    IN      MEM_ARENA       arena,
    IN      INT             count,
    IN      INT             nbytes,
    IN      char          * file,
    IN      INT             line
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
    void      * result = NULL;
    INT         status = RC_OK;
    /* end of local var. */

    /* parameter check */
    assert(count > 0);
    
    /* environment check */

    /* processing. */
    if ( status == RC_OK )
    {
	    result = mem_arena_alloc(arena, count * nbytes, file, line);
        memset( result, '\0', count * nbytes);
    }

    /* return */
    return result;
}


/*---------------------------------------------------------------------------
 * NAME
 *      mem_arena_dispose - 
 *
 * SYNOPSIS
 */
INT
mem_arena_dispose
(
    INOUT   MEM_ARENA     * ap
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
	assert(ap && *ap);
    
    /* environment check */

    /* processing. */
    if ( status == RC_OK )
    {
	    mem_arena_free(*ap);
        free(*ap);
        *ap = NULL;

        status = RC_OK;
    }

    /* return */
    return status;
}


/*---------------------------------------------------------------------------
 * NAME
 *      mem_arena_free - 
 *
 * SYNOPSIS
 */
INT
mem_arena_free
(
    IN      MEM_ARENA       arena
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
        assert(arena);
        while (arena->prev) 
        {
            struct MEM_ARENA tmp = *arena->prev;
            free(arena->prev);
            *arena = tmp;
        }
        assert(arena->limit == NULL);
        assert(arena->avail == NULL);
    }

    /* return */
    return status;
}


/*---------------------------------------------------------------------------
 * NAME
 *      mem_arena_new - 
 *
 * SYNOPSIS
 */
MEM_ARENA
mem_arena_new
(
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
    MEM_ARENA result = 0;
    INT status = RC_OK;
    /* end of local var. */

    /* parameter check */
    
    /* environment check */

    /* processing. */
    if ( status == RC_OK )
    {
        result = malloc(sizeof (*result));

        assert( result != NULL );

        result->prev  = NULL;
        result->limit = NULL;
        result->avail = NULL;
    }

    /* return */
    return result;
}


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
        long     size;
        while(*p_group != NULL)
        {
            ptr = p_group[0];
            free_ptr = ptr;
            size = (long)ptr[1];
            if( g_mem.log_flag )
            {
                fprintf( g_mem.log, "F %4d %p %ld\n", g_mem.cnt++, (void*)ptr, size );
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



struct MEM_GLOBALS      g_mem;


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

