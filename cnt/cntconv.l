%{
#include "platform.h"
#include "mem.h"
#include "cnt.h"

#define TOK_NAME        1
#define TOK_VALUE       2
#define YY_USE_PROTOS

MEM_ARENA   arena   = NULL;

struct name {
    char      * str;
    INT         size;
};

CNT             cnt         = NULL;
CNT             ccnt        = NULL;
CNT             rcnt        = NULL;
INT             status;
INT             etype;
INT             elen;
void          * eval        = NULL;
struct name     name    = {NULL,0};
struct name     ref     = {NULL,0};
INT             idx         = 0;


void set_value( INT, INT, void* );
void decode_idx(char*, char**, INT*);
void set_name( struct name*, char* );
%}

/* a decimal digit / exponent */
DIGIT ([0-9])
EXP ([Ee][+-]?[0-9]+)

LETTER		([a-zA-Z])
ALPHNUM		({LETTER}|{DIGIT})
BLANK		([ \t])
NONBLANK	([^ \t\n\r])
NONBLANKORCOMMA	([^ \t\n\r','])

/* octal and hexademical constants */
OCTDIGIT ([0-7])
HEXDIGIT ([0-9a-fA-F])

/* escape seq such as \' or \\ or \n */
/* WATCOM does things like '\0x0d' instead of '\x0d' (as it should be) */
ESCAPEOCT (\\{OCTDIGIT}{1,3})
ESCAPEHEX (\\0?[xX]({HEXDIGIT}{1,3}))
ESCAPECHAR (\\[0'"?abfnrtv\\])
ESCAPESEQ ({ESCAPECHAR}|{ESCAPEOCT}|{ESCAPEHEX})

/* stuff for being able to scan files Unix/Mac */
RETURN ((\n)|(\r)|(\r\n))

/* an escape seq or any char other than backslash, double quote or newline */
CCHAR ({ESCAPESEQ}|[^\"\\\n\r])
  
/* SGI has extensions such as LUL as int.suffix */
INTSUFFIX ([uUlL]{1,3})
DBLSUFFIX ([fFlL]{1,3})
NCHAR     ({ALPHNUM}|_)
%s EQUALS VALUE COMMA NAME REF
%%

<INITIAL>\"             {
                            BEGIN NAME;
                            if( !cnt )
                                cnt_create(arena, &cnt);
                            assert( cnt );
                            ccnt = cnt;
}
<NAME>{DIGIT}+"."{NCHAR}+ {
                            char  * s;
                            
                            decode_idx( yytext, &s, &idx );
                            set_name( &name, s );
}
<NAME>{DIGIT}+          {
                            set_name( &name, NULL );
                            idx = atol(yytext);
}
<NAME>{NCHAR}+          {
                            set_name( &name, yytext );
                            idx = 0;
}
<NAME>[.]               {
                            if( !name.str )
                            {
                                char        b[20];
                                sprintf( b, "%d", idx );
                                idx = 0;
                                set_name( &name, b );
                            }
                            assert( name.str );
                            status = cnt_get_idx_val(ccnt, idx, name.str, &etype, &elen, &eval ); 
                            if( RC_OK == status )
                                ccnt = eval;
                            else
                            {
                                cnt_create( arena, (CNT*)&eval );
                                cnt_set_idx_val(ccnt, idx, name.str, CNT_SUBCONT, 0, eval );
                                ccnt = eval;
                            }
}
<NAME>\"                {
                            BEGIN EQUALS;                          }
<EQUALS>=               {
                            BEGIN VALUE;                           }
<VALUE>\"{CCHAR}*\"     {
                            BEGIN COMMA;
                            set_value( CNT_ATOM, -1, yytext );
}
<VALUE>{DIGIT}+("."{DIGIT}*)? {
                            BEGIN COMMA;
                            set_value( CNT_ATOM, -1, yytext );
}
<VALUE>"see"                {
                            BEGIN REF;
                            rcnt = cnt;
                            fprintf( stderr, "REF\n" );
}
<REF>{DIGIT}+"."{NCHAR}+"." {
                            INT     i;
                            char  * s;
                            yytext[strlen(yytext)-1] = 0;
                            decode_idx(yytext, &s, &i);
                            cnt_get_idx_val( rcnt, i, s, &etype, &elen, &eval );
                            assert( etype == CNT_SUBCONT );
                            fprintf( stderr, "*[%d].%s\n", i, s );
                            rcnt = eval;
}
<REF>{DIGIT}+"."{NCHAR} {
                            INT     i;
                            char  * s;
                            yytext[strlen(yytext)-1] = 0;
                            decode_idx(yytext, &s, &i);
                            cnt_get_idx_val( rcnt, i, s, &etype, &elen, &eval );
                            assert( etype == CNT_SUBCONT );
                            set_value( etype, elen, eval );
                            fprintf( stderr, "*GET[%d].%s\n", i, s );
}

<REF>{NCHAR}+"."        {
                            yytext[strlen(yytext)-1] = 0;
                            fprintf( stderr, "*%d* %s\n", __LINE__, yytext );
                            status = cnt_get_val( rcnt, yytext, &etype, &elen, &eval );
                            assert( RC_OK == status );
                            assert( etype == CNT_SUBCONT );
                            rcnt = eval;
}
<REF>{DIGIT}+           {
                            char        b[20];
                            INT i = atol(yytext);
                            sprintf( b, "[%d]", i );
                            fprintf( stderr, "*GET %s\n", b );
                            if( !name.str )
                            {
                                cnt_copy_row( rcnt, atol(yytext), idx, ccnt );
                                fprintf( stderr, "* copy %d -> %d\n", i, idx );
                            }
                            else
                            {
                                cnt_get_val( rcnt, b, &etype, &elen, &eval );
                                set_value( etype, elen, eval );
                            }
                            BEGIN COMMA;
}
<REF>{NCHAR}+           {
                            fprintf( stderr, "*GET %s\n", yytext );
                            cnt_get_val( rcnt, yytext, &etype, &elen, &eval );
                            set_value( etype, elen, eval );
                            BEGIN COMMA;
}

<VALUE>true             {BEGIN COMMA;
                            set_value( CNT_ATOM, -1, yytext );
}
<VALUE>false            {BEGIN COMMA;
                            set_value( CNT_ATOM, -1, yytext );
}
<COMMA>,                {
                            BEGIN INITIAL;
}
[ \t\n\r]               {                                       }
%%

int yywrap() { return 1;}

void set_value( 
    INT         p_type,
    INT         p_len,
    void      * p_val
    )
{
    if( name.str )
        cnt_set_idx_val(ccnt, idx, name.str, p_type, p_len, p_val );
    else
    {
        cnt_set_idx_val(ccnt, idx, "NONAME", p_type, p_len, p_val );
    }
}

void set_name( 
    struct name   * p_struct,
    char          * p_name 
    )
{
    if( p_name )
    {
        INT     n = strlen(p_name);
        if( n >= p_struct->size )
        {
            p_struct->size = n + 1;
            p_struct->str = realloc( p_struct->str, p_struct->size );
        }
        strcpy( p_struct->str, p_name );
    }
    else
    {
        free(p_struct->str);
        p_struct->size = 0;
        p_struct->str = NULL;
    }
}

void decode_idx(
    char      * p_src,
    char     ** p_name,
    INT       * p_idx 
    )
{
    char  * s;

    assert( p_src );
    assert( p_name );
    assert( p_idx );
    s = strrchr( p_src, '.' );
    assert( s );
    s++;
    *p_idx = atol(yytext);
    *p_name = s;
}

int main( 
int argc,
char *argv[])
{
    int     t;
    IO_FILE     out;

    arena = mem_arena_new();

    io_connect_file( &out, "-", "w" );

    while ( (t=yylex()) != 0 )
        ;
    cnt_exp( out, cnt );
    return 0;
}

