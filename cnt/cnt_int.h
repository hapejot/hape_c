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

typedef struct _cnt_elem * CNT_ELEM;

/*
 * alternativ könnte man auch eine Liste der Spalten als linked List aufbauen
 * und dann nur noch die einzelnen Zellen abspeichern, wobei die dann als
 * Kennzeichen nur noch die Row/Column Nummer als Id haben.
 */

struct _cnt_cell
{
    INT             row;
    INT             col;
    INT             size;
    void          * val;
    INT             flg;
};

struct _cnt 
{
    INT             check_sum;
    MEM_ARENA       arena;
    INT             max_row;
    INT             max_col;
    INT             count_cell;     /* Anzahl der belegten Zellen */
    INT             max_cell;       /* Reservierte Links in 'cell' */
    CNT_HEAD        first_col;
    CNT_CELL      * cell;
};


#endif
