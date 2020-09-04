#ifndef _FMT_INT_H
#define _FMT_INT_H
#include "fmt.h"
#include <stdarg.h>
#include <limits.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
typedef INT (*FMT_CVT)(IO_FILE, va_list*, char*, INT, INT);
extern char g_fmt_flags[];
extern FMT_CVT     g_fmt_cvt[256];
#include "fmt.func.h"
#endif
