/* mem_alloc_g.c */
API void *mem_alloc_g(INT p_size, void **p_group, char *p_source, int p_line);
/* mem_globals.c */
/* mem_arena_alloc.c */
API void *mem_arena_alloc(MEM_ARENA arena, INT nbytes, char *file, INT line);
/* mem_arena_free.c */
API INT mem_arena_free(MEM_ARENA arena);
/* mem_arena_dispose.c */
API INT mem_arena_dispose(MEM_ARENA *ap);
/* mem_arena_calloc.c */
API void *mem_arena_calloc(MEM_ARENA arena, INT count, INT nbytes, char *file, INT line);
/* mem_arena_new.c */
API MEM_ARENA mem_arena_new(void);
/* mem_free_g.c */
API INT mem_free_g(void **p_group);
/* print_hex_dump.c */
API INT print_hex_dump(INT p_len, CHAR *p_data);
