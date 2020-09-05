#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>

#define clear() printf("\033[H\033[J")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

int main( void ) {
    char buf;
    ssize_t len;
    clear(  );
    printf( "ttyname:%s\n", ttyname( 0 ) );

    int in = STDIN_FILENO;      //open( ttyname( 0 ), O_RDONLY | O_NONBLOCK );

    struct termios old_tio;
/* get the terminal settings for stdin */
    tcgetattr( in, &old_tio );
/* we want to keep the old setting to restore them a the end */
/* disable canonical mode (buffered i/o) and local echo */
    old_tio.c_lflag &= ( ~ICANON );              /*& ~ECHOE  ); */
    printf("CAN:%d\n", old_tio.c_lflag & ICANON);
/* set the new settings immediately */
    int rc = tcsetattr( in, TCSANOW, &old_tio );
    assert(rc == 0);

    fd_set readfds;
    FD_ZERO( &readfds );
    FD_SET( in, &readfds );


    printf( "IN:%d\n", in );

    printf( "Enter your number in the box below\n"
            "+-----------------+\n"
            "|                 |\n"
            "+-----------------+\n" "\n" "\n" "\n" "===================\n" );
    gotoxy( 2, 3 );


    for( int i = 0; i < 10; i++ ) {
        select( in + 1, &readfds, NULL, NULL, NULL );
        buf = _read_kbd(0,1,0);
        gotoxy( 2, 3 + i );
        fprintf( stdout, "len: %ld, %d, %d ", len, EAGAIN == errno, buf );
        fputc( buf, stdout );
    }
    return 0;
}
