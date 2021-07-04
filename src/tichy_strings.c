#include "tichy_strings.h"

#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define INITIAL_CAP 5


static void place_command_add(
    edit_command *current_command,
    const char *data,
    size_t data_size)
{
    current_command->type = EDIT_COMMAND_ADD;
    current_command->length = data_size;
    current_command->add_str = malloc(data_size);
    memcpy(current_command->add_str, data, data_size);
}


static void place_command_move(
    edit_command *current_command,
    size_t length,
    size_t pos)
{
    current_command->type = EDIT_COMMAND_MOVE;
    current_command->length = length;
    current_command->move_pos = pos;
}


/**
 * Copy block of commands of size @p size.
 * @param dst Destination.
 * @param src Source.
 * @param size Size of the block.
 */
static void copy_commands(
        edit_command *dst,
        const edit_command *src,
        size_t size)
{
    const edit_command *src_ptr = src;
    edit_command *dst_ptr = dst;

    while (src_ptr != src + size) {
        dst_ptr->type = src_ptr->type;
        dst_ptr->length = src_ptr->length;

        if (src_ptr->type == EDIT_COMMAND_ADD) {
            dst_ptr->add_str = malloc(src_ptr->length);
            memcpy(dst_ptr->add_str, src_ptr->add_str, src_ptr->length);
        } else {
            dst_ptr->move_pos = src_ptr->move_pos;
        }

        src_ptr++;
        dst_ptr++;
    }
}


/**
 * Allocate additional memory for command stream if necessary.
 * @param commands Stream of commands that should be reallocated.
 * @param[in, out] commands_cap Command stream capacity.
 * @param current_command Current command pointer.
 * @return New current command pointer.
 */
static edit_command * alloc_commands(
        edit_command **commands,
        size_t *commands_cap,
        edit_command *current_command)
{
    assert(current_command >= *commands);

    ptrdiff_t offset = current_command - *commands;

    if ((size_t)offset >= *commands_cap) {
        size_t old_cap = *commands_cap;
        *commands_cap *= 2;
        edit_command *new_commands = calloc(1, *commands_cap * sizeof(edit_command));
        copy_commands(new_commands, *commands, old_cap);

        tichy_free_edit_commands(*commands, old_cap);

        *commands = new_commands;
        current_command = *commands + offset;
    }

    return current_command;
}


edit_command * tichy_translate(
    const char *source,
    size_t source_size,
    const char *template,
    size_t template_size,
    size_t *resulting_commands_size)
{
    size_t minimal_covering_size;
    block_move *minimal_covering = tichy_minimal_covering_naive(
        source, source_size,
        template, template_size,
        &minimal_covering_size);

    if (minimal_covering == NULL) {
        return NULL;
    }

    size_t commands_cap = INITIAL_CAP;
    edit_command *commands = calloc(1, commands_cap * sizeof(edit_command));
    edit_command *current_command = commands;

    size_t template_index = 0;
    block_move *current_move = minimal_covering;
    block_move *minimal_covering_end = minimal_covering + minimal_covering_size;
    while (template_index < template_size
           && current_move != minimal_covering_end)
    {
        size_t current_move_t = current_move->template_pos;

        if (current_move_t > template_index) {
            place_command_add(current_command++,
                              template + template_index,
                              current_move_t - template_index);

            current_command = alloc_commands(&commands,
                                             &commands_cap,
                                             current_command);

            template_index = current_move_t;
        }

        if (current_move_t == template_index) {
            place_command_move(current_command++,
                               current_move->length,
                               current_move->source_pos);
            template_index += current_move->length;
        } else if (current_move_t < template_index &&
                   current_move_t + current_move->length > template_index)
        {
            size_t length = current_move_t +
                            current_move->length - template_index;
            place_command_move(current_command++,
                               length,
                               current_move->source_pos + template_index - current_move_t);

            template_index += length;
        }

        current_command = alloc_commands(&commands,
                                         &commands_cap,
                                         current_command);

        current_move++;
    }

    if (template_index < template_size) {
        place_command_add(current_command++,
                          template + template_index,
                          template_size - template_index);

        current_command = alloc_commands(&commands,
                                         &commands_cap,
                                         current_command);
    }

    free(minimal_covering);
    *resulting_commands_size = current_command - commands;
    return commands;
}


void tichy_free_edit_commands(edit_command *commands, size_t length)
{
    for (size_t i = 0; i < length; i++) {
        if (commands[i].type == EDIT_COMMAND_ADD) {
            free(commands[i].add_str);
        }
    }

    free(commands);
}
