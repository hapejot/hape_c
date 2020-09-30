#include "platform.h"
#include <stdio.h>
#include <check.h>
#include "values.h"
#include "testing.func.h"



//  Suite
Suite *test_suite( void ) {
    Suite *s;
    s = suite_create( "Values" );
    suite_add_tcase( s, test_str_fix() );
//    suite_add_tcase( s, test_values_basic() );
//    suite_add_tcase( s, test_values_obj() );
//    suite_add_tcase( s, test_values_ivars() );
    return s;
}
