#include <stdlib.h>
#include <stdint.h>
#include <check.h>

#include "vcdiff_window.h"


typedef struct {
    size_t data_size;
    size_t instr_size;
    size_t addr_size;

    size_t new_data_size;
    size_t new_instr_size;
    size_t new_addr_size;
} window_sizes_triple;


static const window_sizes_triple window_sizes[] = {
    { 1, 128, 1, 128, 64, 100},
    {128, 128, 128, 256, 256, 256},
    {65, 12, 945, 10, 10, 10},
    {125, 5, 1245, 100, 34, 75}
};


/**
 * Valgrind test for window's memory allocation.
 */
START_TEST(test_vcdiff_window_alloc_zero)
{
    vcdiff_window *win = vcdiff_new_window(0, 0, 0);

    ck_assert_ptr_null(win->addr_ptr);
    ck_assert_ptr_null(win->data_ptr);
    ck_assert_ptr_null(win->instr_ptr);

    ck_assert_uint_eq(win->addr_section_size, 0);
    ck_assert_uint_eq(win->data_section_size, 0);
    ck_assert_uint_eq(win->instr_section_size, 0);

    vcdiff_free_window(win);
}
END_TEST


/**
 * Valgrind test for window's memory reallocation.
 */
START_TEST(test_vcdiff_window_realloc_zero)
{
    vcdiff_window *win = vcdiff_new_window(128, 128, 128);
    vcdiff_realloc_window(win, 0, 0, 0);

    ck_assert_ptr_null(win->addr_ptr);
    ck_assert_ptr_null(win->data_ptr);
    ck_assert_ptr_null(win->instr_ptr);

    ck_assert_uint_eq(win->addr_section_size, 0);
    ck_assert_uint_eq(win->data_section_size, 0);
    ck_assert_uint_eq(win->instr_section_size, 0);

    vcdiff_free_window(win);
}
END_TEST


/**
 * Valgrind test for window's memory allocation.
 */
START_TEST(test_vcdiff_window_alloc)
{
    size_t data_size = window_sizes[_i].data_size;
    size_t instr_size = window_sizes[_i].instr_size;
    size_t addr_size = window_sizes[_i].addr_size;

    vcdiff_window *win = vcdiff_new_window(data_size, instr_size, addr_size);

    ck_assert_ptr_nonnull(win->addr_ptr);
    ck_assert_ptr_nonnull(win->data_ptr);
    ck_assert_ptr_nonnull(win->instr_ptr);

    ck_assert_uint_eq(win->addr_section_size, addr_size);
    ck_assert_uint_eq(win->data_section_size, data_size);
    ck_assert_uint_eq(win->instr_section_size, instr_size);

    vcdiff_free_window(win);
}
END_TEST


/**
 * Valgrind test for window's memory reallocation.
 */
START_TEST(test_vcdiff_window_realloc)
{
    size_t data_size = window_sizes[_i].data_size;
    size_t instr_size = window_sizes[_i].instr_size;
    size_t addr_size = window_sizes[_i].addr_size;

    vcdiff_window *win = vcdiff_new_window(data_size, instr_size, addr_size);

    data_size = window_sizes[_i].new_data_size;
    instr_size = window_sizes[_i].new_instr_size;
    addr_size = window_sizes[_i].new_addr_size;

    vcdiff_realloc_window(win, data_size, instr_size, addr_size);

    ck_assert_ptr_nonnull(win->addr_ptr);
    ck_assert_ptr_nonnull(win->data_ptr);
    ck_assert_ptr_nonnull(win->instr_ptr);

    ck_assert_uint_eq(win->addr_section_size, addr_size);
    ck_assert_uint_eq(win->data_section_size, data_size);
    ck_assert_uint_eq(win->instr_section_size, instr_size);

    vcdiff_free_window(win);
}
END_TEST


Suite * vcdiff_window_suite()
{
    Suite *s = suite_create("vcdiff_window");

    TCase *tc_memory = tcase_create("Memory allocation");

    tcase_add_test(tc_memory, test_vcdiff_window_alloc_zero);
    tcase_add_test(tc_memory, test_vcdiff_window_realloc_zero);

    tcase_add_loop_test(tc_memory, test_vcdiff_window_alloc, 0,
                        sizeof(window_sizes) / sizeof(window_sizes_triple));
    tcase_add_loop_test(tc_memory, test_vcdiff_window_realloc, 0,
                        sizeof(window_sizes) / sizeof(window_sizes_triple));

    suite_add_tcase(s, tc_memory);

    return s;
}
