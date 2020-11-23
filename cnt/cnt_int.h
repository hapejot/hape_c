#ifndef CNT_INT_H
#define CNT_INT_H

#include "cnt.h"
#include "mem.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <malloc.h>

// #define DBG_WHERE() fprintf(stderr,"%s[%d]\n", __FILE__, __LINE__)
#define DBG_WHERE()

typedef struct _cnt_cell * CNT_CELL;

struct _cnt_cell
{
    CNT_IDX         row;
    CNT_IDX         col;
    CNT_FLAG        flg;
    CNT_DATA        val;
};

struct _cnt 
{
    MEM_ARENA       arena;
    CNT_IDX         max_row;
    CNT_IDX         max_col;
    CNT_IDX         count_cell;     /* Anzahl der belegten Zellen */
    CNT_IDX         max_cell;       /* Reservierte Links in 'cell' */
    CNT_HEAD        first_col;
    CNT_CELL      * cell;
};

#endif
