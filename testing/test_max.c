#include "platform.h"
#include "str_fix.h"
#include <stdbool.h>
#include <stdio.h>
int main(  ) {
/* Arrange */
    char buffer[10];
    uint_t max = 0;
    uint_t start = 9998;
    for( uint_t i = start;; i++ ) {
        sprintf( buffer, "X%d", i );
        uint32_t x = str_fix( buffer );
// fprintf(stderr,"%d %s\n", x, buffer);
        if( x == 0 ) {
            max = i - 1;
            break;
        }
    }
    printf( "MAX: %u\n", max );
/* Act     */
    for( uint_t i = start; i < max; i++ ) {
        sprintf( buffer, "X%d", i );
        str_fix( buffer );
    }

/* Assert  */
}
