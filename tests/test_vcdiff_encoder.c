#include <stdlib.h>
#include <stdint.h>
#include <check.h>
#include <unistd.h>

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
 * Temp source file.
 */
char tmp_source_name[] = "/tmp/tempXXXXXX";
int tmp_source_fd;


/**
 * Temp template file.
 */
char tmp_template_name[] = "/tmp/tempXXXXXX";
int tmp_template_fd;


void init_source_template()
{
    const char source_str[] = "uvwuvwxy";
    const char template_str[] = "zuvwxwu";

    tmp_source_fd = mkstemp(tmp_source_name);
    FILE *source = fopen(tmp_source_name, "wb");
    fwrite(source_str, strlen(source_str), 1, source);
    fclose(source);

    tmp_template_fd = mkstemp(tmp_template_name);
    FILE *template = fopen(tmp_template_name, "wb");
    fwrite(template_str, strlen(template_str), 1, template);
    fclose(template);
}


void free_source_template()
{
    if (tmp_source_fd != -1) {
        close(tmp_source_fd);
    }

    if (tmp_template_fd != -1) {
        close(tmp_template_fd);
    }
}


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


/**
 * Test for vcdiff_tichy_encode_block()
 */
START_TEST(test_vcdiff_encode_block)
{
    vcdiff_io_handler *handler = vcdiff_new_io_handler(
                tmp_template_name,
                NULL,
                tmp_source_name,
                16, 4, 16);

    vcdiff_file *vcdiff = vcdiff_new_file();

    vcdiff_read_input(handler);
    vcdiff_read_source(handler);

    size_t instructions_size;
    vcdiff_raw_instr *instructions = vcdiff_tichy_encode_block(
                handler,
                vcdiff,
                &instructions_size);

    ck_assert_ptr_nonnull(instructions);
    ck_assert_uint_eq(instructions_size, 3);

    vcdiff_free_instruction_stream(instructions, instructions_size);
    vcdiff_free_file(vcdiff);
    vcdiff_free_io_handler(handler);
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

    TCase *tc_encode_block = tcase_create("Encode a block");
    tcase_add_unchecked_fixture(tc_encode_block, init_source_template, free_source_template);
    tcase_add_test(tc_encode_block, test_vcdiff_encode_block);

    suite_add_tcase(s, tc_encode_instr);
    suite_add_tcase(s, tc_encode_block);

    return s;
}
