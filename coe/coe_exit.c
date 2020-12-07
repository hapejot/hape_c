#include "coe_int.h"
void con_exit() {
    struct termios t;

    tcgetattr( 0, &t );
    t.c_lflag |= ICANON + ECHO;
    tcsetattr( 0, TCSANOW, &t );
}
