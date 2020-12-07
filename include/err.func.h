/* err_set_err.c */
API INT err_set_err(INT p_errno, char *p_szMsg, char *p_szComp, char *p_szFile, INT p_nLine);
/* err_log.c */
API void err_log_add(ERR_LOG_ENTRY *entry);
API void err_log_dump(void);
/* err_set_syserr.c */
API INT err_set_syserr(char *p_szCall, char *p_szFile, INT p_nLine);
/* err_except.c */
API void err_throw(ERR_LOG_ENTRY *entry);
API _Bool err_catch(void);
/* err_globals.c */
/* err_print.c */
API INT err_print(void);
