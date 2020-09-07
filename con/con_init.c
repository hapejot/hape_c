#include "con_int.h"
void con_init() {
    struct termios t;

    tcgetattr( 0, &t );
    t.c_lflag &= ~ECHO + ~ICANON;
    tcsetattr( 0, TCSANOW, &t );
    fflush( stdout );
}
