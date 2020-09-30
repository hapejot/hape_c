/* cnt_dump.c */
API INT cnt_dump(CNT p_cnt);
/* cnt_split.c */
API INT cnt_split(char *p_data, UINT p_dlen, char p_sep, char *p_elempattern, CNT p_cnt);
/* cnt_exp_par.c */
API INT cnt_exp_par(IO_FILE p_file, CNT p_cnt);
/* cnt_get_dim.c */
API INT cnt_get_dim(CNT p_cnt, INT *p_rowcnt, INT *p_colcnt, INT *p_cellcnt);
/* cnt_cell_cmp.c */
API INT cnt_cell_cmp(CNT_CELL a, CNT_CELL b);
/* cnt_from_csv.c */
API INT cnt_from_csv(IO_FILE p_file, CNT_CSVOPTS p_opts, CNT p_cnt);
/* cnt_next_name.c */
API INT cnt_next_name(void **p_crs, CNT_HEAD *p_head);
/* cnt_release.c */
API INT cnt_release(CNT p_cnt);
/* cnt_get_val.c */
API INT cnt_get_val(CNT p_cnt, char *p_field, INT *p_flg, INT *p_size, void **p_val);
/* cnt_imp.c */
API INT cnt_imp(IO_FILE p_file, CNT p_cnt);
/* cnt_first_name.c */
API INT cnt_first_name(CNT p_cnt, void **p_crs, CNT_HEAD *p_head);
/* cnt_get_idx_val.c */
API INT cnt_get_idx_val(CNT p_cnt, INT p_row, char *p_field, INT *p_flg, INT *p_size, void **p_val);
/* cnt_check.c */
API INT cnt_check(CNT p_cnt);
/* cnt_exp_json.c */
API INT cnt_exp_json(IO_FILE p_file, CNT p_cnt);
/* cnt_copy.c */
API INT cnt_copy(CNT p_src, CNT p_dst);
/* cnt_create.c */
API INT cnt_create(MEM_ARENA p_arena, CNT *p_cnt);
/* cnt_copy_row.c */
API INT cnt_copy_row(CNT p_cnt, INT p_rownum, INT p_dest_rownum, CNT p_row);
/* cnt_set_val.c */
API INT cnt_set_val(CNT p_cnt, const char *p_field, INT p_flg, INT p_size, const void *p_val);
/* cnt_exp.c */
API INT cnt_exp(IO_FILE p_file, CNT p_cnt);
/* cnt_set_idx_val.c */
API INT cnt_set_idx_val(CNT p_cnt, INT p_row, const char *p_field, INT p_flg, INT p_size, const void *p_val);
/* cnt_filter.c */
API INT cnt_filter(CNT p_in, CNT p_filter, CNT p_out);
/* cnt_split_name.c */
API INT cnt_split_name(const char *p_str, char *p_name, INT *p_idx, char *p_rest);
