#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "tichy_strings.h"

#include "vcdiff_code_table.h"
#include "vcdiff_encoder.h"
#include "vcdiff_cache.h"
#include "vcdiff_window.h"


#define WIN_EXPANSION_COEFF 2


/**
 * Reallocate the window's instructions' section if needed.
 * @param window Vcdiff window.
 * @param size Size of data to be written.
 */
static void alloc_window_instr_section(vcdiff_window *window, size_t size)
{
    if (window->instr_ptr + size >=
        window->instr_section + window->instr_section_size)
    {
        vcdiff_realloc_window(window,
                              window->data_section_size,
                              window->instr_section_size * WIN_EXPANSION_COEFF,
                              window->addr_section_size);
    }
}


/**
 * Reallocate the window's data section if needed.
 * @param window Vcdiff window.
 * @param size Size of data to be written.
 */
static void alloc_window_data_section(vcdiff_window *window, size_t size)
{
    if (window->data_ptr + size >=
        window->data_section + window->data_section_size)
    {
        vcdiff_realloc_window(window,
                              window->data_section_size * WIN_EXPANSION_COEFF,
                              window->instr_section_size,
                              window->addr_section_size);
    }
}


/**
 * Reallocate the window's addr section if needed.
 * @param window Vcdiff window.
 * @param size Size of data to be written.
 */
static void alloc_window_addr_section(vcdiff_window *window, size_t size)
{
    if (window->addr_ptr + size >=
        window->addr_section + window->addr_section_size)
    {
        vcdiff_realloc_window(window,
                              window->data_section_size,
                              window->instr_section_size,
                              window->addr_section_size * WIN_EXPANSION_COEFF);
    }
}


/**
 * Encode the RUN instruction.
 * @param stream Current instruction.
 * @param byte The byte to repeat.
 * @param size Size of the current RUN instruction.
 */
static void vcdiff_encode_instr_run(
    vcdiff_raw_instr *stream,
    const char byte,
    const size_t size)
{
    assert(stream != NULL);

    stream->instr_type = VCDIFF_INSTR_RUN;
    stream->size = size;
    stream->data = malloc(1);
    stream->data[0] = byte;
    stream->mode = 0;
}


/**
 * Encode the ADD instruction.
 * @param stream Current instruction.
 * @param size Size of the instruction.
 * @param data Data to add.
 */
static void vcdiff_encode_instr_add(
    vcdiff_raw_instr *stream,
    const size_t size,
    const char *data)
{
    assert(stream != NULL);
    assert(data != NULL);

    stream->instr_type = VCDIFF_INSTR_ADD;
    stream->size = size;
    stream->mode = 0;

    if (stream->data != NULL) {
        free(stream->data);
    }

    stream->data = malloc(size);
    memcpy(stream->data, data, size);
}


/**
 * Encode the COPY instruction.
 * @param stream Current instruction.
 * @param file Current Vcdiff file.
 * @param addr COPY instruction address.
 * @param size COPY instruction size.
 */
static void vcdiff_encode_instr_copy(
    vcdiff_raw_instr *stream,
    vcdiff_file *file,
    size_t addr,
    size_t size)
{
    assert(stream != NULL);
    assert(file != NULL);

    stream->instr_type = VCDIFF_INSTR_COPY;

    size_t mode;
    size_t encoded_addr = vcdiff_cache_encode_addr(file->cache, addr, &mode);

    stream->mode = (char)mode;

    if (stream->data != NULL) {
        free(stream->data);
    }

    // SAME mode, just write a byte
    if (mode >= file->cache->near_cache_size + 2) {
        stream->data = malloc(1);
        stream->data[0] = (unsigned char)encoded_addr;
        stream->addr_size = 1;
    } else {
        stream->data = malloc(VCDIFF_MAX_INTEGER_SIZE + 1);
        stream->addr_size = vcdiff_write_integer_buffer(stream->data, encoded_addr);
    }

    stream->size = size;
}


