/* io_connect_fd.c */
API INT io_connect_fd(IO_FILE *p_file, INT p_fd);
/* io_seek.c */
API INT io_seek(IO_FILE p_io, UINT64 p_pos);
/* io_read.c */
API INT io_read(IO_FILE p_io, void *p_data, INT p_len, INT *p_n);
/* io_attr_get.c */
API INT io_attr_get(IO_FILE p_f, char *p_an, char *p_av, INT p_avsize);
/* io_std_read.c */
API INT io_std_read(IO_FILE p_io, void *p_data, INT p_size, INT *p_len);
/* io_buf_write.c */
API INT io_buf_write(IO_FILE p_file, void *p_data, INT p_size, INT *p_count);
/* io_std_close.c */
API INT io_std_close(IO_FILE p_io);
/* io_buf_read.c */
API INT io_buf_read(void);
/* io_gets.c */
API INT io_gets(IO_FILE p_file, INT8 *p_buf, INT p_bufsize, INT *p_bytes);
/* io_std_seek.c */
API INT io_std_seek(IO_FILE p_io, UINT64 p_pos);
/* io_std_write.c */
API INT io_std_write(IO_FILE p_io, void *p_data, INT p_size, INT *p_len);
/* io_connect_buf.c */
API INT io_connect_buf(IO_FILE *p_file, IO_BUFFER p_buf);
/* io_puts.c */
API INT io_puts(IO_FILE p_io, char *p_chars);
/* io_write.c */
API INT io_write(IO_FILE p_io, void *p_data, INT p_len, INT *p_n);
/* io_ungetc.c */
API INT io_ungetc(IO_FILE p_io, INT p_char);
/* io_close.c */
API INT io_close(IO_FILE *p_io);
/* io_putc.c */
API INT io_putc(IO_FILE p_io, INT p_char);
/* io_getc.c */
API INT io_getc(IO_FILE p_io);
/* io_buf_expand.c */
API INT io_buf_expand(IO_BUFFER p_buf, INT p_size);
/* io_connect_file.c */
API INT io_connect_file(IO_FILE *p_file, char *p_filename, char *p_mode);
/* io_buf_new.c */
API INT io_buf_new(IO_BUFFER *p_buf);
/* globals.c */
