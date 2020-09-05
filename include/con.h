#ifndef _CON_H
#define _CON_H
#include "platform.h"
#define ESC 033 /* Escape char */

struct textinfo
{
 int curx;
 int cury; 
};

enum
{
 BLACK,
 RED,
 GREEN,
 BROWN,
 BLUE,
 MAGENTA,
 CYAN,
 LIGHTGRAY,
 DARKGRAY,
 LIGHTRED,
 LIGHTGREEN,
 YELLOW,
 LIGHTBLUE,
 LIGHTMAGENTA,
 LIGHTCYAN,
 WHITE,
};

#include "con.func.h"
#endif
