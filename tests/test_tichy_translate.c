#include <check.h>

#include "tichy_strings.h"

#define BLOCK_MOVES_TEST_CAP 50


typedef struct {
    char *source;
    size_t source_size;
    char *template;
    size_t template_size;
    size_t expected_instructions_count;
    edit_command expected_instructions[BLOCK_MOVES_TEST_CAP];
} set_test_case;


static const set_test_case set_test_cases[] = {
    {
        "\xAA", 1,
        "\xAA", 1,
        1, {
            {EDIT_COMMAND_MOVE, 1, { .move_pos = 0 }}
        }
    },
    {
        "uvwuvwxy", 8,
        "zuvwxwu", 7,
        3, {
            {EDIT_COMMAND_ADD, 1, { .add_str = "z" }},
            {EDIT_COMMAND_MOVE, 4, { .move_pos = 3 }},
            {EDIT_COMMAND_MOVE, 2, { .move_pos = 2 }}
        }
    },
    {
        "how to differentiate between differences?", 40,
        "betweeniff", 10,
        2, {
            {EDIT_COMMAND_MOVE, 7, { .move_pos = 21 }},
            {EDIT_COMMAND_MOVE, 3, { .move_pos = 8 }}
        }
    }
};



/**
 * Test tichy_translate() function
 */
START_TEST(test_translate)
{
    size_t commands_length;
    set_test_case curr_case = set_test_cases[_i];
    edit_command* commands = tichy_translate(
                curr_case.source,
                curr_case.source_size,
                curr_case.template,
                curr_case.template_size,
                &commands_length);

    ck_assert_ptr_nonnull(commands);
    ck_assert_uint_eq(commands_length, curr_case.expected_instructions_count);

    edit_command *expected = curr_case.expected_instructions;
    for (size_t i = 0; i < commands_length; i++) {
        ck_assert_uint_eq(commands[i].type, expected[i].type);
        ck_assert_uint_eq(commands[i].length, expected[i].length);
        if (expected[i].type == EDIT_COMMAND_ADD) {
            ck_assert_mem_eq(commands[i].add_str, expected[i].add_str,
                             expected[i].length);
        } else {
            ck_assert_uint_eq(commands[i].move_pos, expected[i].move_pos);
        }
    }

    tichy_free_edit_commands(commands, commands_length);
}
END_TEST


Suite *tichy_translate_suite()
{
    Suite *s = suite_create("tichy_translate");

    TCase *tc_translate = tcase_create("Translate");
    tcase_add_loop_test(tc_translate, test_translate, 0,
                        sizeof(set_test_cases)/sizeof(set_test_case));

    suite_add_tcase(s, tc_translate);

    return s;
}
