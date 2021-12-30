
#define _(String) (String)

// #define DBG_WHERE() fprintf(stderr,"%s[%d]\n", __FILE__, __LINE__)
#define DBG_WHERE()
#define ALLOC(a, s) (mem_arena_calloc(a, (s), 1, __FILE__,__LINE__))

#define PAGE_TYPE       0x10000000
#define PAGE_TYPE_MASK  0xfffffff0
#define CELLS_LEVEL_1   (PAGE_TYPE | 1)
#define CELLS_LEVEL_2   (PAGE_TYPE | 2)
#define COLLS           (PAGE_TYPE | 3)

typedef int (*CMP)(void*, void*);


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
    CNT_IDX         used;
    CNT_VECTOR_PAGE cells;
    CNT_VECTOR_PAGE cols;
} ;

struct _token {
    unsigned short type;
    CNT_IDX         len;
    char    data[100];
};

#define VECTOR_PAGE_MAX     0x2000
typedef struct _vector_page {
    unsigned int        type;
    CNT_IDX             used;
    void              * ptr[VECTOR_PAGE_MAX];
}   VECTOR_PAGE;



typedef struct Scanner
{
    char *top, *cur, *ptr, *pos;
    char* end;
    int line;
} Scanner;

CNT_TOKEN* scan(struct Scanner*);



