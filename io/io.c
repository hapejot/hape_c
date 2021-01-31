////////////////////////////////////////////////////////////////////////////////
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
////////////////////////////////////////////////////////////////////////////////
#include <hape/platform.h>
#include <hape/err.h>
#include <hape/io.h>
#include "io_int.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define GLOBALS
#include "io_int.h"


////////////////////////////////////////////////////////////////////////////////



/*---------------------------------------------------------------------------
 * NAME
 *      io_attr_get - reads attributes of a file structure.
 *
 * SYNOPSIS
 */
INT io_attr_get
        ( IN IO_FILE p_f, IN char *p_an, OUT char *p_av, IN INT p_avsize )
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
    if( status == RC_OK )
    {
        status = ( p_f->t->attr_get ) ( p_f, p_an, p_av, p_avsize );
    }

/* return */
    return status;
}


/*---------------------------------------------------------------------------
 * NAME
 *      io_buf_expand - 
 *
 * SYNOPSIS
 */
INT io_buf_expand( INOUT IO_BUFFER p_buf, IN INT p_size )
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
    assert( p_buf != NULL );

/* environment check */

/* processing. */
    if( status == RC_OK )
    {
        p_buf->pch = realloc( p_buf->pch, p_size );
        assert( p_buf->pch != NULL );
    }

/* return */
    return status;
}


/*---------------------------------------------------------------------------
 * NAME
 *      io_buf_new - 
 *
 * SYNOPSIS
 */
INT io_buf_new( OUT IO_BUFFER * p_buf )
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
    if( status == RC_OK )
    {
        IO_BUFFER iobuf;
        iobuf = malloc( sizeof( *iobuf ) );
        assert( iobuf != NULL );
        iobuf->len = 0;
        iobuf->size = 20000;
        iobuf->pch = malloc( iobuf->size );
        assert( iobuf->pch != NULL );
        iobuf->pch[iobuf->len] = 0;
        iobuf->next = NULL;
        iobuf->prev = NULL;

        *p_buf = iobuf;
    }

/* return */
    return status;
}


/*---------------------------------------------------------------------------
 * NAME
 *      io_buf_read - 
 *
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
INT io_buf_read( IO_FILE s, void *d, INT l, INT * len )
{
    (void) s;
    (void) d;
    (void) l;
    (void) len;
    INT status = RC_OK;
/* end of local var. */

/* parameter check */

/* environment check */

/* processing. */
    if( status == RC_OK )
    {

    }

/* return */
    return status;
}


/*---------------------------------------------------------------------------
 * NAME
 *      io_buf_write - 
 *
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
INT io_buf_write
        ( IN IO_FILE p_file,
          IN void *p_data, IN INT p_size, IN INT * p_count )
{
    INT status = RC_OK;
/* end of local var. */

/* parameter check */
    assert( p_file != NULL );
    assert( p_file->p != NULL );

/* environment check */

/* processing. */
    if( status == RC_OK )
    {
        IO_BUFFER buf = p_file->p;
        INT free_space = 0;
        INT len = 0;

        if( p_size == -1 )
            len = strlen( p_data ) + 1;
        else
            len = p_size;

        free_space = buf->size - buf->len;
        while( free_space < len )
        {
            io_buf_expand( buf, buf->size * 2 );
            free_space = buf->size - buf->len;
        }
        memcpy( buf->pch + buf->len, p_data, len );
        buf->len += len;

        *p_count = len;
    }

/* return */
    return status;
}


/*---------------------------------------------------------------------------
 * NAME
 *      io_close - 
 *
 * SYNOPSIS
 */
INT io_close( INOUT IO_FILE * p_io )
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
    if( status == RC_OK )
    {
        assert( ( *p_io )->t->close != NULL );
        status = ( ( *p_io )->t->close ) ( *p_io );
        free( *p_io );
        *p_io = NULL;

        status = RC_OK;
    }

/* return */
    return status;
}


static IO_CLASS g_io_buffer = {
    io_buf_read,                                 /* read     */
    io_buf_write,                                /* write    */
    NULL,                                        /* seek     */
    NULL,                                        /* tell     */
    NULL,                                        /* close    */
    NULL,                                        /* get_attr */
}

;


/*---------------------------------------------------------------------------
 * NAME
 *      io_connect_buf - create a file based on an IO_BUFFER.
 *
 * SYNOPSIS
 */
