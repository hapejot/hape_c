#ifndef _STORAGE_H
#define _STORAGE_H

extern struct storage {
    char filename[100];
} storage;

void storage_init();

#endif
