#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vcdiff.h"
#include "vcdiff_io.h"

#include "error_handling.h"


/**
 * Open current file.
 * @param file_name File name.
 * @param flags fopen flags.
 * @param[out] file_size File size if the file has been opened with 'r' flag.
 * @return File descriptor.
 */
static FILE * open_file(
    const char *file_name,
    const char *flags,
    size_t *file_size)
{
    if (file_name == NULL) {
        return NULL;
    }

    FILE *file = fopen(file_name, flags);

    if (!file || ferror(file)) {
        print_error(FOPEN_ERROR_MSG);
        return NULL;
    }

    if (strchr(flags, 'r') && file_size != NULL) {
        *file_size = vcdiff_file_size(file);
    }

    return file;
}


/**
 * Read the next data block from file.
 * @param file File descriptor.
 * @param buffer Buffer to write the block into.
 * @param buffer_size Data block size.
 * @return Number of read bytes.
 */
static size_t read_data_block(
    FILE *file,
    char *buffer,
    size_t buffer_size)
{
    if (feof(file)) {
        return 0;
    }

    size_t bytes_read = fread(buffer, 1, buffer_size, file);
    if (ferror(file)) {
        print_error(FREAD_ERROR_MSG);
        return 0;
    }

    return bytes_read;
}


size_t vcdiff_read_input(vcdiff_io_handler *handler)
{
    size_t block_size = read_data_block(
        handler->input,
        handler->input_buffer,
        handler->input_buffer_size);

    handler->input_buffer_size = block_size;
    return block_size;
}


size_t vcdiff_read_source(vcdiff_io_handler *handler)
{
    size_t block_size = read_data_block(
        handler->source,
        handler->source_buffer,
        handler->source_buffer_size);

    handler->source_buffer_size = block_size;
    return block_size;
}


/**
 * Write current output buffer's data to output.
 * @param handler IO handler.
 * @return Number of written bytes.
 */
static size_t vcdiff_write_output(vcdiff_io_handler *handler)
{
    size_t bytes_written = fwrite(handler->output_buffer,
                                  1, handler->output_buffer_size, handler->output);

    if (ferror(handler->output)) {
        print_error(FWRITE_ERROR_MSG);
        return 0;
    }

    return bytes_written;
}


/**
 * Open input file.
 * @param file_name Input file name.
 * @param file_handler Input file handler (if any).
 * @param buffer File buffer.
 * @param buffer_size Buffer size.
 * @param[out] file_size Calculated file size.
 * @return File descriptor.
 */
static FILE * open_input_file(
    const char *file_name,
    FILE *file_handler,
    char **buffer,
    size_t *buffer_size,
    size_t *file_size)
{
    FILE *source;

    if (file_name == NULL || strlen(file_name) == 0) {
        if (file_handler == NULL) {
            print_warning("Empty input or source!");
            *buffer = NULL;
            return NULL;
        }

        source = file_handler;
        *file_size = 0;
    } else {
        source = open_file(file_name, "rb", file_size);
    }

    if (source == NULL) {
        *file_size = 0;
        *buffer = NULL;
        return source;
    }

    if (*file_size > 0 && *file_size < *buffer_size) {
        *buffer_size = *file_size;
    }

    *buffer = malloc(*buffer_size);
    return source;
}


/**
 * Open output file and allocate buffers.
 * @param handler IO handler.
 * @param file_name File name.
 * @param file_handler File handler that is used if file_name is empty.
 */
static void vcdiff_open_output(
    vcdiff_io_handler *handler,
    const char *file_name,
    FILE *file_handler)
{
    if (file_name == NULL || strlen(file_name) == 0) {
        if (file_handler == NULL) {
            print_warning("Empty output!");
            handler->output_buffer = NULL;
            return;
        }

        handler->output = file_handler;
    } else {
        handler->output = open_file(file_name, "wb", NULL);
    }

    if (handler->output == NULL) {
        handler->error_code = VCDIFF_IO_INVALID_OUTPUT_FILE;
        handler->output_buffer = NULL;
        return;
    }

    handler->output_buffer = malloc(handler->output_buffer_size);
    handler->output_end = handler->output_buffer + handler->output_buffer_size;
    handler->output_ptr = handler->output_buffer;
}


/**
 * Open input file and allocate buffers.
 * @param handler IO handler.
 * @param file_name File name.
 * @param file_handler File handler that is used if file_name is empty.
 */
static void vcdiff_open_input(
    vcdiff_io_handler *handler,
    const char *file_name,
    FILE *file_handler)
{
    handler->input = open_input_file(
        file_name,
        file_handler,
        &handler->input_buffer,
        &handler->input_buffer_size,
        &handler->input_size);

    if (handler->input == NULL &&
            file_name != NULL && strlen(file_name) != 0) {
        handler->error_code = VCDIFF_IO_INVALID_INPUT_FILE;
    }
}


/**
 * Open source file and allocate buffers.
 * @param handler IO handler.
 * @param file_name File name.
 * @param file_handler File handler that is used if file_name is empty.
 */
static void vcdiff_open_source(
    vcdiff_io_handler *handler,
    const char *file_name,
    FILE *file_handler)
{
    handler->source = open_input_file(
        file_name,
        file_handler,
        &handler->source_buffer,
        &handler->source_buffer_size,
        &handler->source_size);

    if (handler->source == NULL &&
            file_name != NULL && strlen(file_name) != 0) {
        handler->error_code = VCDIFF_IO_INVALID_SOURCE_FILE;
    }
}


