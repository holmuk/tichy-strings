#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <check.h>
#include <unistd.h>


#include "vcdiff.h"
#include "vcdiff_io.h"


static const char * const expected_text =
    "Lorem ipsum dolor sit amet, consectetur adipisicing elit, "
    "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";


static const char * const integers_file_bytes =
    "\x89\x2a\x80\x2a\x93\x90\x42\x3e\x24\x45\xab\xaa\x00\x23";


static const size_t integers_file_size = 14;


static const size_t expected_integers[] = {
    1194, 42, 313410, 62, 36, 69, 709888
};


static const char * const expected_streams[] = {
    "\x89\x2A",
    "\x2A",
    "\x93\x90\x42",
    "\x3E",
    "\x24",
    "\x45",
    "\xAB\xAA\x00"
};


/**
 * Temporary file name.
 */
char tmp_file_name[] = "/tmp/fileXXXXXX";


/**
 * Temporary file descriptor.
 */
int tmp_file_fd;


void init_tmp_file_text()
{
    tmp_file_fd = mkstemp(tmp_file_name);
    FILE *file = fopen(tmp_file_name, "wb");
    fwrite(expected_text, strlen(expected_text) + 1, 1, file);
    fclose(file);
}


void init_tmp_file_integers()
{
    tmp_file_fd = mkstemp(tmp_file_name);
    FILE *file = fopen(tmp_file_name, "wb");
    fwrite(integers_file_bytes, integers_file_size, 1, file);
    fclose(file);
}


void free_tmp_name()
{
    close(tmp_file_fd);
}


/**
 * Valgrind test for IO handler memory allocation.
 */
START_TEST(test_io_handler_life)
{
    vcdiff_io_handler *handler = vcdiff_new_io_handler(
        NULL,
        NULL,
        tmp_file_name,
        128, 128, 1024);

    ck_assert_ptr_nonnull(handler);

    vcdiff_free_io_handler(handler);
}
END_TEST


/**
 * Test for vcdiff_write_integer()
 */
START_TEST(test_write_integer_stream)
{
    size_t buffer_size = 16;
    vcdiff_io_handler *handler = vcdiff_new_io_handler(
        NULL,
        tmp_file_name,
        NULL,
        16, buffer_size, 16);

    vcdiff_write_integer(handler, expected_integers[_i]);

    ck_assert_mem_eq(handler->output_buffer, expected_streams[_i],
                     strlen(expected_streams[_i]));

    vcdiff_free_io_handler(handler);
}
END_TEST


/**
 * Test for reading input file.
 */
START_TEST(test_read_input)
{
    size_t buffer_size = 4;
    vcdiff_io_handler *handler = vcdiff_new_io_handler(
        tmp_file_name,
        NULL,
        NULL,
        buffer_size, 16, 16);

    ck_assert_ptr_nonnull(handler);
    ck_assert_ptr_nonnull(handler->input_buffer);
    ck_assert_uint_eq(handler->input_buffer_size, buffer_size);
    ck_assert_int_eq(handler->input_size, strlen(expected_text) + 1);

    char *buffer = malloc(strlen(expected_text) + 1);
    char *buffer_ptr = buffer;

    while (vcdiff_read_input(handler) != 0) {
        memcpy(buffer_ptr, handler->input_buffer, handler->input_buffer_size);
        buffer_ptr += handler->input_buffer_size;
    }

    ck_assert_mem_eq(buffer, expected_text, strlen(expected_text) + 1);

    free(buffer);
    vcdiff_free_io_handler(handler);
}
END_TEST


/**
 * Test for reading source file.
 */
START_TEST(test_read_source)
{
    size_t buffer_size = 4;
    vcdiff_io_handler *handler = vcdiff_new_io_handler(
        NULL,
        NULL,
        tmp_file_name,
        16, 16, buffer_size);

    ck_assert_ptr_nonnull(handler);
    ck_assert_ptr_nonnull(handler->source_buffer);
    ck_assert_uint_eq(handler->source_buffer_size, buffer_size);
    ck_assert_int_eq(handler->source_size, strlen(expected_text) + 1);

    char *buffer = malloc(strlen(expected_text) + 1);
    char *buffer_ptr = buffer;

    while (vcdiff_read_source(handler) != 0) {
        memcpy(buffer_ptr, handler->source_buffer, handler->source_buffer_size);
        buffer_ptr += handler->source_buffer_size;
    }

    ck_assert_mem_eq(buffer, expected_text, strlen(expected_text) + 1);

    free(buffer);
    vcdiff_free_io_handler(handler);
}
END_TEST


Suite * vcdiff_io_suite()
{
    Suite *s = suite_create("vcdiff_io");

    TCase *tc_memory = tcase_create("Memory");
    tcase_add_unchecked_fixture(tc_memory, init_tmp_file_text, free_tmp_name);
    tcase_add_test(tc_memory, test_io_handler_life);

    TCase *tc_integers = tcase_create("Integers");
    tcase_add_loop_test(tc_integers, test_write_integer_stream, 0,
                        sizeof(expected_integers) / sizeof(size_t));

    TCase *tc_reading = tcase_create("Reading");
    tcase_add_unchecked_fixture(tc_reading, init_tmp_file_text, free_tmp_name);
    tcase_add_test(tc_reading, test_read_source);
    tcase_add_test(tc_reading, test_read_input);

    suite_add_tcase(s, tc_memory);
    suite_add_tcase(s, tc_integers);
    suite_add_tcase(s, tc_reading);

    return s;
}
