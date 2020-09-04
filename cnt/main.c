#include "platform.h"
#include "cnt_int.h"
#include <string.h>
#include <stdio.h>

int main()
{
    CNT         cnt             = NULL;
    INT         status          = 0;
    void      * val             = NULL;
    INT         len             = 0;
    INT         flg             = 0;
    MEM_ARENA   arena           = NULL;
    IO_FILE     f               = NULL;
    IO_FILE     std_out         = NULL;
    CNT_HEAD    head            = NULL;

    arena = mem_arena_new();

    status = io_connect_file( &std_out, NULL, "w" );

    status = cnt_create(arena, &cnt);
    /*                                 / user flags        */
    /*                                 |   / strlen        */
    /*                                 |   |  / value      */
    status = cnt_set_val(cnt, "a",     0, -1, "value1");
    status = cnt_set_val(cnt, "b",     0, -1, "value2");
    status = cnt_set_val(cnt, "test",  0, -1, "value3.0");

    status = cnt_dump(cnt);
    status = cnt_exp(std_out, cnt);
    /*                            / row number                      */
    /*                            |  / field name                   */
    /*                            |  |        / user flags          */
    /*                            |  |        |   / strlen          */
    /*                            |  |        |   |  / value        */
    status = cnt_set_idx_val(cnt, 1, "test",  0, -1, "value3.1");

    status = cnt_dump(cnt);
    status = cnt_exp(std_out, cnt);

    /*                            / row number                      */
    /*                            |  / field name                   */
    /*                            |  |        / user flags          */
    /*                            |  |        |     / strlen        */
    /*                            |  |        |     |     / value   */
    status = cnt_get_idx_val(cnt, 0, "test", &flg, &len, &val);
    assert(strcmp(val, "value3.0") == 0);
    status = cnt_get_idx_val(cnt, 0, "b",    &flg, &len, &val);
    assert(strcmp(val, "value2") == 0);

    /*                        / field name                   */
    /*                        |        / user flags          */
    /*                        |        |     / strlen        */
    /*                        |        |     |     / value   */
    status = cnt_get_val(cnt, "a",    &flg, &len, &val);
    assert(strcmp(val, "value1") == 0);

    status = io_connect_file( &f, "cnt.txt", "r");
    assert( status == RC_OK );
    status = cnt_imp(f, cnt);
    io_close(&f);
    cnt_dump(cnt);

    cnt_first_name(cnt, &val, &head);
    printf("%s[%ld]\n", head->name, head->width);
    while( cnt_next_name(&val, &head) == RC_OK )
    {
        printf("%s[%ld]\n", head->name, head->width);
    }
    mem_arena_dispose( &arena );


    arena = mem_arena_new();
    status = cnt_create(arena, &cnt);
    status = cnt_exp(std_out, cnt);
    cnt_split( "/a/b/c", 6, '/',    "t[0].arg[%d].val", cnt );
    cnt_split( "/a", 2, '/',        "t[1].arg[%d].val", cnt );
    cnt_split( "/a/", 3, '/',       "t[2].arg[%d].val", cnt );
    cnt_split( "/", 1, '/',         "t[3].arg[%d].val", cnt );
    cnt_split( "", 1, '/',          "t[4].arg[%d].val", cnt );
    status = cnt_exp(std_out, cnt);
    mem_arena_dispose( &arena );

    return status;
}
