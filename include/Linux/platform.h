#ifndef _PLATFORM_H
#define _PLATFORM_H

#define API
#define API_DEF
#define API_EXT

#define IN
#define OUT
#define INOUT

typedef unsigned long ulong_t;
typedef unsigned int uint_t;

#define PLATFORM_IS_LINUX   1
#define PLATFORM_IS_UNIX    1
#define PLATFORM_IS_WIN32   0
#define PLATFORM_IS_SOLARIS 0

#define O_BINARY            0

typedef long long INT64;
typedef unsigned long long UINT64;
typedef long INT32;
typedef unsigned long UINT32;
typedef short INT16;
typedef unsigned short UINT16;
typedef char INT8;
typedef unsigned char UINT8;
typedef unsigned char CHAR;
typedef void VOID;

typedef INT32 INT;
typedef UINT32 UINT;

#define SCCSID(file,id)
// static char sccsid##file[] = "@(#) (c) PJA " BUILDNO " "  id 
#define OK          0
#define RC_OK       0
#define ERROR       -1
#define RC_ERROR    -1
#define ENDSTR      '\0'

#define IN
#define OUT
#define INOUT

#endif
