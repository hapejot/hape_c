////////// GENERAL INCLUDES ////////////////////////////////////////////////////
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
/////////// PROJECT INCLUDES ///////////////////////////////////////////////////
#include <hape/platform.h>
#include <hape/err.h>
#include "err_int.h"

////////// DEFINES /////////////////////////////////////////////////////////////
#define MAX_LOG 1000

////////// GLOBAL DATA /////////////////////////////////////////////////////////

ERR_FRAME *g_err_stack;


static jmp_buf _exitbuf;
static struct gd {
    int len;
    ERR_LOG_ENTRY log[MAX_LOG];
} gd;
////////// LOCAL FUNCTIONS /////////////////////////////////////////////////////
void err_throw( ERR_LOG_ENTRY * entry ) {
    err_log_add( entry );
    longjmp( _exitbuf, true );
}

bool err_catch(  ) {
    return ( bool )setjmp( _exitbuf );
}

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

/*---------------------------------------------------------------------------*/
INT err_print(  )
/*---------------------------------------------------------------------------*/
{
    INT rc = RC_OK;

/* end of local var. */

/* parameter check */

/* environment check */

/* processing. */
    if( rc == RC_OK ) {
        printf( "*************************************************\n" );
    }

/* return */
    return rc;
}


ERR_EXC g_err_current;

void err_raise( ERR_EXC * exc, ... ) {
    va_list ap;
    va_start( ap, exc );
    memset( &g_err_current, 0, sizeof( g_err_current ) );
    strcpy( g_err_current.msg, exc->msg );
    for( int i = 0; i < 4; i++ ) {
        char *arg = va_arg( ap, char * );
        if( arg )
            strcpy( g_err_current.arg[i], arg );
        else
            break;
    }
    longjmp( g_err_stack->env, err_raised );
}