/**
 * Encode the instruction as a 3-bytes array (instr - size - mode).
 * @param instr Current instruction.
 * @param buffer Buffer to write data to.
 */
static void vcdiff_encode_raw_instr(
    const vcdiff_raw_instr *instr,
    char *buffer)
{
    buffer[VCDIFF_POSITION_INSTR_TYPE] = instr->instr_type;
    if (instr->size > 255) {
        buffer[VCDIFF_POSITION_INSTR_SIZE] = 0;
    } else {
        buffer[VCDIFF_POSITION_INSTR_SIZE] = (char)instr->size;
    }
    buffer[VCDIFF_POSITION_INSTR_MODE] = instr->mode;
}


/**
 * Write the instruction to the window.
 * @param window Current window.
 * @param instr Current instruction.
 * @param buffer Buffer with encoded instruction
 * (see vcdiff_encode_raw_instr() )
 */
static void vcdiff_write_instr(
    vcdiff_window *window,
    const vcdiff_raw_instr *instr,
    char *buffer)
{
    if (buffer[VCDIFF_POSITION_INSTR_TYPE] == VCDIFF_INSTR_NOOP) {
        return;
    }

    if (buffer[VCDIFF_POSITION_INSTR_SIZE] == 0) { // size = 0 for instr
        alloc_window_instr_section(window, VCDIFF_MAX_INTEGER_SIZE + 1);
        size_t instr_size = vcdiff_write_integer_buffer(window->instr_ptr, instr->size);
        window->instr_ptr += instr_size;
    }

    if (buffer[VCDIFF_POSITION_INSTR_TYPE] == VCDIFF_INSTR_COPY) {
        alloc_window_addr_section(window, instr->addr_size);
        memcpy(window->addr_ptr, instr->data, instr->addr_size);
        window->addr_ptr += instr->addr_size;
    } else if (buffer[VCDIFF_POSITION_INSTR_TYPE] == VCDIFF_INSTR_ADD) {
        alloc_window_data_section(window, instr->size);
        memcpy(window->data_ptr, instr->data, instr->size);
        window->data_ptr += instr->size;
    } else {  /* VCDIFF_INSTR_RUN */
        alloc_window_data_section(window, 1);
        *window->data_ptr++ = instr->data[0];
    }

    window->target_len += instr->size;
}

void vcdiff_encode_window_instructions(
    vcdiff_window *window,
    vcdiff_codetable_tree *code_table,
    vcdiff_raw_instr *stream,
    size_t stream_size)
{
    vcdiff_raw_instr *stream_end = stream + stream_size;

    vcdiff_raw_instr *instr1;
    vcdiff_raw_instr *instr2;

    char *encoded_pair = calloc(1, VCDIFF_CODE_INSTR_PAIR_SIZE);
    uint16_t opcode;
    window->target_len = 0;

    bool encoded_two = true;

    while (stream < stream_end) {
        if (!encoded_two) {
            // Use old instruction.
            instr1 = instr2;
        } else {
            instr1 = stream;
        }

        stream++;
        vcdiff_encode_raw_instr(instr1, encoded_pair);
        if (stream != stream_end) {
            instr2 = stream;
            vcdiff_encode_raw_instr(instr2, encoded_pair + 3);
        } else {
            memset(encoded_pair + 3, 0, 3);
        }

        unsigned int encoded_n = vcdiff_find_instruction(
            code_table,
            encoded_pair,
            encoded_pair + VCDIFF_CODE_INSTR_SIZE,
            &opcode);

        if (opcode == VCDIFF_CODE_TABLE_INVALID_OPCODE) {
            print_error("Invalid opcode: %hx\n", opcode);
            continue;
        }

        encoded_two = (encoded_n == 2);
        if (!encoded_two) {
            (encoded_pair + 3)[VCDIFF_POSITION_INSTR_TYPE] = VCDIFF_INSTR_NOOP;
            if (encoded_n == 0) {
                encoded_pair[VCDIFF_POSITION_INSTR_SIZE] = 0;
            }
        }

        alloc_window_instr_section(window, 1);
        *window->instr_ptr++ = (char)opcode;

        vcdiff_write_instr(window, instr1, encoded_pair);
        vcdiff_write_instr(window, instr2, encoded_pair + 3);
    }

    free(encoded_pair);
}


