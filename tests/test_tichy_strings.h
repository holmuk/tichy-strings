#ifndef TEST_TICHY_STRINGS_H
#define TEST_TICHY_STRINGS_H

#include <check.h>

Suite * tichy_prefix_suite();
Suite * tichy_translate_suite();

Suite * vcdiff_cache_suite();
Suite * vcdiff_code_table_suite();
Suite * vcdiff_encoder_suite();
Suite * vcdiff_file_allocation_suite();
Suite * vcdiff_io_suite();
Suite * vcdiff_window_suite();

#endif // TEST_TICHY_STRINGS_H
