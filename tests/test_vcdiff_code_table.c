#include <stdlib.h>
#include <stdint.h>
#include <check.h>

#include "vcdiff_code_table.h"


typedef struct {
    vcdiff_code_table_entry instr;
    uint16_t expected_opcode;
    unsigned int expected_n_encoded;
} test_read_instruction_pair;


static const test_read_instruction_pair test_find_instruction_expected_pairs[] = {
    { {VCDIFF_INSTR_NOOP, 0,    0,  VCDIFF_INSTR_NOOP,   0,   0},  VCDIFF_CODE_TABLE_INVALID_OPCODE, 1},
    { {VCDIFF_INSTR_RUN,  10,   0,  VCDIFF_INSTR_NOOP,   0,   0},  0,   1 },
    { {VCDIFF_INSTR_ADD,  11,   0,  VCDIFF_INSTR_NOOP,   0,   0},  12,  1 },
    { {VCDIFF_INSTR_COPY, 11,   0,  VCDIFF_INSTR_NOOP,   0,   0},  27,  1 },
    { {VCDIFF_INSTR_COPY, 11,   6,  VCDIFF_INSTR_NOOP,   0,   0},  123, 1 },
    { {VCDIFF_INSTR_COPY,  0,   1,  VCDIFF_INSTR_COPY,   1,   0},  35,  1 },
    { {VCDIFF_INSTR_COPY, 17,   2,  VCDIFF_INSTR_RUN,   10,   0},  65,  1 },
    { {VCDIFF_INSTR_ADD,   3,   0,  VCDIFF_INSTR_COPY,   5,   5}, 230,  2 },
    { {VCDIFF_INSTR_COPY,  4,   0,  VCDIFF_INSTR_ADD,    1,   0}, 247,  2 },
    { {VCDIFF_INSTR_ADD,   4,   0,  VCDIFF_INSTR_COPY,   6,   3}, 210,  2 }
};


static const uint16_t default_instructions_expected_indices[4] = {
    VCDIFF_CODE_TABLE_INVALID_OPCODE,
    1,
    19,
    0
};


/**
 * Test for "default" instructions.
 */
START_TEST(test_default_instruction_indices)
{
    vcdiff_codetable_tree *tree = vcdiff_new_codetable_tree(vcdiff_default_instr_table);

    ck_assert_mem_eq(tree->default_opcodes,
                     default_instructions_expected_indices,
                     VCDIFF_INSTR_INVALID);

    vcdiff_free_codetable_tree(tree);
}
END_TEST


/**
 * Valgrind test for codetree memory allocation.
 */
START_TEST(test_codetree_allocation)
{
    vcdiff_codetable_tree *tree = vcdiff_new_codetable_tree(vcdiff_default_instr_table);

    ck_assert_ptr_nonnull(tree->first);
    ck_assert_ptr_nonnull(tree->second);
    ck_assert_ptr_nonnull(tree->opcodes);
    ck_assert_ptr_nonnull(tree->second_size);

    ck_assert_uint_eq(tree->first_size, 163);

    vcdiff_free_codetable_tree(tree);
}
END_TEST


START_TEST(test_find_instruction)
{
    vcdiff_codetable_tree *tree = vcdiff_new_codetable_tree(vcdiff_default_instr_table);

    uint16_t opcode;
    char *instr = (char*)&test_find_instruction_expected_pairs[_i].instr;

    unsigned int encoded_n = vcdiff_find_instruction(
                tree,
                instr,
                instr + VCDIFF_CODE_INSTR_SIZE,
                &opcode);

    vcdiff_free_codetable_tree(tree);

    ck_assert_uint_eq(opcode,
                      test_find_instruction_expected_pairs[_i].expected_opcode);
    ck_assert_uint_eq(encoded_n,
                      test_find_instruction_expected_pairs[_i].expected_n_encoded);
}
END_TEST


Suite *vcdiff_code_table_suite()
{
    Suite *s = suite_create("vcdiff_code_table");

    TCase *tc_instr = tcase_create("Instructions");
    tcase_add_loop_test(tc_instr, test_find_instruction, 0,
                        sizeof(test_find_instruction_expected_pairs) /
                        sizeof(test_read_instruction_pair));
    tcase_add_test(tc_instr, test_default_instruction_indices);

    TCase *tc_memory = tcase_create("Memory");
    tcase_add_test(tc_memory, test_codetree_allocation);

    suite_add_tcase(s, tc_instr);
    suite_add_tcase(s, tc_memory);

    return s;
}
