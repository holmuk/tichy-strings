#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vcdiff_code_table.h"
#include "vcdiff_io.h"


size_t vcdiff_find_instruction_pair(
    const vcdiff_code_table_entry * const code_table,
    char *encoded_pair)
{
    const char *code_table_data = (const char *)code_table;

    size_t index = 0;
    while (index < VCDIFF_CODE_TABLE_SIZE) {
        if (memcmp(encoded_pair, code_table_data,
                   VCDIFF_CODE_INSTR_PAIR_SIZE) == 0)
        {
            break;
        }

        code_table_data += VCDIFF_CODE_INSTR_PAIR_SIZE;
        index++;
    }

    return index;
}


static size_t search_for_instruction(
    const char *data,
    size_t size,
    const char *instr)
{
    const char *ptr = data;

    size_t index = 0;
    while (index < size) {
        if (memcmp(instr, ptr, VCDIFF_CODE_INSTR_SIZE) == 0) {
            break;
        }

        ptr += VCDIFF_CODE_INSTR_SIZE;
        index++;
    }

    return index;
}



static void vcdiff_find_default_instructions(vcdiff_codetable_tree *tree)
{
    char *instr = (char *)calloc(1, VCDIFF_CODE_INSTR_SIZE);

    for (size_t i = 0; i < VCDIFF_INSTR_INVALID; i++) {
        instr[0] = i;

        size_t index_first = search_for_instruction(
            tree->first,
            tree->first_size,
            instr);

        if (index_first == tree->first_size) {
            tree->default_opcodes[i] = VCDIFF_CODE_TABLE_INVALID_OPCODE;
        } else {
            tree->default_opcodes[i] = tree->opcodes[index_first][
                tree->second_size[index_first]];
        }
    }

    free(instr);
}


vcdiff_codetable_tree * vcdiff_new_codetable_tree(vcdiff_code_table_entry *code_table)
{
    vcdiff_codetable_tree *tree = malloc(sizeof(vcdiff_codetable_tree));
    tree->first = malloc(VCDIFF_CODE_TABLE_SIZE * VCDIFF_CODE_INSTR_SIZE);
    uint16_t *noop_indices = malloc(VCDIFF_CODE_TABLE_SIZE * sizeof(uint16_t));

    size_t first_size = 0;
    vcdiff_code_table_entry *curr_instr = code_table;
    vcdiff_code_table_entry *code_table_end = code_table + VCDIFF_CODE_TABLE_SIZE;
    while (curr_instr < code_table_end) {
        if (curr_instr->instr_type2 == VCDIFF_INSTR_NOOP) {
            memcpy(tree->first + first_size * VCDIFF_CODE_INSTR_SIZE,
                   (char *)curr_instr,
                   VCDIFF_CODE_INSTR_SIZE);

            noop_indices[first_size] = curr_instr - code_table;
            first_size++;
        }

        curr_instr++;
    }

    tree->first = realloc(tree->first, first_size * VCDIFF_CODE_INSTR_SIZE);
    tree->second = malloc(first_size * sizeof(char *));
    tree->second_size = calloc(first_size, sizeof(size_t));
    tree->opcodes = malloc((first_size + 1) * sizeof(uint16_t *));

    for (size_t i = 0; i < first_size; i++) {
        tree->opcodes[i] = calloc(VCDIFF_CODE_TABLE_SIZE, sizeof(uint16_t));
        tree->second[i] = malloc(VCDIFF_CODE_TABLE_SIZE * VCDIFF_CODE_INSTR_SIZE);
    }

    curr_instr = code_table;
    while (curr_instr < code_table_end) {
        if (curr_instr->instr_type2 != VCDIFF_INSTR_NOOP) {
            size_t instr_index = search_for_instruction(
                tree->first,
                first_size,
                (char *)curr_instr);

            size_t curr_index = tree->second_size[instr_index];
            memcpy(tree->second[instr_index] + curr_index * VCDIFF_CODE_INSTR_SIZE,
                   (char *)curr_instr + VCDIFF_CODE_INSTR_SIZE,
                   VCDIFF_CODE_INSTR_SIZE);

            tree->opcodes[instr_index][curr_index] = curr_instr - code_table;
            tree->second_size[instr_index]++;
        }

        curr_instr++;
    }

    for (size_t i = 0; i < first_size; i++) {
        tree->opcodes[i][tree->second_size[i]] = noop_indices[i];
        tree->opcodes[i] = realloc(tree->opcodes[i],
                                   (tree->second_size[i] + 1) * sizeof(uint16_t *));
        tree->second[i] = realloc(tree->second[i],
                                  tree->second_size[i] * VCDIFF_CODE_INSTR_SIZE);
    }

    tree->first_size = first_size;

    free(noop_indices);

    vcdiff_find_default_instructions(tree);

    return tree;
}


void vcdiff_free_codetable_tree(vcdiff_codetable_tree *tree)
{
    for (size_t i = 0; i < tree->first_size; i++) {
        free(tree->second[i]);
        free(tree->opcodes[i]);
    }

    free(tree->first);
    free(tree->second);
    free(tree->opcodes);
    free(tree->second_size);

    free(tree);
}


uint16_t vcdiff_find_instruction(
    vcdiff_codetable_tree *tree,
    const char *encoded_first,
    const char *encoded_second,
    uint16_t *opcode)
{
    size_t index_first = search_for_instruction(
        tree->first,
        tree->first_size,
        encoded_first);

    if (index_first == tree->first_size) {
        *opcode = tree->default_opcodes[(int)encoded_first[0]];
        return 1;
    }

    size_t index_second = search_for_instruction(
        tree->second[index_first],
        tree->second_size[index_first],
        encoded_second);

    // x + NOOP if index_second == tree->second_size[index_first]
    *opcode = tree->opcodes[index_first][index_second];

    if (index_second == tree->second_size[index_first]) {
        return 1;
    }

    return 2;
}


vcdiff_raw_instr * vcdiff_new_instruction_stream(const size_t size)
{
    vcdiff_raw_instr *stream = malloc(size * sizeof(vcdiff_raw_instr));

    for (size_t i = 0; i < size; i++) {
        stream[i].data = NULL;
    }

    return stream;
}


void vcdiff_free_instruction_stream(
    vcdiff_raw_instr *stream,
    const size_t size)
{
    for (size_t i = 0; i < size; i++) {
        if (stream->data != NULL) {
            free(stream->data);
        }
    }

    free(stream);
}


void dump_instruction(
    const vcdiff_code_table_entry *table,
    uint16_t opcode)
{
    vcdiff_code_table_entry code = table[opcode];
    const char *instr = "NARC";

    fprintf(stderr, "(%c %u %u, %c %u %u)\n",
            instr[(size_t)code.instr_type],
            code.size,
            code.mode,
            instr[(size_t)code.instr_type2],
            code.size2,
            code.mode2);
}
