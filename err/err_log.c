#include <hape/err.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define MAX_LOG 1000
static struct gd {
    int len;
    ERR_LOG_ENTRY log[MAX_LOG];
} gd;

void err_log_add( ERR_LOG_ENTRY * entry ) {
    if( ( gd.len + 2 ) >= MAX_LOG ) {
        ERR_LOG_ENTRY err = {.pkg = "ERR",.id = "001",.typ = 'E' };
        sprintf( err.msg, "too many messages, stopping processing." );
        memcpy( gd.log + gd.len, &err, sizeof( err ) );
        gd.len++;
        err_log_dump(  );
        exit( 1 );
    }
    ERR_LOG_ENTRY *e = gd.log + gd.len++;
    memcpy( e, entry, sizeof( ERR_LOG_ENTRY ) );
    char *x = ( char * )e;
    for( int i =
         sizeof( entry->typ ) + sizeof( entry->pkg ) + sizeof( entry->id ) -
         1; i >= 0; i-- ) {
        x[i] = toupper( x[i] );
    }
}
void err_log_dump(  ) {
    for( int i = 0; i < gd.len; i++ ) {
        ERR_LOG_ENTRY *e = gd.log + i;
        fprintf( stderr, "%s%s %c - %s\n", e->pkg, e->id, e->typ, e->msg );
    }
}
