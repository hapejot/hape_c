#ifndef _PLATFORM_H
#define _PLATFORM_H

#ifndef API
#define API_EXT extern __declspec(dllexport)
#define API_DEF __declspec(dllexport)
#define API API_EXT
#endif


typedef unsigned long ulong_t;
typedef unsigned int uint_t;

#endif
