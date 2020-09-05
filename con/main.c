#include <stdio.h>

#include "con.h"

#define clear() printf("\033[H\033[J")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

int main( void ) {

    printf( "Enter your number in the box below\n"
            "+-----------------+\n"
            "|                 |\n"
            "+-----------------+\n" "\n" "\n" "\n" "===================\n" );
    gotoxy( 2, 3 );


    for( int i = 0; i < 10; i++ ) {
        int buf = con_getch();
        gotoxy( 2, 3 + i );
        fprintf(stdout, "%d ", buf);
        fputc( buf, stdout );
    }
    return 0;
}