INT io_connect_buf( INOUT IO_FILE * p_file, IN IO_BUFFER p_buf )
/*
 * DESCRIPTION
 *      The IO_FILE that is created will wrap a IO_BUFFER structure
 *      as the file implementation. Every read or write operation will go
 *      to the buffer. And so all operations go into memory only.
 *      This is an easy method create a string by using io procedures
 *      only. The fmt module relies on this.
 * 
 * PARAMETERS
 *      p_file - file structure to be initialized. The structure will
 *              be allocated by this function. It will be released 
 *              upon an io_close call.
 *
 *      p_buf - buffer to wrap with a file structure. The buffer must
 *              be initialized and fully functional.
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
    if( status == RC_OK )
    {
// pint - wird als Bufferpointer zweckentfremdet.
// p - wird für den IO_BUFFER verwendet.
        IO_FILE io;

        io = malloc( sizeof( *io ) );
        assert( io != NULL );

        io->t = &g_io_buffer;
        io->pint = 0;
        io->p = p_buf;
        io->unget_count = 0;

        *p_file = io;
    }

/* return */
    return status;
}


static IO_CLASS g_io_file = {
    io_std_read,                                 /* read     */
    io_std_write,                                /* write    */
    io_std_seek,                                 /* seek     */
    NULL,                                        /* tell     */
    io_std_close,                                /* close    */
    NULL
}

;
/*---------------------------------------------------------------------------
 * NAME
 *      io_connect_fd - Wraps the fopen function of stdio.
 *
 * SYNOPSIS
 */
INT io_connect_fd( IN IO_FILE * p_file, IN INT p_fd )
/*
 * DESCRIPTION
 * 
 * PARAMETERS
 *      p_file - File Handle that will be generated.
 *      p_fd - abstract file descriptor.
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
    if( status == RC_OK )
    {
        IO_FILE io;

        io = malloc( sizeof( *io ) );
        assert( io != NULL );

        io->t = &g_io_file;
        io->pint = p_fd;
        io->unget_count = 0;

        *p_file = io;
    }

/* return */
    return status;
}


/*---------------------------------------------------------------------------
 * NAME
 *      io_connect_file - Wraps the fopen function of stdio.
 *
 * SYNOPSIS
 */
