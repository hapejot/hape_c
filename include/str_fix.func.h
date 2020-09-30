/* str_fix_base.c */
API void prepare_next(uint32_t *next, uint32_t len, char *needle);
API uint32_t kmp_search(char *haystack, uint32_t len, char *needle);
API uint32_t str_find(char *str);
API uint32_t str_fix(char *str);
API const char *str_fix_str(uint32_t v);
