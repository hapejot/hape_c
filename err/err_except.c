#include <hape/err.h>
#include <stdio.h>
#include <setjmp.h>
#include <stdbool.h>

static jmp_buf _exitbuf;

void err_throw( ERR_LOG_ENTRY *entry ) {
    err_log_add( entry );
    longjmp( _exitbuf, true );
}

bool err_catch(){
    return (bool)setjmp( _exitbuf );
}


