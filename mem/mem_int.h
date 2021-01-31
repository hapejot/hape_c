
struct MEM_ARENA
{
    MEM_ARENA       prev;
    char          * avail;
    char          * limit;
};

union align
{
    INT     i;
    void    * p;
    double  d;
    INT64   i64;
};

union header
{
    struct MEM_ARENA    b;
    union align         a;
};

struct MEM_GLOBALS
{
    FILE      * log;
    INT         log_flag;
    INT         cnt;
    INT         alloc_cnt;
    INT         free_cnt;
};


#ifndef GLOBALS
extern struct MEM_GLOBALS   g_mem;
#endif

