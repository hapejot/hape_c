#ifndef CNT_H
#define CNT_H
#include "mem.h"
typedef struct _cnt *CNT;
typedef struct _cnt_head *CNT_HEAD;
typedef struct _cnt_csvopts CNT_CSVOPTS;


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

extern INT cnt_create( MEM_ARENA, CNT * p_cnt );
extern INT cnt_dump( CNT p_cnt );
extern INT cnt_exp( IO_FILE p_file, CNT p_cnt );
extern INT cnt_exp_par( IO_FILE p_file, CNT p_cnt );
extern INT cnt_exp_json( IO_FILE p_file, CNT p_cnt );
extern INT cnt_imp( IO_FILE p_file, CNT p_cnt );
extern INT cnt_first_name( IN CNT p_cnt, OUT void **p_crs,
                           OUT CNT_HEAD * p_head );
extern INT cnt_get_dim( IN CNT p_cnt, OUT INT * p_rowcnt, OUT INT * p_colcnt,
                        OUT INT * p_cellcnt );
extern INT cnt_get_idx_val( CNT p_cnt, INT p_row, char *p_field, INT * p_flg,
                            INT * p_size, void **p_val );
extern INT cnt_get_val( CNT p_cnt, char *p_field, INT * p_flg, INT * p_size,
                        void **p_val );
extern INT cnt_next_name( INOUT void **p_crs, OUT CNT_HEAD * p_head );
extern INT cnt_set_idx_val( CNT p_cnt, INT p_row, char *p_field, INT p_flg,
                            INT p_size, void *p_val );
extern INT cnt_set_val( CNT p_cnt, char *p_field, INT p_flg, INT p_size,
                        void *p_val );
extern INT cnt_release( CNT );
extern INT cnt_split( IN char *, IN UINT, IN char separator_char, IN char *, INOUT CNT );
extern INT cnt_copy( IN CNT p_src, INOUT CNT p_dst );

extern INT cnt_check( IN CNT p_cnt );

extern INT
        cnt_copy_row
        ( IN CNT p_cnt,
          IN INT p_rownum, IN INT p_dest_rownum, OUT CNT p_row );

extern INT
        cnt_from_csv
        ( IN IO_FILE p_file, IN CNT_CSVOPTS p_opts, INOUT CNT p_cnt );



#endif
