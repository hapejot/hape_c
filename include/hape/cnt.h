#ifndef CNT_H
#define CNT_H
#include <hape/mem.h>
typedef struct _cnt           * CNT;
typedef struct _cnt_head      * CNT_HEAD;
typedef struct _cnt_csvopts     CNT_CSVOPTS;
typedef struct _cnt_col       * CNT_COL;
typedef void                  * CNT_BYTES;
typedef char                  * CNT_COL_NAME;
typedef int                     CNT_IDX;

typedef struct _cnt_flag {
    int cnt:1;
}   CNT_FLAG;

typedef struct _cnt_dat {
    CNT_IDX     l;      // length
    CNT_FLAG    f;
    char       *d;   // data guarded by 0 byte at the end.
} CNT_DATA;

struct _cnt_col {
    char        name[20];
    CNT_IDX     pos;
} ;



#define CNT_SUBCONT     1
#define CNT_ATOM        0
#define CNT_KINDMASK    1

#define CNT_ISCONT(x)   ((x & CNT_KINDMASK) == CNT_SUBCONT)
#define CNT_ISATOM(x)   ((x & CNT_KINDMASK) == CNT_ATOM)

// new CNT_IDXerface:
// container throws error, when there is an error.
// there is no return code.

// create container with default arena
extern CNT          cnt_create( );
// create container with specified arena
extern CNT          cnt_create_a(MEM_ARENA); 

extern void         cnt_dump( CNT p_cnt );

extern CNT_IDX      cnt_lines( CNT );
extern CNT_IDX      cnt_columns( CNT );
extern CNT_COL_NAME cnt_colname( CNT, CNT_IDX );

extern CNT_BYTES    cnt_val_b(CNT, CNT_COL_NAME );
extern CNT_DATA     cnt_val_d(CNT, CNT_COL_NAME );

extern CNT_BYTES    cnt_idx_b(CNT, CNT_COL_NAME, CNT_IDX );
extern CNT_DATA     cnt_idx_d(CNT, CNT_COL_NAME, CNT_IDX );

extern void         cnt_set_val_b(CNT, CNT_COL_NAME, CNT_BYTES );
extern void         cnt_set_val_d(CNT, CNT_COL_NAME, CNT_DATA* );
extern void         cnt_set_idx_b(CNT, CNT_COL_NAME, CNT_IDX, CNT_BYTES );
extern void         cnt_set_idx_d(CNT, CNT_COL_NAME, CNT_IDX, CNT_DATA* );

extern void         cnt_release( CNT );
extern CNT          cnt_dup( CNT );
extern void         cnt_ins( CNT, CNT_IDX );
extern CNT          cnt_row_cpy( CNT dst, CNT_IDX dst_idx, CNT src, CNT_IDX src_idx );

extern void         cnt_dump(CNT);

extern void         cnt_json(CNT);
extern CNT          cnt_json_imp(char*);

#endif
