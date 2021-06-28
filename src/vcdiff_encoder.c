#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "vcdiff_code_table.h"
#include "vcdiff_encoder.h"
#include "vcdiff_cache.h"
#include "vcdiff_window.h"


void vcdiff_encode_instr_run(
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


void vcdiff_encode_instr_add(
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


void vcdiff_encode_instr_copy(
    vcdiff_raw_instr *stream,
    vcdiff_file *file,
    size_t addr,
    size_t size)
{
    assert(stream != NULL);
    assert(file != NULL);

    stream->instr_type = VCDIFF_INSTR_COPY;

    size_t here = file->current_window->here;
    size_t mode;

    size_t encoded_addr = vcdiff_cache_encode_addr(file->cache, addr, here,
                                                   &mode);

    stream->mode = (char)mode;

    if (stream->data != NULL) {
        free(stream->data);
    }

    stream->data = malloc(VCDIFF_MAX_INTEGER_SIZE);
    stream->addr_size = vcdiff_write_integer_buffer(stream->data, encoded_addr);
    stream->size = size;

    file->current_window->here += size;
}


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


static void vcdiff_write_instr(
    vcdiff_window *window,
    const vcdiff_raw_instr *instr,
    char *buffer)
{
    if (buffer[VCDIFF_POSITION_INSTR_TYPE] == VCDIFF_INSTR_NOOP) {
        return;
    }

    if (buffer[VCDIFF_POSITION_INSTR_SIZE] == 0) { // size = 0 for instr
        window->instr_ptr +=
            vcdiff_write_integer_buffer(window->instr_ptr, instr->size);
    }

    if (buffer[VCDIFF_POSITION_INSTR_TYPE] == VCDIFF_INSTR_RUN) {
        *window->data_ptr++ = instr->data[0];
        window->target_len += instr->size;
    } else if (buffer[VCDIFF_POSITION_INSTR_TYPE] == VCDIFF_INSTR_COPY) {
        memcpy(window->addr_ptr, instr->data, instr->addr_size);
        window->addr_ptr += instr->addr_size;
        window->target_len += instr->size;
    } else { // VCDIFF_INSTR_ADD
        memcpy(window->data_ptr, instr->data, instr->size);
        window->data_ptr += instr->size;
        window->target_len += instr->size;
    }
}


void vcdiff_encode_window_instructions(
    vcdiff_window *window,
    vcdiff_codetable_tree *code_table,
    vcdiff_raw_instr *stream,
    size_t stream_size)
{
    vcdiff_raw_instr *stream_end = stream + stream_size;

    vcdiff_raw_instr instr1, instr2;

    char *encoded_pair = (char *)calloc(1, VCDIFF_CODE_INSTR_PAIR_SIZE);
    uint16_t opcode;
    window->target_len = 0;

    while (stream < stream_end) {
        instr1 = *stream++;
        vcdiff_encode_raw_instr(&instr1, encoded_pair);
        if (stream != stream_end) {
            instr2 = *stream;
            vcdiff_encode_raw_instr(&instr2, encoded_pair + 3);
        } else {
            memset(encoded_pair + 3, 0, 3);
        }

        unsigned int encoded_n = vcdiff_find_instruction(
            code_table,
            encoded_pair,
            encoded_pair + VCDIFF_CODE_INSTR_SIZE,
            &opcode);

        if (opcode == VCDIFF_CODE_TABLE_INVALID_OPCODE) {
            printf("Invalid opcode!\n");
            continue;
        }

        if (encoded_n != 1) {
            stream++;
        } else {
            encoded_pair[VCDIFF_POSITION_INSTR_SIZE] = 0;
            encoded_pair[3 + VCDIFF_POSITION_INSTR_TYPE] = VCDIFF_INSTR_NOOP;
        }

        *window->instr_ptr++ = (char)opcode;
        vcdiff_write_instr(window, &instr1, encoded_pair);
        vcdiff_write_instr(window, &instr2, encoded_pair + 3);
    }

    window->data_section_size = window->data_ptr - window->data_section;
    window->instr_section_size = window->instr_ptr - window->instr_section;
    window->addr_section_size = window->addr_ptr - window->addr_section;

    free(encoded_pair);
}


vcdiff_raw_instr * vcdiff_tichy_encode_block(
    vcdiff_io_handler *handler,
    vcdiff_file *vcdiff)
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

    vcdiff_raw_instr *instructions = calloc(1,
                                            sizeof(vcdiff_raw_instr) * commands_size);
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
    }

    tichy_free_edit_commands(commands, commands_size);

    return instructions;
}
