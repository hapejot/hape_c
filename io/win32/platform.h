#ifndef _PLATFORM_H
#define _PLATFORM_H
#include <windows.h>
#include <stdlib.h>
#include <assert.h>

#define PLATFORM_IS_LINUX   0
#define PLATFORM_IS_UNIX    0
#define PLATFORM_IS_WIN32   1
#define PLATFORM_IS_SOLARIS 0

typedef INT32               INT;


#define SCCSID(file,id) static char sccsid##file[] = ("@(#) (c) PJA " BUILDNO " "  id )
#define RC_OK       0
#define RC_ERROR    -1
#define ENDSTR      '\0'

#define IN
#define OUT
#define INOUT

#define S_IRWXU     0600
#define S_IRWXG     0060
#define S_IRWXO     0006


#endif
