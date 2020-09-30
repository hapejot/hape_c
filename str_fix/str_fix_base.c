
#include "str_fix.h"

#include <malloc.h>
#include <string.h>
#include <assert.h>
//#define assert(x)

#define STRMAX 800000
//#define STRMAX 8000
static char _str[STRMAX];
static size_t _top = 1;

static inline int max( int a, int b ) {
    return a < b ? b : a;
}

uint32_t boyer_moore( char *haystack, uint32_t m, char *needle ) {
    uint32_t n = strlen( needle ) + 1;
    static int skip[256];
    uint32_t shift = 0;

    for( uint32_t i = 0; i < 256; i++ )
        skip[i] = -1;
    for( uint32_t i = 0; i <= n; i++ )
        skip[( uint32_t ) needle[i]] = i;

    while( shift + n < m ) {
        int j = n - 1;
//        assert( j < ( int )n );
//        assert( shift + j < m );
        while( j >= 0 && needle[j] == haystack[shift + j] ) {
            j--;
//            assert( j < ( int )n );
//            assert( shift + j < m );
        }
        if( j < 0 ) {
            return shift;
        }
        else {
//            assert( shift + j < m );
            shift += max( 1, j - skip[( uint32_t ) haystack[shift + j]] );
        }
    }
    return 0;
}

API uint32_t str_find( char *str ) {
    return boyer_moore( _str, _top, str );
}


API uint32_t str_fix( char *str ) {
    uint32_t n = strlen( str ) + 1;     // include zero terminator
    uint32_t r = str_find( str );
    if( r == 0 && ( _top + n ) < STRMAX ) {
        r = _top;
        strcpy( _str + _top, str );
        _top += n;
    }
    return r;
}


API const char *str_fix_str( uint32_t v ) {
    const char *r = "<no symbol>";
    r = _str + v;
    return r;
}
