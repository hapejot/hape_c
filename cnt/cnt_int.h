
#include "cnt.h"
#include "mem.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <malloc.h>

// #define DBG_WHERE() fprintf(stderr,"%s[%d]\n", __FILE__, __LINE__)
#define DBG_WHERE()
#define ALLOC(a, s) (mem_arena_calloc(a, (s), 1, __FILE__,__LINE__))

typedef struct _cnt_cell * CNT_CELL;
typedef struct _vector_page * CNT_VECTOR_PAGE;
typedef struct _token CNT_TOKEN;

struct _cnt_cell
{
    CNT_IDX         row;
    CNT_IDX         col;
    CNT_FLAG        flg;
    CNT_DATA       *val;
};

struct _cnt 
{
    MEM_ARENA       arena;
    CNT_IDX         max_row;
    CNT_IDX         max_col;
    CNT_VECTOR_PAGE cells;
    CNT_VECTOR_PAGE cols;
} ;

struct _token {
    int     type;
    int     len;
    char    data[100];
};



#define VECTOR_PAGE_MAX     100
typedef struct _vector_page {
    int type;
    int used;
    void* ptr[VECTOR_PAGE_MAX];
}   VECTOR_PAGE;


typedef struct Scanner
{
    char *top, *cur, *ptr, *pos;
    char* end;
    int line;
} Scanner;

CNT_TOKEN* scan(struct Scanner*);
