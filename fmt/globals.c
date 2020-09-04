#include "platform.h"
#define GLOBALS
#include "fmt_int.h"

char g_fmt_flags[] = "l-+";

FMT_CVT     g_fmt_cvt[256] = {
    /* 000 - 007 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 008 - 015 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 016 - 023 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 024 - 031 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 032 - 039 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 040 - 047 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 048 - 055 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 056 - 063 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 064 - 071 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 072 - 079 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 080 - 087 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 088 - 095 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 096 - 103 */ NULL,   NULL,   NULL,   NULL,   fmt_cvt_number,   NULL,   NULL,   NULL,
    /* 104 - 111 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
    /* 112 - 119 */ NULL,   NULL,   NULL, fmt_cvt_string,   NULL,   NULL,   NULL,   NULL,
    /* 120 - 127 */ NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,
};
