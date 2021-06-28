#include <stdlib.h>
#include <stdint.h>
#include <check.h>

#include "vcdiff.h"
#include "vcdiff_code_table.h"
#include "vcdiff_encoder.h"


typedef struct {
    size_t size;
    char byte;
    const char *expected_data;
    const char *expected_instr;
    size_t expected_instr_size;
} run_instr_encoding_case;


typedef struct {
    size_t size;
    const char *data;
    const char *expected_instr;
    size_t expected_instr_size;
} add_instr_encoding_case;


static const run_instr_encoding_case encode_run_instr_data[] = {
    {1, 'x', "x", "\x00\x01", 2},
    {20, 'o', "o", "\x00\x14", 2},
    {1000, 'p', "p", "\x00\x87\x68", 3}
};


static const add_instr_encoding_case encode_add_instr_data[] = {
    {1, "x", "\x02", 1},
    {4, "abcd", "\x05", 1},
    {12, "qwertyuiop[]", "\x0D", 1},
    {24, "12345678900987654321qwert", "\x01\x18", 2}
};


/**
 * Checking fixture.
 */
vcdiff_file *encoding_file;


void init_encoding_file()
{
    encoding_file = vcdiff_new_file();
}


void free_encoding_file()
{
    vcdiff_free_file(encoding_file);
}


/**
 * Check how RUN instructions are encoded.
 */
START_TEST(test_encode_run_instr)
{
    vcdiff_raw_instr *instr = vcdiff_new_instruction_stream(1);
    run_instr_encoding_case c = encode_run_instr_data[_i];

    vcdiff_encode_instr_run(instr, c.byte, c.size);

    ck_assert_uint_eq(instr->size, c.size);
    ck_assert_uint_eq(instr->data[0], c.byte);
    ck_assert_int_eq(instr->instr_type, VCDIFF_INSTR_RUN);

    vcdiff_encode_window_instructions(encoding_file->current_window,
                                      encoding_file->code_tree,
                                      instr,
                                      1);

    ck_assert_mem_eq(encoding_file->current_window->data_section,
                     c.expected_data,
                     strlen(c.expected_data));

    ck_assert_mem_eq(encoding_file->current_window->instr_section,
                     c.expected_instr,
                     c.expected_instr_size);

    vcdiff_free_instruction_stream(instr, 1);
}
END_TEST


/**
 * Check how ADD instructions are encoded.
 */
START_TEST(test_encode_add_instr)
{
    vcdiff_raw_instr *instr = vcdiff_new_instruction_stream(1);
    add_instr_encoding_case c = encode_add_instr_data[_i];

    vcdiff_encode_instr_add(instr, c.size, c.data);

    ck_assert_uint_eq(instr->size, c.size);
    ck_assert_mem_eq(instr->data, c.data, c.size);
    ck_assert_int_eq(instr->instr_type, VCDIFF_INSTR_ADD);

    vcdiff_encode_window_instructions(encoding_file->current_window,
                                      encoding_file->code_tree,
                                      instr,
                                      1);

    ck_assert_mem_eq(encoding_file->current_window->data_section,
                     c.data,
                     c.size);

    ck_assert_mem_eq(encoding_file->current_window->instr_section,
                     c.expected_instr,
                     c.expected_instr_size);

    vcdiff_free_instruction_stream(instr, 1);
}
END_TEST


Suite * vcdiff_encoder_suite()
{
    Suite *s = suite_create("vcdiff_encoder");

    TCase *tc_encode_instr = tcase_create("Encode an instruction");

    tcase_add_checked_fixture(tc_encode_instr, init_encoding_file, free_encoding_file);
    tcase_add_loop_test(tc_encode_instr, test_encode_run_instr, 0,
                        sizeof(encode_run_instr_data) / sizeof(run_instr_encoding_case));
    tcase_add_loop_test(tc_encode_instr, test_encode_add_instr, 0,
                        sizeof(encode_add_instr_data) / sizeof(add_instr_encoding_case));

    suite_add_tcase(s, tc_encode_instr);

    return s;
}
