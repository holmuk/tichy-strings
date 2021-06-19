#include <check.h>

#include "tichy_strings.h"

#define BLOCK_MOVES_TEST_CAP 50


typedef struct {
    char *source;
    size_t source_size;
    char *template;
    size_t template_size;
    size_t expected_block_moves_count;
    block_move expected_block_moves[BLOCK_MOVES_TEST_CAP];
} set_test_case;


static const set_test_case set_test_cases[] = {
    {
        "\xAA", 1,
        "\xAA", 1,
        1, {
            {0, 0, 1}
        }
    },
    {
        "uvwuvwxy", 8,
        "zuvwxwu", 7,
        2, {
            {3, 1, 4},
            {2, 5, 2}
        }
    },
    {
        "\xAA\xBB\x00\x00\x40", 5,
        "\xBB\x00\x00\x00\x00\x40", 6,
        2, {
            {1, 0, 3},
            {2, 3, 3}
        }
    }
};



/**
 * Test tichy_minimal_covering_naive() function
 */
START_TEST(test_minimal_covering_naive)
{
    size_t set_size;
    set_test_case curr_case = set_test_cases[_i];
    block_move *moves_set = tichy_minimal_covering_naive(
                curr_case.source,
                curr_case.source_size,
                curr_case.template,
                curr_case.template_size,
                &set_size);

    ck_assert_ptr_nonnull(moves_set);
    ck_assert_uint_eq(set_size, curr_case.expected_block_moves_count);
    ck_assert_mem_eq(moves_set, curr_case.expected_block_moves,
                     set_size * sizeof(block_move));

    free(moves_set);
}
END_TEST


Suite *tichy_prefix_suite()
{
    Suite *s = suite_create("tichy_prefix");

    TCase *tc_naive = tcase_create("Naive");
    tcase_add_loop_test(tc_naive, test_minimal_covering_naive, 0,
                        sizeof(set_test_cases)/sizeof(set_test_case));

    // TCase *tc_kmp = tcase_create("KMP");
    // tcase_add_test(tc_kmp, test_minimal_covering_kmp);

    suite_add_tcase(s, tc_naive);
    // suite_add_tcase(s, tc_kmp);

    return s;
}