vcdiff_io_handler * vcdiff_new_io_handler(
    const char *input_file_name,
    const char *output_file_name,
    const char *source_file_name,
    size_t input_buffer_size,
    size_t output_buffer_size,
    size_t source_buffer_size)
{
    vcdiff_io_handler *handler = malloc(sizeof(vcdiff_io_handler));

    if (handler == NULL) {
        return NULL;
    }

    handler->error_code = VCDIFF_OK;

    handler->input_file_name = input_file_name;
    handler->output_file_name = output_file_name;
    handler->source_file_name = source_file_name;

    // Instantinate buffers' sizes
    handler->input_buffer_size = input_buffer_size;
    handler->output_buffer_size = output_buffer_size;
    handler->source_buffer_size = source_buffer_size;

    // Open source file
    vcdiff_open_source(handler, source_file_name, NULL);
    if (handler->error_code != VCDIFF_OK) {
        print_error("Invalid source file: %s", source_file_name);
    }
    handler->error_code = VCDIFF_OK;

    // Open input file
    vcdiff_open_input(handler, input_file_name, stdin);
    if (handler->error_code != VCDIFF_OK) {
        print_error("Invalid input file: %s", input_file_name);
    }
    handler->error_code = VCDIFF_OK;

    // Open output file
    vcdiff_open_output(handler, output_file_name, stdout);
    if (handler->error_code != VCDIFF_OK) {
        print_error("Invalid output file: %s", output_file_name);
    }
    handler->error_code = VCDIFF_OK;

    return handler;
}


void vcdiff_free_io_handler(vcdiff_io_handler *handler)
{
    if (handler == NULL) {
        return;
    }

    if (handler->input != stdin && handler->input != NULL) {
        fclose(handler->input);
    }

    if (handler->output != stdout && handler->output != NULL) {
        fclose(handler->output);
    }

    if (handler->source != NULL) {
        fclose(handler->source);
    }

    if (handler->input_buffer != NULL) {
        free(handler->input_buffer);
    }

    if (handler->output_buffer != NULL) {
        free(handler->output_buffer);
    }

    if (handler->source_buffer != NULL) {
        free(handler->source_buffer);
    }

    free(handler);
}


size_t vcdiff_write_integer_buffer(
    char *buffer,
    size_t value)
{
    if (value > VCDIFF_MAX_INTEGER_32) {
        return 0;
    }

    char *buffer_int = malloc(VCDIFF_MAX_INTEGER_SIZE + 1);
    char *buffer_ptr = buffer_int + VCDIFF_MAX_INTEGER_SIZE;

    size_t tmp = value;

    *buffer_ptr = tmp % 128;
    buffer_ptr--;
    tmp = tmp / 128;

    while (buffer_ptr != buffer_int && tmp > 0) {
        *buffer_ptr = (tmp % 128) | 0x80;
        buffer_ptr--;
        tmp = tmp / 128;
    }
    buffer_ptr++;

    size_t size = (char)(buffer_int + VCDIFF_MAX_INTEGER_SIZE + 1 - buffer_ptr);
    memcpy(buffer, buffer_ptr, size);
    free(buffer_int);

    return size;
}


size_t vcdiff_write_integer(
    vcdiff_io_handler *handler,
    size_t value)
{
    char *buffer = malloc(VCDIFF_MAX_INTEGER_SIZE + 1);
    size_t size = vcdiff_write_integer_buffer(buffer, value);
    size = vcdiff_write_bytes(handler, buffer, size);

    free(buffer);

    return size;
}


size_t vcdiff_write_byte(
    vcdiff_io_handler *handler,
    const char byte)
{
    if (handler->output_ptr != handler->output_end) {
        *handler->output_ptr++ = byte;
    } else {
        vcdiff_write_output(handler);

        handler->output_ptr = handler->output_buffer;
        *handler->output_ptr = byte;
        handler->output_ptr++;
    }

    return 1;
}


size_t vcdiff_write_bytes(
    vcdiff_io_handler *handler,
    const char *bytes,
    const size_t bytes_len)
{
    size_t remnant_bytes = (size_t)(handler->output_end - handler->output_ptr);

    if (bytes_len < remnant_bytes) {
        memcpy(handler->output_ptr, bytes, bytes_len);
        handler->output_ptr += bytes_len;
    } else {
        memcpy(handler->output_ptr, bytes, remnant_bytes);

        size_t pos_to_move = remnant_bytes;
        size_t bytes_to_move = bytes_len - remnant_bytes;

        while (bytes_to_move > handler->output_buffer_size) {
            vcdiff_write_output(handler);

            memcpy(handler->output_buffer, bytes + pos_to_move, handler->output_buffer_size);
            pos_to_move += handler->output_buffer_size;
            bytes_to_move -= handler->output_buffer_size;
        }

        vcdiff_write_output(handler);
        memcpy(handler->output_buffer, bytes + pos_to_move, bytes_to_move);
        handler->output_ptr = handler->output_buffer + bytes_to_move;
    }

    return bytes_len;
}


size_t vcdiff_flush_output(vcdiff_io_handler *handler)
{
    size_t flush_size = (size_t)(handler->output_ptr - handler->output_buffer);
    size_t bytes_written = fwrite(handler->output_buffer,
                                  1, flush_size, handler->output);

    if (ferror(handler->output)) {
        print_error(FWRITE_ERROR_MSG);
        return 0;
    }

    return bytes_written;
}


size_t vcdiff_file_size(FILE *file)
{
    size_t old_pos = ftell(file);

    if (fseek(file, 0, SEEK_END) != 0) {
        print_error(FILESIZE_ERROR_MSG);
        return 0;
    }

    long file_size = ftell(file);
    if (file_size == -1) {
        print_error(FILESIZE_ERROR_MSG);
        return 0;
    }

    if (fseek(file, old_pos, SEEK_SET) != 0) {
        print_error(FILESIZE_ERROR_MSG);
        return 0;
    }

    return file_size;
}
