#include "fmt_int.h"
#include "mem.h"

int main(  ) {
    IO_FILE out;
    INT status = RC_OK;

    io_connect_file( &out, NULL, "w" );
    fmt_format( out, "<%s>\n", "Hallo Welt" );
    fmt_format( out, "<%20s>\n", "Hallo Welt" );
    fmt_format( out, "<%-20s>\n", "Hallo Welt" );

    for( int i = 1; i < 10000; i *= 2 ) {
        fmt_format( out, "<%5d>\n", 0 );
        fmt_format( out, "<%5d>\n", 1*i );
        fmt_format( out, "<%7d>\n", 12345678*i );
        fmt_format( out, "<%8d>\n", 12345678*i );
        fmt_format( out, "<%9d>\n", 12345678*i );
        fmt_format( out, "<%10d>\n", 12345678*i );
    }

    return status;
}
