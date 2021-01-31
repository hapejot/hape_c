#ifndef MEM_H
#define MEM_H
#include <hape/platform.h>
typedef struct MEM_ARENA *MEM_ARENA;

extern void *mem_arena_alloc( MEM_ARENA arena, INT nbytes, char *file,
                              INT line );
extern void *mem_arena_calloc( MEM_ARENA arena, INT count, INT nbytes,
                               char *file, INT line );
extern INT mem_arena_dispose( MEM_ARENA * ap );
extern INT mem_arena_free( MEM_ARENA arena );
extern MEM_ARENA mem_arena_new( void );

extern void *mem_alloc_g( INT, void **, char *, int );
extern INT mem_free_g( void ** );
extern INT print_hex_dump( INT p_len, CHAR * p_data );

#define MEM_A_ALLOC(p, a) mem_arena_alloc((p), (a), __FILE__, __LINE__)

#endif
