#include <check.h>

#include "tichy_strings.h"
#include "test_tichy_strings.h"


int main(void)
{
    SRunner *sr = srunner_create(tichy_prefix_suite());
    srunner_add_suite(sr, tichy_translate_suite());

    srunner_run_all(sr, CK_NORMAL);

    int number_failed = srunner_ntests_failed(sr);

    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
