/* fmt_cvt_number.c */
API void out_leader(IO_FILE p_io, int w, char leader);
API void out_digit(IO_FILE p_io, int n, int w, char leader);
API INT fmt_cvt_number(IO_FILE p_io, va_list *p_ap, char *p_flags, INT p_width, INT p_scale);
/* fmt_cvt_string.c */
API INT fmt_cvt_string(IO_FILE p_io, va_list *p_ap, char *p_flags, INT p_width, INT p_scale);
/* fmt_vfmt.c */
API INT fmt_vfmt(IO_FILE p_io, char *p_fmt, va_list *p_ap);
/* fmt_format.c */
API INT fmt_format(IO_FILE p_io, char *p_fmt, ...);
/* globals.c */
