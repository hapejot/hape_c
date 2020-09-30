#ifndef CNT_H
#define CNT_H
#include "platform.h"
#include "mem.h"
typedef struct _cnt *CNT;
typedef struct _cnt_head *CNT_HEAD;
typedef struct _cnt_csvopts CNT_CSVOPTS;
typedef struct _cnt_cell * CNT_CELL;

#define CNT_SUBCONT     1
#define CNT_ATOM        0
#define CNT_KINDMASK    1

#define CNT_ISCONT(x)   ((x & CNT_KINDMASK) == CNT_SUBCONT)
#define CNT_ISATOM(x)   ((x & CNT_KINDMASK) == CNT_ATOM)
struct _cnt_head {
    char *name;
    INT col;
    INT width;
    CNT_HEAD next;
};

struct _cnt_csvopts {
    INT no_title;
    CHAR sep_char;
};

#include "io.h"

#include "cnt.func.h"

#endif
