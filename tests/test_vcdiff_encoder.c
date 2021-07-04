#include <stdlib.h>
#include <stdint.h>
#include <check.h>
#include <unistd.h>

#include "vcdiff.h"
#include "vcdiff_code_table.h"
#include "vcdiff_encoder.h"

#define INSTR_BUNCH_SIZE 25


typedef struct {
    vcdiff_raw_instr stream[INSTR_BUNCH_SIZE];
    size_t stream_size;

    const char *expected_data_section;
    size_t expected_data_section_size;
    const char *expected_instr_section;
    size_t expected_instr_section_size;
    const char *expected_addr_section;
    size_t expected_addr_section_size;
} instr_encoding_case;


static const instr_encoding_case encode_instr_stream_data[] = {
    {
        {
            {VCDIFF_INSTR_ADD, 0, "H", 1, 0},
            {VCDIFF_INSTR_ADD, 0, "sP", 2, 0},
            {VCDIFF_INSTR_ADD, 0, "IMZ", 3, 0},
            {VCDIFF_INSTR_ADD, 0, "QyeT", 4, 0},
            {VCDIFF_INSTR_ADD, 0, "rlDew", 5, 0},
            {VCDIFF_INSTR_ADD, 0, "VtmVIw", 6, 0},
            {VCDIFF_INSTR_ADD, 0, "DtTgnnK", 7, 0},
            {VCDIFF_INSTR_ADD, 0, "X[xSpumi", 8, 0},
            {VCDIFF_INSTR_ADD, 0, "[_EXqWTYZ", 9, 0},
            {VCDIFF_INSTR_ADD, 0, "QmafIYDXpV", 10, 0},
            {VCDIFF_INSTR_ADD, 0, "FAMZXS`HHoY", 11, 0},
            {VCDIFF_INSTR_ADD, 0, "L/ZnlA`fhREF", 12, 0},
            {VCDIFF_INSTR_ADD, 0, "TI[^lRHXNAB[f", 13, 0},
            {VCDIFF_INSTR_ADD, 0, "UyHPumTNOAYtDk", 14, 0},
            {VCDIFF_INSTR_ADD, 0, "JRuTtOhivfUBdwA", 15, 0},
            {VCDIFF_INSTR_ADD, 0, "noYypqhtWMumFOgL", 16, 0},
            {VCDIFF_INSTR_ADD, 0, "ZkqBwfyPH[t]gTldq", 17, 0}
        }, 17,
        "HsPIMZQyeTrlDewVtmVIwDtTgnnKX[xSpumi[_EXqWTYZQmafIYDXpVFAMZXS`"
        "HHoYL/ZnlA`fhREFTI[^lRHXNAB[fUyHPumTNOAYtDkJRuTtOhivfUBdwAnoYy"
        "pqhtWMumFOgLZkqBwfyPH[t]gTldq", 153,
        "\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12", 17,
        "", 0
    }
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
 * Codetable tree.
 */
vcdiff_codetable_tree *codetable_tree;


void init_codetable_tree()
{
    codetable_tree = vcdiff_new_codetable_tree(vcdiff_default_instr_table);
}


void free_codetable_tree()
{
    vcdiff_free_codetable_tree(codetable_tree);
}


/**
 * Check the validity of how instruction streams are encoded.
 */
START_TEST(test_vcdiff_encode_instr_stream)
{
    const instr_encoding_case *test_case = &encode_instr_stream_data[_i];
    vcdiff_window *window = vcdiff_new_window(128, 128, 128);

    vcdiff_encode_window_instructions(
        window,
        codetable_tree,
        (vcdiff_raw_instr *)test_case->stream,
        test_case->stream_size);

    ck_assert_uint_eq(window->data_ptr - window->data_section,
                      test_case->expected_data_section_size);
    ck_assert_mem_eq(window->data_section,
                     test_case->expected_data_section,
                     test_case->expected_data_section_size);

    ck_assert_uint_eq(window->addr_ptr - window->addr_section,
                      test_case->expected_addr_section_size);
    ck_assert_mem_eq(window->addr_section,
                     test_case->expected_addr_section,
                     test_case->expected_addr_section_size);

    ck_assert_uint_eq(window->instr_ptr - window->instr_section,
                      test_case->expected_instr_section_size);
    ck_assert_mem_eq(window->instr_section,
                     test_case->expected_instr_section,
                     test_case->expected_instr_section_size);

    vcdiff_free_window(window);
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

    TCase *tc_encode_block = tcase_create("Encode a block");
    tcase_add_unchecked_fixture(tc_encode_block, init_source_template, free_source_template);
    tcase_add_test(tc_encode_block, test_vcdiff_encode_block);

    TCase *tc_encode_stream = tcase_create("Encode instructions' stream");
    tcase_add_checked_fixture(tc_encode_stream, init_codetable_tree, free_codetable_tree);
    tcase_add_loop_test(tc_encode_stream, test_vcdiff_encode_instr_stream, 0,
                        sizeof(encode_instr_stream_data) / sizeof(instr_encoding_case));

    suite_add_tcase(s, tc_encode_instr);
    suite_add_tcase(s, tc_encode_block);
    suite_add_tcase(s, tc_encode_stream);

    return s;
}
