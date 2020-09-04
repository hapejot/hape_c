#include "platform.h"
#include "err.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    fopen("abcd", "r");
    err_set_syserr("fopen", __FILE__, __LINE__); err_print();

    err_set_err(0x1234567, "standard fehler","err", __FILE__, __LINE__); err_print();

    return 0;
}
