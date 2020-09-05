
#include "con.h"
#include <termios.h>
#include <stdio.h>

int con_getch(  ) {
    struct termios t;
    int c;

    tcgetattr( 0, &t );
    t.c_lflag &= ~ECHO + ~ICANON;
    tcsetattr( 0, TCSANOW, &t );
    fflush( stdout );
    c = getchar(  );
    t.c_lflag |= ICANON + ECHO;
    tcsetattr( 0, TCSANOW, &t );
    return c;
}
