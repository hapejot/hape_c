////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
////////////////////////////////////////////////////////////////////////////////
#include <hape/cnt.h>
#include "cnt_int.h"
#include "cnt_json_parser.h"
////////////////////////////////////////////////////////////////////////////////
#define   YYCTYPE     char
#define   YYCURSOR    s->cur
#define   YYMARKER    s->ptr
////////////////////////////////////////////////////////////////////////////////
CNT_TOKEN *cnt_tok_new(int);
CNT_TOKEN *cnt_tok_str(int, int, const char*);
////////////////////////////////////////////////////////////////////////////////


CNT_TOKEN *cnt_tok_new(int type){
    CNT_TOKEN *r = malloc(sizeof(*r));
    r->type = type;
    r->len = 0;
    return r;
}

CNT_TOKEN *cnt_tok_str(int type, int len, const char* data){
    CNT_TOKEN *r = calloc(1, sizeof(*r));
    r->type = type;
    r->len = len;
    assert(len < 100);
    memcpy(r->data, data, len);
    return r;
}

CNT_TOKEN *scan( Scanner * s )
{
  const char* str_start;
  const char* str_end;

  regular:
    if( s->cur >= s->end )
    {
        return 0;
    }
    s->top = s->cur;
/*!stags:re2c format = 'const char *@@;'; */
/*!re2c
  re2c:yyfill:enable = 0;
  re2c:flags:tags = 1;
 
  ALPHANUMS = [a-zA-Z0-9]+;
  whitespace = [ \t\v\f]+;
  dig = [0-9];
  let = [a-zA-Z_];
  hex = [a-fA-F0-9];
  int_des = [uUlL]*;
  any = [\000-\377];
*/

/*!re2c
  "/*"            { goto comment; }
  ","             { return cnt_tok_new(TK_COMMA); }
  ":"             { return cnt_tok_new(TK_COLON); }
  "["             { return cnt_tok_new(TK_LBRACK); }
  "]"             { return cnt_tok_new(TK_RBRACK); }
  "{"             { return cnt_tok_new(TK_LBRACE); }
  "}"             { return cnt_tok_new(TK_RBRACE); }
  ["]@str_start([^"]+)@str_end["]   { 
                    return cnt_tok_str(TK_STRING, str_end - str_start, str_start); }
  whitespace      { goto regular; }

  "\r\n"|"\n"     { s->pos = s->cur; s->line++; goto regular; }

  any
  {
    printf("unexpected character: %c\n", s->cur[-1]);
    goto regular;
  }
*/

  comment:
/*!re2c
  "*/ "          { goto regular; }
  " \ n "          { s->line++; goto comment; }
  any           { goto comment; }
*/
}

