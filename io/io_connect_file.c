#include "platform.h"
#include "err.h"
#include "io_int.h"
// #include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

/*---------------------------------------------------------------------------
 * NAME
 *      io_connect_file - Wraps the fopen function of stdio.
 *
 * SYNOPSIS
 */
INT
io_connect_file
(
    IN  IO_FILE       * p_file,
    IN  char          * p_filename,
    IN  char          * p_mode
)
/*
 * DESCRIPTION
 * 
 * PARAMETERS
 *      p_file - File Handle that will be generated.
 *      p_filename - Name of the file to be opened. If p_filename is NULL than
 *                  either stdin or stdout are used as file. Which one depends
 *                  on the setting of p_mode. 
 *                  stdout, if it starts with 'w'
 *                  stdint, if it starts with 'r'
 *      p_mode - Defines the opening mode.
 *                  r: read only
 *                  rb: read only, binary
 * 
 * RETURN VALUES
 *      Error Status
 *
 * ERRORS
 *      [RC_OK]             Successful completion.
 *      RC_IO_NOSUCHFILE    The file doesn't exist.
 *      RC_IO_NOPERM        The process doesn't have the permissions to open the
 *                          file with the given mode. It might be working in
 *                          another mode though.
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
        INT         io_mode     = 0;
        INT         fd = -1;

        assert( strlen(p_mode) > 0 );
        if( strcmp(p_mode, "r") == 0 )
            io_mode = O_RDONLY ;
        else if( strcmp(p_mode, "r+") == 0 )
            io_mode = O_RDWR ;
        else if( strcmp(p_mode, "w") == 0 )
            io_mode = O_WRONLY | O_TRUNC ;
        else if( strcmp(p_mode, "w+") == 0 )
            io_mode = O_RDWR | O_TRUNC | O_CREAT ;
        else if( strcmp(p_mode, "a") == 0 )
            io_mode = O_APPEND | O_CREAT ;
        else if( strcmp(p_mode, "a+") == 0 )
            io_mode = O_RDWR | O_CREAT ;
        

        if( p_filename == NULL || strcmp(p_filename, "-") == 0 )
        {
            switch( io_mode & 0x03 )
            {
                case O_RDONLY:
                    fd = 0;
                    break;
                case O_WRONLY:
                    fd = 1;
                    break;
                default:
                    status = RC_ERROR;
                    break;
            }
        }
        else
        {
            fd = open( p_filename, io_mode|O_BINARY, S_IRWXU | S_IRWXG | S_IRWXO );
        }

        if( fd == -1 )
        {
            char    buf[80];

            switch( errno )
            {
                case ENOENT:
                    status = RC_IO_NOSUCHFILE;
                    break;
                case EACCES:
                    status = RC_IO_NOPERM;
                    break;
                default:
                    perror( "open" );
                    status = RC_ERROR;
                    break;
            }
            sprintf( buf, "open %ld", io_mode );
            err_set_syserr( buf, __FILE__, __LINE__ );
            *p_file = NULL;
        }
        else
            status = io_connect_fd( p_file, fd );
    }

    /* return */
    return status;
}

