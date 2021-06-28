#include <stdlib.h>
#include <stdint.h>
#include <check.h>

#include "vcdiff.h"
#include "vcdiff_cache.h"
#include "vcdiff_window.h"


/**
  * Valgrind test for file memory allocation.
  */
START_TEST(test_vcdiff_file_allocation_alloc)
{
    vcdiff_file *file = vcdiff_new_file();

    ck_assert_ptr_nonnull(file->cache);
    ck_assert_ptr_nonnull(file->current_window);
    ck_assert_ptr_nonnull(file->code_tree);

    vcdiff_free_file(file);
}


/**
  * Valgrind test for file memory allocation.
  */
START_TEST(test_vcdiff_file_allocation_realloc)
{
    vcdiff_file *file = vcdiff_new_file();

    vcdiff_realloc_cache(file->cache, 10, 20);
    vcdiff_realloc_window(file->current_window, 128, 50, 200);

    ck_assert_ptr_nonnull(file->cache);
    ck_assert_ptr_nonnull(file->current_window);
    ck_assert_ptr_nonnull(file->code_tree);

    vcdiff_free_file(file);
}


Suite *vcdiff_file_allocation_suite()
{
    Suite *s = suite_create("vcdiff_file_allocation");

    TCase *tc_memory = tcase_create("Memory allocation");

    tcase_add_test(tc_memory, test_vcdiff_file_allocation_alloc);
    tcase_add_test(tc_memory, test_vcdiff_file_allocation_realloc);

    suite_add_tcase(s, tc_memory);

    return s;
}