INT io_connect_file
        ( IN IO_FILE * p_file, IN char *p_filename, IN char *p_mode )
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
    if( status == RC_OK )
    {
        INT io_mode = 0;
        INT fd = -1;

        assert( strlen( p_mode ) > 0 );
        if( strcmp( p_mode, "r" ) == 0 )
            io_mode = O_RDONLY;
        else if( strcmp( p_mode, "r+" ) == 0 )
            io_mode = O_RDWR;
        else if( strcmp( p_mode, "w" ) == 0 )
            io_mode = O_WRONLY | O_TRUNC;
        else if( strcmp( p_mode, "w+" ) == 0 )
            io_mode = O_RDWR | O_TRUNC | O_CREAT;
        else if( strcmp( p_mode, "a" ) == 0 )
            io_mode = O_APPEND | O_CREAT;
        else if( strcmp( p_mode, "a+" ) == 0 )
            io_mode = O_RDWR | O_CREAT;


        if( p_filename == NULL || strcmp( p_filename, "-" ) == 0 )
        {
            switch ( io_mode & 0x03 )
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
            fd = open( p_filename, io_mode | O_BINARY,
                       S_IRWXU | S_IRWXG | S_IRWXO );
        }

        if( fd == -1 )
        {
            char buf[80];

            switch ( errno )
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


/*---------------------------------------------------------------------------
 * NAME
 *      io_getc - get one character from file.
 *
 * SYNOPSIS
 */
INT io_getc( IN IO_FILE p_io )
/*
 * DESCRIPTION
 *      uses the read procedure to get data from the file stream.
 * 
 * PARAMETERS
 *      p_io - file to read from.
 *
 * RETURN VALUES
 *      Character read or
 *      -1, when end of file reached.
 *      -2, when temporarily no char could be read.
 *
 * ERRORS
 *      [RC_OK]         Successful completion.
 *
 *---------------------------------------------------------------------------*/
{
    INT result = 0;
    INT status = RC_OK;
/* end of local var. */

/* parameter check */
    assert( p_io != NULL );

/* environment check */

/* processing. */
    if( status == RC_OK )
    {
        assert( p_io->t != NULL );
        assert( p_io->t->read != NULL );
        if( p_io->unget_count > 0 )
        {
            result = p_io->unget_buffer[IO_UNGET_BUFSIZE - p_io->unget_count];
            p_io->unget_count--;
        }
        else
        {
            unsigned char c;
            INT n;

            status = ( p_io->t->read ) ( p_io, &c, sizeof( c ), &n );
            if( status == RC_OK )
            {
                assert( n == sizeof( c ) );
                result = c;
            }
            else
                result = -1;
        }
    }

/* return */
    return result;
}


/*---------------------------------------------------------------------------
 * NAME
 *      io_gets - Read one string from input stream.
 *
 * SYNOPSIS
 */
INT io_gets
        ( IN IO_FILE p_file,
          INOUT INT8 * p_buf, IN INT p_bufsize, OUT INT * p_bytes )
/*
 * DESCRIPTION
 * 
 * PARAMETERS
 *      p_file - input stream.
 *      p_buf - memory to store string.
 *      p_bufsize - amount of available space for storing the string.
 *      p_bytes - returns the number of bytes used for the string.
 * 
 * RETURN VALUES
 *      error code and the number of bytes copied.
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
    if( status == RC_OK )
    {
        INT i = 0;
        INT c;

        while( i < ( p_bufsize - 1 ) )
        {
            c = io_getc( p_file );
            if( c == -1 )
                break;
            p_buf[i++] = ( INT8 ) c;
            p_buf[i] = 0;
            if( c == '\n' )
                break;
        }
        *p_bytes = i;
        if( i == 0 && c == -1 )
            status = RC_IO_EOF;
    }

/* return */
    return status;
}


/*---------------------------------------------------------------------------
 * NAME
 *      io_putc - write a character to a stream.
 *
 * SYNOPSIS
 */
INT io_putc( IN IO_FILE p_io, IN INT p_char )
/*
 * DESCRIPTION
 * 
 * PARAMETERS
 *      p_io - sream to write to.
 *      p_char - character to write.
 * 
 * RETURN VALUES
 *      Character read or
 *      -1, when end of file reached.
 *      -2, when temporarily no char could be read.
 *
 * ERRORS
 *      [RC_OK]         Successful completion.
 *
 *---------------------------------------------------------------------------*/
{
    INT status = RC_OK;
/* end of local var. */

/* parameter check */
    assert( p_io != NULL );

/* environment check */

/* processing. */
    if( status == RC_OK )
    {
        unsigned char c;
        INT n;

        c = p_char;

        assert( p_io->t != NULL );
        assert( p_io->t->write != NULL );
        status = ( p_io->t->write ) ( p_io, &c, sizeof( c ), &n );
    }

/* return */
    return status;
}


/*---------------------------------------------------------------------------
 * NAME
 *      io_puts - write a 0-terminated string to the output stream
 *
 * SYNOPSIS
 */
INT io_puts( IN IO_FILE p_io, IN char *p_chars )
/*
 * DESCRIPTION
 * 
 * PARAMETERS
 *      p_io - sream to write to.
 *      p_chars - string
 * 
 * RETURN VALUES
 *
 * ERRORS
 *      [RC_OK]         Successful completion.
 *
 *---------------------------------------------------------------------------*/
{
    INT status = RC_OK;

    assert( p_chars );
    assert( p_io != NULL );


    if( status == RC_OK )
    {
        INT n;
        n = strlen( p_chars );

        assert( p_io->t != NULL );
        assert( p_io->t->write != NULL );
        status = ( p_io->t->write ) ( p_io, p_chars, n, &n );
    }

/* return */
    return status;
}


/*---------------------------------------------------------------------------
 * NAME
 *      io_read - read a block of characters from a stream.
 *
 * SYNOPSIS
 */
INT io_read( IN IO_FILE p_io, IN void *p_data, IN INT p_len, OUT INT * p_n )
/*
 * DESCRIPTION
 * 
 * PARAMETERS
 *      p_io - input stream.
 *      p_data - data to store the data.
 *      p_len - amount of space available for storing the data.
 *      p_n - number of bytes acually stored.
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
    assert( p_io != NULL );
    assert( p_io->t != NULL );
    assert( p_io->t->read != NULL );
    assert( p_data != NULL );
    assert( p_len >= 0 );

/* processing. */
    if( status == RC_OK )
    {
        INT n = 0;
        INT read_count;

        if( p_io->unget_count > 0 )
        {
            if( p_io->unget_count >= p_len )
            {
                memcpy( p_data,
                        p_io->unget_buffer + IO_UNGET_BUFSIZE -
                        p_io->unget_count, p_len );
                p_io->unget_count -= p_len;
                n = p_len;
            }
            else
            {
                memcpy( p_data,
                        p_io->unget_buffer + IO_UNGET_BUFSIZE -
                        p_io->unget_count, p_io->unget_count );
                n = p_io->unget_count;
                p_io->unget_count = 0;
            }
        }
        if( n < p_len )
        {
            status = ( p_io->t->read ) ( p_io, ( ( CHAR * ) p_data ) + n,
                                         p_len - n, &read_count );
            n += read_count;
        }
        assert( p_len >= n );
        *p_n = n;
    }

/* return */
    return status;
}


