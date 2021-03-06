#include <check.h>
#include "platform.h"

#include "testing.func.h"

int main( void ) {
    int number_failed;
    SRunner *sr;

    sr = srunner_create( test_suite() );
    srunner_run_all( sr, CK_VERBOSE );
    number_failed = srunner_ntests_failed( sr );
    srunner_free( sr );
    return ( number_failed == 0 ) ? 0 : 1;
}
