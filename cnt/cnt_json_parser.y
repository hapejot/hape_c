% include
{
////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
////////////////////////////////////////////////////////////////////////////////
#include <hape/cnt.h>
#include "cnt_int.h"
////////////////////////////////////////////////////////////////////////////////
    struct _data
    {
        int count;
        CNT_DATA d;
        int token;
        int row;
        CNT cnt;
    } gd;
}



%token_type { CNT_TOKEN* }
%token_destructor { (void)yypParser; free($$); }
%token_prefix TK_
%type expr { CNT_TOKEN* }

all ::= expr.

expr(A) ::= STRING(B). { A = B; }

expr(A) ::= LBRACK expr_list(B) RBRACK. { A = B; }

expr ::= start_row expr_pairs RBRACE.

start_row ::= LBRACE. { gd.row++; 
     fprintf(stderr, "R: %d\n", gd.row); }

expr_list ::= expr.  

expr_list ::= expr_list COMMA expr. 



pair ::= STRING(A) COLON expr(B). { 
     CNT_DATA data;
     data.l = B->len;
     data.d = B->data;
     cnt_set_idx_d(gd.cnt, A->data, gd.row, &data);
     fprintf(stderr, "E: %s <- %.*s\n",  A->data,
                                          B->len, B->data); }

expr_pairs ::= pair .

expr_pairs ::= expr_pairs COMMA pair.

%syntax_error { 
(void)yypParser;
(void)yymajor;
(void)yyminor;
fprintf( stderr, "syntax error\n" ); }

%parse_failure { fprintf( stderr, "parsing error\n" ); }

%parse_accept { fprintf( stderr, "parsing accepted\n" ); }

%code
{
    CNT cnt_json_imp( char *json )
    {

        Scanner scanner;
        void *xp;
        xp = ParseAlloc( malloc );
/* Start scanning */
        scanner.top = json;
        scanner.cur = json;
        scanner.pos = json;
        scanner.line = 1;
        scanner.end = json + strlen( json );
        gd.count = 0;
        gd.cnt = cnt_create( );

        CNT_TOKEN *tok = NULL;
        gd.row = -1;
        while( ( tok = scan( &scanner ) ) )
        {
            Parse( xp, tok->type, tok );
        }
        Parse( xp, 0, NULL );
        ParseFree( xp, free );

        cnt_dump(gd.cnt);
        return 0;
    }
}
