#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "con.h"

#define clear() printf("\033[H\033[J")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

#define MAX_BUF 100
#define MAX_FIELD 10

#define KEY_UP          0x0141
#define KEY_DOWN        0x0142
typedef unsigned long long KEYSPEC;

struct keydef {
    KEYSPEC spec;
    int cmd;
};


struct keydef keydefs[] = {
    {.spec = 0x1b5b48, cmd = 1},
    {.spec = 0x1b5b, cmd = 0},                   // cmd == 0 -> it's a prefix, get next char
    {.spec = 0x1b, cmd = 0},                     // cmd == 0 -> it's a prefix, get next char
    {.spec = 0}                                  //last spec
};


struct field {
    int x;
    int y;
    char *label;
    char buf[MAX_BUF];
    int len;
};

#define MAX_KEY 5
KEYSPEC c;
KEYSPEC keys[MAX_KEY];
int fld;
struct field fields[MAX_FIELD] = {
    {.x = 5,.y = 3,.label = "TEST 1",.len = 2},
    {.x = 5,.y = 4,.label = "TEST 2",.len = 2},
    {.x = 5,.y = 5,.label = "TEST 3",.len = 2}
};


void handle_key( int c ) {
    ( void )c;
    for( uint_t i = MAX_KEY; i > 0; i-- )
        keys[i] = keys[i - 1];
    keys[0] = c;
}

void show_data(  ) {
    struct field *f;
    gotoxy( 1, 1 );
    printf( "key:" );
    for( uint_t i = 0; i < MAX_KEY; i++ )
        printf( " %4x", keys[i] );
    for( uint_t i = 0; i < MAX_FIELD; i++ ) {
        f = fields + i;
        if( f->label ) {
            gotoxy( f->x, f->y );
            printf( "%s : ", f->label );
        }
    }

    f = fields + fld;
    gotoxy( ( int )( f->x + strlen( f->label ) + 3 ), f->y );

}


int main( void ) {
    con_init(  );
    clear(  );


    for( uint_t i = 0; i < 10; i++ ) {
        show_data(  );
        c = con_getch(  );

        while( true ) {
            struct keydef *ks;
            uint_t i = 0;
            while( true ) {
                ks = keydefs + i;
                if( ks->spec == 0 || c == ks->spec ) {
                    break;
                }
                i++;
            }
            if( ks->spec == 0) break; // 
            
            if( ks->cmd > 0 )
            {
                printf("CMD: %d %lx", d, c);
                break;
            }
            c = c << 8 | con_getch();
        }
        handle_key( c );
    }
    con_exit(  );
    return 0;
}
