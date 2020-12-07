#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "coe.h"

// got up left corner clear rest of the screen
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
    {.spec = 0x1b5b317e,.cmd = 1},
    {.spec = 0x1b5b31,.cmd = 0},
    {.spec = 0x1b5b41,.cmd = 2},
    {.spec = 0x1b5b42,.cmd = 3},
    {.spec = 0x1b5b43,.cmd = 4},
    {.spec = 0x1b5b44,.cmd = 5},
    {.spec = 0x1b5b,.cmd = 0},                   // cmd == 0 -> it's a prefix, get next char
    {.spec = 0x1b,.cmd = 0},                     // cmd == 0 -> it's a prefix, get next char
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
int pos;
struct field fields[MAX_FIELD] = {
    {.x = 5,.y = 3,.label = "Vorname",.len = 30},
    {.x = 5,.y = 4,.label = "Nachname",.len = 30},
    {.x = 5,.y = 5,.label = "Straße",.len = 30},
    {.x = 5,.y = 6,.label = "PLZ",.len = 5},
    {.x = 20,.y = 6,.label = "Ort",.len = 30}
};


void handle_key( KEYSPEC c ) {
    for( uint_t i = MAX_KEY - 1; i > 0; i-- )
        keys[i] = keys[i - 1];
    keys[0] = c;

    struct field *f = fields + fld;
    if( strlen( f->buf ) < (ulong)f->len )
        f->buf[pos++] = ( char )c;
}

void show_data(  ) {
    struct field *f;
    gotoxy( 1, 1 );
    printf( "key:" );
    for( uint_t i = 0; i < MAX_KEY; i++ )
        printf( " %8llx", keys[i] );
    for( uint_t i = 0; i < MAX_FIELD; i++ ) {
        f = fields + i;
        if( f->label ) {
            gotoxy( f->x, f->y );
            printf( "%s : ", f->label );
            printf( "%s", f->buf );
        }
    }

    f = fields + fld;
    assert( f->label );
    int x = f->x;
    x += strlen( f->label );
    x += 3;
    x += pos;
    gotoxy( x, f->y );
}


int main( void ) {
    con_init(  );
    clear(  );


    for( uint_t i = 0; i < 80; i++ ) {
        show_data(  );
        c = con_getch(  );
        struct keydef *ks;

        while( true ) {
            uint_t i = 0;
            while( true ) {
                ks = keydefs + i;
                if( ks->spec == 0 || c == ks->spec ) {
                    break;
                }
                i++;
            }
            if( ks->spec == 0 )
                break;                           // 

            if( ks->cmd > 0 ) {
                break;
            }
            c = c << 8 | con_getch(  );
        }
        if( ks && ks->cmd ) {
            gotoxy( 1, 2 );
            printf( "CMD: %d %llx", ks->cmd, c );
            switch ( ks->cmd ) {
                case 2:
                    fld--;
                    pos = 0;
                    break;
                case 3:
                    fld++;
                    pos = 0;
                    break;
                case 4:
                    if( pos < fields[fld].len )
                        pos++;
                    break;
                case 5:
                    if( pos > 0 )
                        pos--;
                    break;
            }
        }
        else
            handle_key( c );
    }
    con_exit(  );
    return 0;
}