/*---------------------------------------------------------------------------
 * NAME
 *      io_seek -  seek to a specific file position.
 *
 * SYNOPSIS
 */
INT io_seek( INOUT IO_FILE p_io, IN UINT64 p_pos )
/*
 * DESCRIPTION
 * 
 * PARAMETERS
 *      p_io - file stream.
 *      p_pos - byte position form the beginning of the stream.
 * 
 * RETURN VALUES
 *      error code.
 *
 * ERRORS
 *      [RC_OK]         Successful completion.
 *
 *---------------------------------------------------------------------------*/
{
    INT status = RC_OK;
/* end of local var. */

/* parameter check */
    assert( p_io != NULL );
    assert( p_io->t != NULL );
    assert( p_io->t->seek != NULL );

/* environment check */

/* processing. */
    if( status == RC_OK )
    {
        status = ( p_io->t->seek ) ( p_io, p_pos );
        p_io->unget_count = 0;
    }

/* return */
    return status;
}


/*---------------------------------------------------------------------------
 * NAME
 *      io_std_close - 
 *
 * SYNOPSIS
 */
INT io_std_close( IN IO_FILE p_io )
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
    if( status == RC_OK )
    {
        close( p_io->pint );
    }

/* return */
    return status;
}


/*---------------------------------------------------------------------------
 * NAME
 *      io_std_read - 
 *
 * SYNOPSIS
 */
INT io_std_read
        ( IN IO_FILE p_io, IN void *p_data, IN INT p_size, OUT INT * p_len )
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
    if( status == RC_OK )
    {
        INT n;

        n = read( p_io->pint, p_data, p_size );
        if( n >= 0 )
        {
            *p_len = n;
            if( n == 0 )
                status = RC_IO_EOF;
        }
        else
        {
            status = RC_ERROR;
        }
    }

/* return */
    return status;
}


/*---------------------------------------------------------------------------
 * NAME
 *      io_std_seek - 
 *
 * SYNOPSIS
 */
INT io_std_seek( IN IO_FILE p_io, IN UINT64 p_pos )
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
    fprintf( stderr, "std_seek\n" );

/* processing. */
    if( status == RC_OK )
    {
        lseek( p_io->pint, p_pos, SEEK_SET );
    }

/* return */
    return status;
}


/*---------------------------------------------------------------------------
 * NAME
 *      io_std_write - 
 *
 * SYNOPSIS
 */
INT io_std_write
        ( IN IO_FILE p_io, IN void *p_data, IN INT p_size, OUT INT * p_len )
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
    if( status == RC_OK )
    {
        INT n;

        n = write( p_io->pint, p_data, p_size );
        if( n >= 0 )
        {
            *p_len = n;
        }
        else
        {
            err_set_syserr( "write", __FILE__, __LINE__ );
            status = RC_ERROR;
        }
    }

/* return */
    return status;
}


/*---------------------------------------------------------------------------
 * NAME
 *      io_ungetc - 
 *
 * SYNOPSIS
 */
INT io_ungetc( IN IO_FILE p_io, IN INT p_char )
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
    assert( p_io );
    assert( p_char > 0 );

/* environment check */
    assert( p_io->unget_count < IO_UNGET_BUFSIZE );
    assert( p_io->unget_count >= 0 );

/* processing. */
    if( status == RC_OK )
    {
        p_io->unget_count++;
        p_io->unget_buffer[IO_UNGET_BUFSIZE - p_io->unget_count] = p_char;
    }

/* return */
    return status;
}


/*---------------------------------------------------------------------------
 * NAME
 *      io_write - write data to stream.
 *
 * SYNOPSIS
 */
INT io_write( IN IO_FILE p_io, IN void *p_data, IN INT p_len, OUT INT * p_n )
/*
 * DESCRIPTION
 * 
 * PARAMETERS
 *      p_io - output stream.
 *      p_data - data to write.
 *      p_len - size of data to write.
 *      p_n - return value of acutally written amount of data.
 * 
 * RETURN VALUES
 *      error code.
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
    assert( p_io != NULL );
    assert( p_io->t != NULL );
    assert( p_io->t->write != NULL );

/* processing. */
    if( status == RC_OK )
    {
        INT len = p_len;
        *p_n = 0;
        if( len == -1 )
            len = strlen( p_data );
        status = ( p_io->t->write ) ( p_io, p_data, len, p_n );
        assert( len >= *p_n );
    }

/* return */
    return status;
}
