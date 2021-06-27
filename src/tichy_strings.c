#include "tichy_strings.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define INITIAL_CAP 5


static void place_command_add(
        edit_command* current_command,
        const char* data,
        size_t data_size)
{
    current_command->type = EDIT_COMMAND_ADD;
    current_command->length = data_size;
    current_command->add_str = (char*)malloc(data_size);
    memcpy(current_command->add_str, data, data_size);
}


static void place_command_move(
        edit_command* current_command,
        size_t length,
        size_t pos)
{
    current_command->type = EDIT_COMMAND_MOVE;
    current_command->length = length;
    current_command->move_pos = pos;
}


edit_command* tichy_translate(
        const char *source,
        size_t source_size,
        const char *template,
        size_t template_size,
        size_t *resulting_commands_size)
{
    size_t minimal_covering_size;
    block_move* minimal_covering = tichy_minimal_covering_kmp(
                source, source_size,
                template, template_size,
                &minimal_covering_size);

    size_t commands_cap = INITIAL_CAP;
    edit_command* commands =
            (edit_command*)malloc(sizeof(edit_command) * commands_cap);
    edit_command* current_command = commands;

    size_t template_index = 0;
    block_move* current_move = minimal_covering;
    block_move* minimal_covering_end = minimal_covering + minimal_covering_size;
    while (template_index < template_size
           && current_move != minimal_covering_end) {
        size_t current_move_t = current_move->template_pos;

        if (current_move_t > template_index) {
            place_command_add(current_command++,
                              template + template_index,
                              current_move_t - template_index);
            template_index = current_move_t;
        }

        if (current_move_t == template_index) {
            place_command_move(current_command++,
                               current_move->length,
                               current_move->source_pos);
            template_index += current_move->length;
        } else if (current_move_t < template_index &&
                   current_move_t + current_move->length > template_index) {
            size_t length = current_move_t +
                    current_move->length - template_index;
            place_command_move(current_command++,
                               length,
                               current_move->source_pos + template_index - current_move_t);
            template_index += length;
        }

        if (current_command >= commands + commands_cap) {
            ptrdiff_t offset = current_command - commands;
            commands_cap *= 2;
            commands = realloc(commands, commands_cap);
            current_command = commands + offset;
        }

        current_move++;
    }

    if (template_index + 1 < template_size) {
        place_command_add(current_command++,
                          template + template_index,
                          template_size - template_index);
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
