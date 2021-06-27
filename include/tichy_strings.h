#ifndef TICHY_STRINGS_H
#define TICHY_STRINGS_H

#include <stdlib.h>


typedef struct {
    size_t source_pos;
    size_t template_pos;
    size_t length;
} block_move;


typedef struct {
    enum {
        EDIT_COMMAND_MOVE,
        EDIT_COMMAND_ADD
    } type;
    size_t length;

    union {
        size_t move_pos;
        char *add_str;
    };
} edit_command;


/**
 * Search for a minimal covering set of block moves (D) for the template (T)
 * with respect to the source (S), so that every symbol of T that appears in S
 * is included in exactly one block move containing in D.
 * @param source Source data.
 * @param source_size Source data size.
 * @param template Template data.
 * @param template_size Template data size.
 * @param[out] resulting_set_size Resulting covering set size.
 * @return covering_set Pointer to covering set. The caller is responsible for
 * freeing the allocated memory.
 */
block_move* tichy_minimal_covering_naive(
        const char *source,
        size_t source_size,
        const char *template,
        size_t template_size,
        size_t *resulting_set_size);


/**
 * Search for a minimal covering set of block moves (D) for the template (T)
 * with respect to the source (S), so that every symbol of T that appears in S
 * is included in exactly one block move containing in D.
 * @param source Source data.
 * @param source_size Source data size.
 * @param template Template data.
 * @param template_size Template data size.
 * @param[out] resulting_set_size Resulting covering set size.
 * @return covering_set Pointer to covering set. The caller is responsible for
 * freeing the allocated memory.
 */
block_move* tichy_minimal_covering_kmp(
        const char *source,
        size_t source_size,
        const char *template,
        size_t template_size,
        size_t *resulting_set_size);


edit_command* tichy_translate(
        const char *source,
        size_t source_size,
        const char *template,
        size_t template_size,
        size_t *resulting_commands_size);


void tichy_free_edit_commands(edit_command *commands, size_t length);

#endif // TICHY_STRINGS_H
