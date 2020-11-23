/* cnt_create.c */
API CNT cnt_create(void);
API CNT cnt_create_a(MEM_ARENA p_arena);
/* cnt.c */
API void cnt_set_val_b(CNT cnt, char *name, void *data);
API void cnt_set_val_d(CNT cnt, char *name, CNT_DATA *data);
/* main.c */
API int main(void);
