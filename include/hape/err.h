#ifndef ERR_H
#define ERR_H
#include <setjmp.h>
#include <hape/platform.h>

typedef struct _err_def ERR_DEF;
typedef struct _err_msg ERR_MSG;

struct _err_def {
    char pkg[10];
    int num;
    const char *msg; // message template language dependent
};

struct _err_msg {
    ERR_DEF *err;
    char typ;
    char arg1[50];
    char arg2[50];
    char arg3[50];
    char arg4[50];
};

/*
typedef struct {
    char szFile[80];
    long nLine;
    char szSysComp[20];
    char szSysCall[40];
    long nSysErr;
    char szSysMsg[500];
} ERR;
*/

typedef struct err_Log_entry {
    char typ;
    char pkg[10];
    char id[4];
    char msg[200];
} ERR_LOG_ENTRY;

typedef struct _err_frame ERR_FRAME;

struct _err_frame {
    ERR_FRAME *prev;
    jmp_buf env;
    const char *file;
    int line;
    ERR_LOG_ENTRY err;
};

// extern ERR g_err;
extern ERR_FRAME *g_err_stack;

enum {
    err_entered = 0,
    err_raised,
    err_handled,
    err_finalized
};

#define ERR_LAST    (g_err_stack->err)

#define TRY do {                                                \
/* fprintf(stderr, "TRY\n"); */                                 \
    volatile int    err_flag;                                   \
    ERR_FRAME       err_frame;                                  \
    err_frame.prev = g_err_stack;                               \
    g_err_stack = &err_frame;                                   \
    err_flag = setjmp(err_frame.env);                           \
/*  fprintf(stderr, "TRY1 %d\n", err_flag); */                  \
    if (err_flag == err_entered) {

#define CATCH(_pkg, _id)                                        \
/* fprintf(stderr, "CATCH1\n"); */                              \
        if(err_flag == err_entered)                             \
            g_err_stack = g_err_stack->prev;                    \
        } else if (err_flag == err_raised ){                    \
/* fprintf(stderr, "CATCH2 %s\n", g_err_stack->err.pkg); */     \
/* fprintf(stderr, "CATCH2 %s\n", g_err_stack->err.id); */      \
        err_flag = err_handled;


#define END_TRY                                                 \
    if(err_flag == err_entered)                                 \
        g_err_stack = g_err_stack->prev;                        \
    }                                                           \
/* fprintf(stderr, "ENDTRY1\n"); */                             \
    if (err_flag == err_raised){                                \
/* fprintf(stderr, "ENDTRY2\n"); */                             \
        g_err_stack = g_err_stack->prev;                        \
        if(g_err_stack)                                         \
        longjmp(g_err_stack->env, err_raised);                  \
    }                                                           \
/* fprintf(stderr, "DONE\n"); */                                \
} while(0);

#define RAISE(e) do{ longjmp(g_err_stack->env, err_raised); } while(0);


#define LOG(...) do{ sprintf(_log.msg, __VA_ARGS__);\
    _log.typ = 'I';\
    err_log_add(&_log);\
}while(false)

#include "err.func.h"
#endif
