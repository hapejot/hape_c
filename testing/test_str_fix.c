#include "platform.h"
#include <check.h>
#include "str_fix.h"
#include <stdbool.h>
#include <stdio.h>
/******************************************/
START_TEST( __str_fix) {
    /* Arrange */

    /* Act     */
    uint32_t x = str_fix("string");

    /* Assert  */
    ck_assert(x > 0);
} END_TEST
/******************************************/
START_TEST( __str_fix_eq) {
    /* Arrange */

    /* Act     */
    uint32_t x = str_fix("string");
    uint32_t z = str_fix("dummy");
    uint32_t y = str_fix("string");

    /* Assert  */
    ck_assert(x == y);
    ck_assert(x != z);
} END_TEST
/******************************************/
START_TEST( __str_fix_str) {
    /* Arrange */

    /* Act     */
    const char* str = str_fix_str(1);

    /* Assert  */
    ck_assert(str != NULL);
} END_TEST
/******************************************/
START_TEST( __str_fix_max) {
    /* Arrange */
    char buffer[10];
    uint_t max = 0;
    uint_t start = 9998;
    for( uint_t i = start; ; i++){
        sprintf(buffer, "X%d", i);
        uint32_t x = str_fix(buffer);
        // fprintf(stderr,"%d %s\n", x, buffer);
        if(x == 0){
            max = i - 1;
            break;
        }
    }
    ck_assert(max > 1);
    printf("MAX: %u\n", max);
    /* Act     */
    for( uint_t i = start; i < max; i++){
        sprintf(buffer, "X%d", i);
        uint32_t x = str_fix(buffer);
        ck_assert(x);
    }

    /* Assert  */
} END_TEST


/******************************************/
START_TEST( __str_find) {
    /* Arrange */
    char buffer[10];
    for( uint_t i = 0; i< 1000; i++){
        sprintf(buffer, "X%d", i);
        str_fix(buffer);
    }

    uint32_t idx = str_find("X666");

    ck_assert(strcmp("X666", str_fix_str(idx)) == 0);
    ck_assert(str_find("XX") == 0);
    /* Assert  */
} END_TEST

TCase *test_str_fix( ) {
    (void)__str_fix_max;
    TCase *tcase = tcase_create( "str_fix" );
    tcase_add_test( tcase, __str_fix );
    tcase_add_test( tcase, __str_fix_eq );
    tcase_add_test( tcase, __str_fix_str );
    tcase_add_test( tcase, __str_find );
    tcase_set_timeout( tcase, 3600 );
    return tcase;
}
