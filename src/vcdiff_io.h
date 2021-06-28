#ifndef VCDIFF_IO
#define VCDIFF_IO

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "error_handling.h"


#define VCDIFF_IO_DEFAULT_MAX_SOURCE_FILE_SIZE    2 * 1024 * 1024 * 1024 // 2GiB
#define VCDIFF_IO_DEFAULT_OUTPUT_BUFFER_SIZE      1024 * 1024            // 1MiB
#define VCDIFF_IO_DEFAULT_INPUT_BUFFER_SIZE       1024 * 1024            // 1MiB


typedef struct {
    const char *input_file_name;
    const char *output_file_name;
    const char *source_file_name;

    FILE *output;
    char *output_buffer;
    size_t output_buffer_size;
    char *output_ptr;
    char *output_end;

    FILE *input;
    char *input_buffer;
    size_t input_buffer_size;
    size_t input_size;

    FILE *source;
    char *source_buffer;
    size_t source_buffer_size;
    size_t source_size;

    int error_code;
} vcdiff_io_handler;


/** Get file size.
 * @param file File descriptor.
 * @return File size in bytes.
 */
size_t vcdiff_file_size(FILE *file);


/**
 * Allocate memory for a new IO handler.
 * @param input_file_name Input file name.
 * @param output_file_name Output file name.
 * @param source_file_name Source file name.
 * @param input_buffer_size Input buffer size.
 * @param output_buffer_size Output buffer size.
 * @param source_buffer_size Source buffer size.
 * @return IO handler.
 */
vcdiff_io_handler * vcdiff_new_io_handler(
    const char *input_file_name,
    const char *output_file_name,
    const char *source_file_name,
    size_t input_buffer_size,
    size_t output_buffer_size,
    size_t source_buffer_size);


/**
 * Free memory occupied by the handler.
 * @param handler IO handler.
 */
void vcdiff_free_io_handler(vcdiff_io_handler *handler);


/**
 * Read next block of data from input.
 * @param handler IO handler
 * @return Number of read bytes.
 */
size_t vcdiff_read_input(vcdiff_io_handler *handler);


/**
 * Read next block of data from source.
 * @param handler IO handler
 * @return Number of read bytes.
 */
size_t vcdiff_read_source(vcdiff_io_handler *handler);


/**
 * Flush the current output buffer; write its context into the file.
 * @param handler IO handler.
 * @return Number of bytes written.
 */
size_t vcdiff_flush_output(vcdiff_io_handler *handler);


/** Write a byte.
 * @param handler IO handler.
 * @param byte The byte to write.
 * @return Number of bytes written.
 */
size_t vcdiff_write_byte(
    vcdiff_io_handler *handler,
    const char byte);


/**
 * Write an integer in VCDIFF format.
 * @param handler IO handler.
 * @param value Value to write.
 * @return Number of bytes written.
 */
size_t vcdiff_write_integer(
    vcdiff_io_handler *handler,
    size_t value);


/**
 * Write an integer in VCDIFF format.
 * @param buffer Buffer.
 * @param value Value to write.
 * @return Number of bytes written.
 */
size_t vcdiff_write_integer_buffer(
    char *buffer,
    size_t value);


/** Write bytes to the file.
 * @param handler IO handler.
 * @param bytes Bytes to write.
 * @param bytes_len Number of bytes to write.
 * @return Number of bytes written.
 */
size_t vcdiff_write_bytes(
    vcdiff_io_handler *handler,
    const char * const bytes,
    size_t bytes_len);


#endif // VCDIFF_IO
