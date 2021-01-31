////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
////////////////////////////////////////////////////////////////////////////////
#include <hape/platform.h>
#include <hape/io.h>
// #include "io_int.h"

////////////////////////////////////////////////////////////////////////////////
#define IO_UNGET_BUFSIZE 8
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char *argv[] )
{
    IO_FILE io = NULL;
    IO_FILE out = NULL;
    INT status = RC_OK;
    INT i = 0;
    INT j = 0;
    INT c = 0;
    CHAR buf[80];

    ( void )argc;
    assert( argc == 2 );
    status = io_connect_file( &io, argv[1], "r" );
    if( status != RC_OK )
        fprintf( stderr, "io_fopen %s -> %04lx\n", argv[1], status );
    status = io_connect_file( &out, NULL, "w" );
    if( status != RC_OK )
        fprintf( stderr, "io_fopen -> %04lx\n", status );

    for( i = 0; i < IO_UNGET_BUFSIZE; i++ )
        io_ungetc( io, 'A' + i );
    i = 0;

    io_read( io, buf, 16, &i );
    for( j = 0; j < i; j++ )
        printf( "%02X ", buf[j] );
    for( j = 0; j < i; j++ )
        io_putc( out, isprint( buf[j] ) ? buf[j] : '.' );
    io_putc( out, '\n' );

    i = 0;
    j = 0;

    while( c >= 0 )
    {
        c = io_getc( io );
        if( c < 0 )
            break;
        buf[i] = c;
        printf( "%02lX ", c );
        if( i == 15 )
        {
            for( j = 0; j < 16; j++ )
                io_putc( out, isprint( buf[j] ) ? buf[j] : '.' );
            io_putc( out, '\n' );
        }
        i = ( i + 1 ) & 0x0f;
    }

    io_close( &io );
}
