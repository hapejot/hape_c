#include <string.h>
#include "storage.h"

struct storage storage;

void storage_init(){
    strcpy(storage.filename, "small.sqlite");
}