vcdiff_raw_instr * vcdiff_tichy_encode_block(
    vcdiff_io_handler *handler,
    vcdiff_file *vcdiff,
    size_t *instructions_size)
{
    size_t commands_size;
    edit_command *commands = tichy_translate(
        handler->source_buffer,
        handler->source_buffer_size,
        handler->input_buffer,
        handler->input_buffer_size,
        &commands_size);

    edit_command *curr_command = commands;
    edit_command *commands_end = commands + commands_size;

    vcdiff_raw_instr *instructions = vcdiff_new_instruction_stream(commands_size);
    vcdiff_raw_instr *curr_instruction = instructions;

    while (curr_command != commands_end) {
        if (curr_command->type == EDIT_COMMAND_ADD) {
            /** TODO: Convert repeated bytes to RUN */
            vcdiff_encode_instr_add(curr_instruction,
                                    curr_command->length,
                                    curr_command->add_str);
        } else {  // EDIT_COMMAND_MOVE
            vcdiff_encode_instr_copy(curr_instruction,
                                     vcdiff,
                                     curr_command->move_pos,
                                     curr_command->length);
        }

        curr_instruction++;
        curr_command++;
    }

    tichy_free_edit_commands(commands, commands_size);
    *instructions_size = commands_size;

    return instructions;
}


void tichy_encode_vcdiff(
    const char *source_filename,
    const char *template_filename,
    const char *output_filename,
    size_t source_buffer_size,
    size_t template_buffer_size,
    size_t output_buffer_size)
{
    vcdiff_io_handler *io_handler = vcdiff_new_io_handler(
        template_filename,
        output_filename,
        source_filename,
        template_buffer_size,
        output_buffer_size,
        source_buffer_size);

    vcdiff_file *vcdiff = vcdiff_new_file();
    vcdiff_write_file_header(io_handler);

    /// TODO: Source size == 0

    size_t curr_source_pos = 0;
    while (vcdiff_read_input(io_handler) != 0) {
        size_t source_block_size = vcdiff_read_source(io_handler);
        if (source_block_size == 0) {
            // Repeat the source
            fseek(io_handler->source, 0, SEEK_SET);
            curr_source_pos = 0;
            source_block_size = vcdiff_read_source(io_handler);
        }

        vcdiff->current_window->source_segment_pos = curr_source_pos;
        vcdiff->current_window->source_segment_size = source_block_size;
        curr_source_pos += source_block_size;

        /// TODO: Optimize (use the same memory for all instruction streams)
        size_t instructions_size;
        vcdiff_raw_instr *instructions =
            vcdiff_tichy_encode_block(io_handler, vcdiff, &instructions_size);

        vcdiff_encode_window_instructions(
            vcdiff->current_window,
            vcdiff->code_tree,
            instructions,
            instructions_size);

        vcdiff_write_window(io_handler, vcdiff->current_window);

        if (io_handler->error_code != VCDIFF_OK) {
            print_error("Unexpected error during IO operations.");
            return;
        }

        vcdiff_reset_window_pointers(vcdiff->current_window);

        vcdiff_free_cache(vcdiff->cache);
        vcdiff->cache = vcdiff_new_cache(
            VCDIFF_CACHE_DEFAULT_NEAR_SIZE,
            VCDIFF_CACHE_DEFAULT_SAME_SIZE);

        vcdiff_free_instruction_stream(instructions, instructions_size);
    }

    vcdiff_flush_output(io_handler);
    if (io_handler->error_code != VCDIFF_OK) {
        print_error("Unexpected error during IO operations.");
        return;
    }

    vcdiff_free_io_handler(io_handler);
    vcdiff_free_file(vcdiff);
}
