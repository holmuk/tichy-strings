#include "tichy_strings.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>


#define INITIAL_SET_SIZE 5

block_move* tichy_minimal_covering_naive(
        const char *source,
        size_t source_size,
        const char *template,
        size_t template_size,
        size_t *resulting_set_size)
{
    size_t covering_set_cap = INITIAL_SET_SIZE;
    block_move *covering_set =
            (block_move*)calloc(1, sizeof(block_move) * covering_set_cap);
    size_t covering_set_size = 0;

    const char *source_end = source + source_size;
    const char *template_end = template + template_size;
    const char *template_ptr = template;

    while (template_ptr < template_end) {
        block_move current_move = {0, 0, 0};

        const char *source_ptr = source;
        while (source_ptr < source_end) {
            ptrdiff_t block_len = 0;

            const char *ptr = template_ptr;
            while (ptr != template_end && source_ptr != source_end &&
                   *source_ptr == *ptr) {
                source_ptr++;
                ptr++;
            }

            if (ptr == template_ptr) {
                source_ptr++;
            }

            block_len = ptr - template_ptr;
            if (block_len > 0 && (size_t)block_len > current_move.length) {
                current_move.length = block_len;
                current_move.source_pos = source_ptr - source - block_len;
                current_move.template_pos = template_ptr - template;
            }
        }

        if (current_move.length != 0) {
            covering_set[covering_set_size++] = current_move;
            if (covering_set_size >= covering_set_cap) {
                covering_set_cap = covering_set_cap * 2;
                covering_set = (block_move*)realloc(
                            covering_set,
                            covering_set_cap * sizeof(block_move));
            }

            template_ptr += current_move.length;
        } else {
            template_ptr++;
        }
    }

    *resulting_set_size = covering_set_size;
    return covering_set;
}
