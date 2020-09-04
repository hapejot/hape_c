#ifndef ERR_H
#define ERR_H
#include "platform.h"
typedef struct {
    char szFile[80];
    long nLine;
    char szSysComp[20];
    char szSysCall[40];
    long nSysErr;
    char szSysMsg[500];
} ERR;

extern ERR g_err;

typedef struct err_Log_entry {
    char typ;
    char pkg[10];
    char id[4];
    char msg[200];
} ERR_LOG_ENTRY;

#define LOG(...) do{ sprintf(_log.msg, __VA_ARGS__);\
    _log.typ = 'I';\
    err_log_add(&_log);\
}while(false)

#include "err.func.h"
#endif
