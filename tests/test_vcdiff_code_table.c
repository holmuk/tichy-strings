#include <stdlib.h>
#include <stdint.h>
#include <check.h>

#include "vcdiff_code_table.h"


typedef struct {
    vcdiff_code_table_entry instr;
    uint16_t expected_opcode;
    unsigned int expected_n_encoded;
} test_read_instruction_pair;


static const test_read_instruction_pair test_find_instruction_expected_pairs[] = {
    { {VCDIFF_INSTR_RUN, 0, 0, VCDIFF_INSTR_NOOP, 0, 0}, 0, 1 },
    { {VCDIFF_INSTR_ADD, 0, 0, VCDIFF_INSTR_NOOP, 0, 0}, 1, 1 },
    { {VCDIFF_INSTR_ADD, 1, 0, VCDIFF_INSTR_NOOP, 0, 0}, 2, 1 },
    { {VCDIFF_INSTR_ADD, 2, 0, VCDIFF_INSTR_NOOP, 0, 0}, 3, 1 },
    { {VCDIFF_INSTR_ADD, 3, 0, VCDIFF_INSTR_NOOP, 0, 0}, 4, 1 },
    { {VCDIFF_INSTR_ADD, 4, 0, VCDIFF_INSTR_NOOP, 0, 0}, 5, 1 },
    { {VCDIFF_INSTR_ADD, 5, 0, VCDIFF_INSTR_NOOP, 0, 0}, 6, 1 },
    { {VCDIFF_INSTR_ADD, 6, 0, VCDIFF_INSTR_NOOP, 0, 0}, 7, 1 },
    { {VCDIFF_INSTR_ADD, 7, 0, VCDIFF_INSTR_NOOP, 0, 0}, 8, 1 },
    { {VCDIFF_INSTR_ADD, 8, 0, VCDIFF_INSTR_NOOP, 0, 0}, 9, 1 },
    { {VCDIFF_INSTR_ADD, 9, 0, VCDIFF_INSTR_NOOP, 0, 0}, 10, 1 },
    { {VCDIFF_INSTR_ADD, 10, 0, VCDIFF_INSTR_NOOP, 0, 0}, 11, 1 },
    { {VCDIFF_INSTR_ADD, 11, 0, VCDIFF_INSTR_NOOP, 0, 0}, 12, 1 },
    { {VCDIFF_INSTR_ADD, 12, 0, VCDIFF_INSTR_NOOP, 0, 0}, 13, 1 },
    { {VCDIFF_INSTR_ADD, 13, 0, VCDIFF_INSTR_NOOP, 0, 0}, 14, 1 },
    { {VCDIFF_INSTR_ADD, 14, 0, VCDIFF_INSTR_NOOP, 0, 0}, 15, 1 },
    { {VCDIFF_INSTR_ADD, 15, 0, VCDIFF_INSTR_NOOP, 0, 0}, 16, 1 },
    { {VCDIFF_INSTR_ADD, 16, 0, VCDIFF_INSTR_NOOP, 0, 0}, 17, 1 },
    { {VCDIFF_INSTR_ADD, 17, 0, VCDIFF_INSTR_NOOP, 0, 0}, 18, 1 },
    { {VCDIFF_INSTR_COPY, 0, 0, VCDIFF_INSTR_NOOP, 0, 0}, 19, 1 },
    { {VCDIFF_INSTR_COPY, 4, 0, VCDIFF_INSTR_NOOP, 0, 0}, 20, 1 },
    { {VCDIFF_INSTR_COPY, 5, 0, VCDIFF_INSTR_NOOP, 0, 0}, 21, 1 },
    { {VCDIFF_INSTR_COPY, 6, 0, VCDIFF_INSTR_NOOP, 0, 0}, 22, 1 },
    { {VCDIFF_INSTR_COPY, 7, 0, VCDIFF_INSTR_NOOP, 0, 0}, 23, 1 },
    { {VCDIFF_INSTR_COPY, 8, 0, VCDIFF_INSTR_NOOP, 0, 0}, 24, 1 },
    { {VCDIFF_INSTR_COPY, 9, 0, VCDIFF_INSTR_NOOP, 0, 0}, 25, 1 },
    { {VCDIFF_INSTR_COPY, 10, 0, VCDIFF_INSTR_NOOP, 0, 0}, 26, 1 },
    { {VCDIFF_INSTR_COPY, 11, 0, VCDIFF_INSTR_NOOP, 0, 0}, 27, 1 },
    { {VCDIFF_INSTR_COPY, 12, 0, VCDIFF_INSTR_NOOP, 0, 0}, 28, 1 },
    { {VCDIFF_INSTR_COPY, 13, 0, VCDIFF_INSTR_NOOP, 0, 0}, 29, 1 },
    { {VCDIFF_INSTR_COPY, 14, 0, VCDIFF_INSTR_NOOP, 0, 0}, 30, 1 },
    { {VCDIFF_INSTR_COPY, 15, 0, VCDIFF_INSTR_NOOP, 0, 0}, 31, 1 },
    { {VCDIFF_INSTR_COPY, 16, 0, VCDIFF_INSTR_NOOP, 0, 0}, 32, 1 },
    { {VCDIFF_INSTR_COPY, 17, 0, VCDIFF_INSTR_NOOP, 0, 0}, 33, 1 },
    { {VCDIFF_INSTR_COPY, 18, 0, VCDIFF_INSTR_NOOP, 0, 0}, 34, 1 },
    { {VCDIFF_INSTR_COPY, 0, 1, VCDIFF_INSTR_NOOP, 0, 0}, 35, 1 },
    { {VCDIFF_INSTR_COPY, 4, 1, VCDIFF_INSTR_NOOP, 0, 0}, 36, 1 },
    { {VCDIFF_INSTR_COPY, 5, 1, VCDIFF_INSTR_NOOP, 0, 0}, 37, 1 },
    { {VCDIFF_INSTR_COPY, 6, 1, VCDIFF_INSTR_NOOP, 0, 0}, 38, 1 },
    { {VCDIFF_INSTR_COPY, 7, 1, VCDIFF_INSTR_NOOP, 0, 0}, 39, 1 },
    { {VCDIFF_INSTR_COPY, 8, 1, VCDIFF_INSTR_NOOP, 0, 0}, 40, 1 },
    { {VCDIFF_INSTR_COPY, 9, 1, VCDIFF_INSTR_NOOP, 0, 0}, 41, 1 },
    { {VCDIFF_INSTR_COPY, 10, 1, VCDIFF_INSTR_NOOP, 0, 0}, 42, 1 },
    { {VCDIFF_INSTR_COPY, 11, 1, VCDIFF_INSTR_NOOP, 0, 0}, 43, 1 },
    { {VCDIFF_INSTR_COPY, 12, 1, VCDIFF_INSTR_NOOP, 0, 0}, 44, 1 },
    { {VCDIFF_INSTR_COPY, 13, 1, VCDIFF_INSTR_NOOP, 0, 0}, 45, 1 },
    { {VCDIFF_INSTR_COPY, 14, 1, VCDIFF_INSTR_NOOP, 0, 0}, 46, 1 },
    { {VCDIFF_INSTR_COPY, 15, 1, VCDIFF_INSTR_NOOP, 0, 0}, 47, 1 },
    { {VCDIFF_INSTR_COPY, 16, 1, VCDIFF_INSTR_NOOP, 0, 0}, 48, 1 },
    { {VCDIFF_INSTR_COPY, 17, 1, VCDIFF_INSTR_NOOP, 0, 0}, 49, 1 },
    { {VCDIFF_INSTR_COPY, 18, 1, VCDIFF_INSTR_NOOP, 0, 0}, 50, 1 },
    { {VCDIFF_INSTR_COPY, 0, 2, VCDIFF_INSTR_NOOP, 0, 0}, 51, 1 },
    { {VCDIFF_INSTR_COPY, 4, 2, VCDIFF_INSTR_NOOP, 0, 0}, 52, 1 },
    { {VCDIFF_INSTR_COPY, 5, 2, VCDIFF_INSTR_NOOP, 0, 0}, 53, 1 },
    { {VCDIFF_INSTR_COPY, 6, 2, VCDIFF_INSTR_NOOP, 0, 0}, 54, 1 },
    { {VCDIFF_INSTR_COPY, 7, 2, VCDIFF_INSTR_NOOP, 0, 0}, 55, 1 },
    { {VCDIFF_INSTR_COPY, 8, 2, VCDIFF_INSTR_NOOP, 0, 0}, 56, 1 },
    { {VCDIFF_INSTR_COPY, 9, 2, VCDIFF_INSTR_NOOP, 0, 0}, 57, 1 },
    { {VCDIFF_INSTR_COPY, 10, 2, VCDIFF_INSTR_NOOP, 0, 0}, 58, 1 },
    { {VCDIFF_INSTR_COPY, 11, 2, VCDIFF_INSTR_NOOP, 0, 0}, 59, 1 },
    { {VCDIFF_INSTR_COPY, 12, 2, VCDIFF_INSTR_NOOP, 0, 0}, 60, 1 },
    { {VCDIFF_INSTR_COPY, 13, 2, VCDIFF_INSTR_NOOP, 0, 0}, 61, 1 },
    { {VCDIFF_INSTR_COPY, 14, 2, VCDIFF_INSTR_NOOP, 0, 0}, 62, 1 },
    { {VCDIFF_INSTR_COPY, 15, 2, VCDIFF_INSTR_NOOP, 0, 0}, 63, 1 },
    { {VCDIFF_INSTR_COPY, 16, 2, VCDIFF_INSTR_NOOP, 0, 0}, 64, 1 },
    { {VCDIFF_INSTR_COPY, 17, 2, VCDIFF_INSTR_NOOP, 0, 0}, 65, 1 },
    { {VCDIFF_INSTR_COPY, 18, 2, VCDIFF_INSTR_NOOP, 0, 0}, 66, 1 },
    { {VCDIFF_INSTR_COPY, 0, 3, VCDIFF_INSTR_NOOP, 0, 0}, 67, 1 },
    { {VCDIFF_INSTR_COPY, 4, 3, VCDIFF_INSTR_NOOP, 0, 0}, 68, 1 },
    { {VCDIFF_INSTR_COPY, 5, 3, VCDIFF_INSTR_NOOP, 0, 0}, 69, 1 },
    { {VCDIFF_INSTR_COPY, 6, 3, VCDIFF_INSTR_NOOP, 0, 0}, 70, 1 },
    { {VCDIFF_INSTR_COPY, 7, 3, VCDIFF_INSTR_NOOP, 0, 0}, 71, 1 },
    { {VCDIFF_INSTR_COPY, 8, 3, VCDIFF_INSTR_NOOP, 0, 0}, 72, 1 },
    { {VCDIFF_INSTR_COPY, 9, 3, VCDIFF_INSTR_NOOP, 0, 0}, 73, 1 },
    { {VCDIFF_INSTR_COPY, 10, 3, VCDIFF_INSTR_NOOP, 0, 0}, 74, 1 },
    { {VCDIFF_INSTR_COPY, 11, 3, VCDIFF_INSTR_NOOP, 0, 0}, 75, 1 },
    { {VCDIFF_INSTR_COPY, 12, 3, VCDIFF_INSTR_NOOP, 0, 0}, 76, 1 },
    { {VCDIFF_INSTR_COPY, 13, 3, VCDIFF_INSTR_NOOP, 0, 0}, 77, 1 },
    { {VCDIFF_INSTR_COPY, 14, 3, VCDIFF_INSTR_NOOP, 0, 0}, 78, 1 },
    { {VCDIFF_INSTR_COPY, 15, 3, VCDIFF_INSTR_NOOP, 0, 0}, 79, 1 },
    { {VCDIFF_INSTR_COPY, 16, 3, VCDIFF_INSTR_NOOP, 0, 0}, 80, 1 },
    { {VCDIFF_INSTR_COPY, 17, 3, VCDIFF_INSTR_NOOP, 0, 0}, 81, 1 },
    { {VCDIFF_INSTR_COPY, 18, 3, VCDIFF_INSTR_NOOP, 0, 0}, 82, 1 },
    { {VCDIFF_INSTR_COPY, 0, 4, VCDIFF_INSTR_NOOP, 0, 0}, 83, 1 },
    { {VCDIFF_INSTR_COPY, 4, 4, VCDIFF_INSTR_NOOP, 0, 0}, 84, 1 },
    { {VCDIFF_INSTR_COPY, 5, 4, VCDIFF_INSTR_NOOP, 0, 0}, 85, 1 },
    { {VCDIFF_INSTR_COPY, 6, 4, VCDIFF_INSTR_NOOP, 0, 0}, 86, 1 },
    { {VCDIFF_INSTR_COPY, 7, 4, VCDIFF_INSTR_NOOP, 0, 0}, 87, 1 },
    { {VCDIFF_INSTR_COPY, 8, 4, VCDIFF_INSTR_NOOP, 0, 0}, 88, 1 },
    { {VCDIFF_INSTR_COPY, 9, 4, VCDIFF_INSTR_NOOP, 0, 0}, 89, 1 },
    { {VCDIFF_INSTR_COPY, 10, 4, VCDIFF_INSTR_NOOP, 0, 0}, 90, 1 },
    { {VCDIFF_INSTR_COPY, 11, 4, VCDIFF_INSTR_NOOP, 0, 0}, 91, 1 },
    { {VCDIFF_INSTR_COPY, 12, 4, VCDIFF_INSTR_NOOP, 0, 0}, 92, 1 },
    { {VCDIFF_INSTR_COPY, 13, 4, VCDIFF_INSTR_NOOP, 0, 0}, 93, 1 },
    { {VCDIFF_INSTR_COPY, 14, 4, VCDIFF_INSTR_NOOP, 0, 0}, 94, 1 },
    { {VCDIFF_INSTR_COPY, 15, 4, VCDIFF_INSTR_NOOP, 0, 0}, 95, 1 },
    { {VCDIFF_INSTR_COPY, 16, 4, VCDIFF_INSTR_NOOP, 0, 0}, 96, 1 },
    { {VCDIFF_INSTR_COPY, 17, 4, VCDIFF_INSTR_NOOP, 0, 0}, 97, 1 },
    { {VCDIFF_INSTR_COPY, 18, 4, VCDIFF_INSTR_NOOP, 0, 0}, 98, 1 },
    { {VCDIFF_INSTR_COPY, 0, 5, VCDIFF_INSTR_NOOP, 0, 0}, 99, 1 },
    { {VCDIFF_INSTR_COPY, 4, 5, VCDIFF_INSTR_NOOP, 0, 0}, 100, 1 },
    { {VCDIFF_INSTR_COPY, 5, 5, VCDIFF_INSTR_NOOP, 0, 0}, 101, 1 },
    { {VCDIFF_INSTR_COPY, 6, 5, VCDIFF_INSTR_NOOP, 0, 0}, 102, 1 },
    { {VCDIFF_INSTR_COPY, 7, 5, VCDIFF_INSTR_NOOP, 0, 0}, 103, 1 },
    { {VCDIFF_INSTR_COPY, 8, 5, VCDIFF_INSTR_NOOP, 0, 0}, 104, 1 },
    { {VCDIFF_INSTR_COPY, 9, 5, VCDIFF_INSTR_NOOP, 0, 0}, 105, 1 },
    { {VCDIFF_INSTR_COPY, 10, 5, VCDIFF_INSTR_NOOP, 0, 0}, 106, 1 },
    { {VCDIFF_INSTR_COPY, 11, 5, VCDIFF_INSTR_NOOP, 0, 0}, 107, 1 },
    { {VCDIFF_INSTR_COPY, 12, 5, VCDIFF_INSTR_NOOP, 0, 0}, 108, 1 },
    { {VCDIFF_INSTR_COPY, 13, 5, VCDIFF_INSTR_NOOP, 0, 0}, 109, 1 },
    { {VCDIFF_INSTR_COPY, 14, 5, VCDIFF_INSTR_NOOP, 0, 0}, 110, 1 },
    { {VCDIFF_INSTR_COPY, 15, 5, VCDIFF_INSTR_NOOP, 0, 0}, 111, 1 },
    { {VCDIFF_INSTR_COPY, 16, 5, VCDIFF_INSTR_NOOP, 0, 0}, 112, 1 },
    { {VCDIFF_INSTR_COPY, 17, 5, VCDIFF_INSTR_NOOP, 0, 0}, 113, 1 },
    { {VCDIFF_INSTR_COPY, 18, 5, VCDIFF_INSTR_NOOP, 0, 0}, 114, 1 },
    { {VCDIFF_INSTR_COPY, 0, 6, VCDIFF_INSTR_NOOP, 0, 0}, 115, 1 },
    { {VCDIFF_INSTR_COPY, 4, 6, VCDIFF_INSTR_NOOP, 0, 0}, 116, 1 },
    { {VCDIFF_INSTR_COPY, 5, 6, VCDIFF_INSTR_NOOP, 0, 0}, 117, 1 },
    { {VCDIFF_INSTR_COPY, 6, 6, VCDIFF_INSTR_NOOP, 0, 0}, 118, 1 },
    { {VCDIFF_INSTR_COPY, 7, 6, VCDIFF_INSTR_NOOP, 0, 0}, 119, 1 },
    { {VCDIFF_INSTR_COPY, 8, 6, VCDIFF_INSTR_NOOP, 0, 0}, 120, 1 },
    { {VCDIFF_INSTR_COPY, 9, 6, VCDIFF_INSTR_NOOP, 0, 0}, 121, 1 },
    { {VCDIFF_INSTR_COPY, 10, 6, VCDIFF_INSTR_NOOP, 0, 0}, 122, 1 },
    { {VCDIFF_INSTR_COPY, 11, 6, VCDIFF_INSTR_NOOP, 0, 0}, 123, 1 },
    { {VCDIFF_INSTR_COPY, 12, 6, VCDIFF_INSTR_NOOP, 0, 0}, 124, 1 },
    { {VCDIFF_INSTR_COPY, 13, 6, VCDIFF_INSTR_NOOP, 0, 0}, 125, 1 },
    { {VCDIFF_INSTR_COPY, 14, 6, VCDIFF_INSTR_NOOP, 0, 0}, 126, 1 },
    { {VCDIFF_INSTR_COPY, 15, 6, VCDIFF_INSTR_NOOP, 0, 0}, 127, 1 },
    { {VCDIFF_INSTR_COPY, 16, 6, VCDIFF_INSTR_NOOP, 0, 0}, 128, 1 },
    { {VCDIFF_INSTR_COPY, 17, 6, VCDIFF_INSTR_NOOP, 0, 0}, 129, 1 },
    { {VCDIFF_INSTR_COPY, 18, 6, VCDIFF_INSTR_NOOP, 0, 0}, 130, 1 },
    { {VCDIFF_INSTR_COPY, 0, 7, VCDIFF_INSTR_NOOP, 0, 0}, 131, 1 },
    { {VCDIFF_INSTR_COPY, 4, 7, VCDIFF_INSTR_NOOP, 0, 0}, 132, 1 },
    { {VCDIFF_INSTR_COPY, 5, 7, VCDIFF_INSTR_NOOP, 0, 0}, 133, 1 },
    { {VCDIFF_INSTR_COPY, 6, 7, VCDIFF_INSTR_NOOP, 0, 0}, 134, 1 },
    { {VCDIFF_INSTR_COPY, 7, 7, VCDIFF_INSTR_NOOP, 0, 0}, 135, 1 },
    { {VCDIFF_INSTR_COPY, 8, 7, VCDIFF_INSTR_NOOP, 0, 0}, 136, 1 },
    { {VCDIFF_INSTR_COPY, 9, 7, VCDIFF_INSTR_NOOP, 0, 0}, 137, 1 },
    { {VCDIFF_INSTR_COPY, 10, 7, VCDIFF_INSTR_NOOP, 0, 0}, 138, 1 },
    { {VCDIFF_INSTR_COPY, 11, 7, VCDIFF_INSTR_NOOP, 0, 0}, 139, 1 },
    { {VCDIFF_INSTR_COPY, 12, 7, VCDIFF_INSTR_NOOP, 0, 0}, 140, 1 },
    { {VCDIFF_INSTR_COPY, 13, 7, VCDIFF_INSTR_NOOP, 0, 0}, 141, 1 },
    { {VCDIFF_INSTR_COPY, 14, 7, VCDIFF_INSTR_NOOP, 0, 0}, 142, 1 },
    { {VCDIFF_INSTR_COPY, 15, 7, VCDIFF_INSTR_NOOP, 0, 0}, 143, 1 },
    { {VCDIFF_INSTR_COPY, 16, 7, VCDIFF_INSTR_NOOP, 0, 0}, 144, 1 },
    { {VCDIFF_INSTR_COPY, 17, 7, VCDIFF_INSTR_NOOP, 0, 0}, 145, 1 },
    { {VCDIFF_INSTR_COPY, 18, 7, VCDIFF_INSTR_NOOP, 0, 0}, 146, 1 },
    { {VCDIFF_INSTR_COPY, 0, 8, VCDIFF_INSTR_NOOP, 0, 0}, 147, 1 },
    { {VCDIFF_INSTR_COPY, 4, 8, VCDIFF_INSTR_NOOP, 0, 0}, 148, 1 },
    { {VCDIFF_INSTR_COPY, 5, 8, VCDIFF_INSTR_NOOP, 0, 0}, 149, 1 },
    { {VCDIFF_INSTR_COPY, 6, 8, VCDIFF_INSTR_NOOP, 0, 0}, 150, 1 },
    { {VCDIFF_INSTR_COPY, 7, 8, VCDIFF_INSTR_NOOP, 0, 0}, 151, 1 },
    { {VCDIFF_INSTR_COPY, 8, 8, VCDIFF_INSTR_NOOP, 0, 0}, 152, 1 },
    { {VCDIFF_INSTR_COPY, 9, 8, VCDIFF_INSTR_NOOP, 0, 0}, 153, 1 },
    { {VCDIFF_INSTR_COPY, 10, 8, VCDIFF_INSTR_NOOP, 0, 0}, 154, 1 },
    { {VCDIFF_INSTR_COPY, 11, 8, VCDIFF_INSTR_NOOP, 0, 0}, 155, 1 },
    { {VCDIFF_INSTR_COPY, 12, 8, VCDIFF_INSTR_NOOP, 0, 0}, 156, 1 },
    { {VCDIFF_INSTR_COPY, 13, 8, VCDIFF_INSTR_NOOP, 0, 0}, 157, 1 },
    { {VCDIFF_INSTR_COPY, 14, 8, VCDIFF_INSTR_NOOP, 0, 0}, 158, 1 },
    { {VCDIFF_INSTR_COPY, 15, 8, VCDIFF_INSTR_NOOP, 0, 0}, 159, 1 },
    { {VCDIFF_INSTR_COPY, 16, 8, VCDIFF_INSTR_NOOP, 0, 0}, 160, 1 },
    { {VCDIFF_INSTR_COPY, 17, 8, VCDIFF_INSTR_NOOP, 0, 0}, 161, 1 },
    { {VCDIFF_INSTR_COPY, 18, 8, VCDIFF_INSTR_NOOP, 0, 0}, 162, 1 },
    { {VCDIFF_INSTR_ADD, 1, 0, VCDIFF_INSTR_COPY, 4, 0}, 163, 2 },
    { {VCDIFF_INSTR_ADD, 1, 0, VCDIFF_INSTR_COPY, 5, 0}, 164, 2 },
    { {VCDIFF_INSTR_ADD, 1, 0, VCDIFF_INSTR_COPY, 6, 0}, 165, 2 },
    { {VCDIFF_INSTR_ADD, 2, 0, VCDIFF_INSTR_COPY, 4, 0}, 166, 2 },
    { {VCDIFF_INSTR_ADD, 2, 0, VCDIFF_INSTR_COPY, 5, 0}, 167, 2 },
    { {VCDIFF_INSTR_ADD, 2, 0, VCDIFF_INSTR_COPY, 6, 0}, 168, 2 },
    { {VCDIFF_INSTR_ADD, 3, 0, VCDIFF_INSTR_COPY, 4, 0}, 169, 2 },
    { {VCDIFF_INSTR_ADD, 3, 0, VCDIFF_INSTR_COPY, 5, 0}, 170, 2 },
    { {VCDIFF_INSTR_ADD, 3, 0, VCDIFF_INSTR_COPY, 6, 0}, 171, 2 },
    { {VCDIFF_INSTR_ADD, 4, 0, VCDIFF_INSTR_COPY, 4, 0}, 172, 2 },
    { {VCDIFF_INSTR_ADD, 4, 0, VCDIFF_INSTR_COPY, 5, 0}, 173, 2 },
    { {VCDIFF_INSTR_ADD, 4, 0, VCDIFF_INSTR_COPY, 6, 0}, 174, 2 },
    { {VCDIFF_INSTR_ADD, 1, 0, VCDIFF_INSTR_COPY, 4, 1}, 175, 2 },
    { {VCDIFF_INSTR_ADD, 1, 0, VCDIFF_INSTR_COPY, 5, 1}, 176, 2 },
    { {VCDIFF_INSTR_ADD, 1, 0, VCDIFF_INSTR_COPY, 6, 1}, 177, 2 },
    { {VCDIFF_INSTR_ADD, 2, 0, VCDIFF_INSTR_COPY, 4, 1}, 178, 2 },
    { {VCDIFF_INSTR_ADD, 2, 0, VCDIFF_INSTR_COPY, 5, 1}, 179, 2 },
    { {VCDIFF_INSTR_ADD, 2, 0, VCDIFF_INSTR_COPY, 6, 1}, 180, 2 },
    { {VCDIFF_INSTR_ADD, 3, 0, VCDIFF_INSTR_COPY, 4, 1}, 181, 2 },
    { {VCDIFF_INSTR_ADD, 3, 0, VCDIFF_INSTR_COPY, 5, 1}, 182, 2 },
    { {VCDIFF_INSTR_ADD, 3, 0, VCDIFF_INSTR_COPY, 6, 1}, 183, 2 },
    { {VCDIFF_INSTR_ADD, 4, 0, VCDIFF_INSTR_COPY, 4, 1}, 184, 2 },
    { {VCDIFF_INSTR_ADD, 4, 0, VCDIFF_INSTR_COPY, 5, 1}, 185, 2 },
    { {VCDIFF_INSTR_ADD, 4, 0, VCDIFF_INSTR_COPY, 6, 1}, 186, 2 },
    { {VCDIFF_INSTR_ADD, 1, 0, VCDIFF_INSTR_COPY, 4, 2}, 187, 2 },
    { {VCDIFF_INSTR_ADD, 1, 0, VCDIFF_INSTR_COPY, 5, 2}, 188, 2 },
    { {VCDIFF_INSTR_ADD, 1, 0, VCDIFF_INSTR_COPY, 6, 2}, 189, 2 },
    { {VCDIFF_INSTR_ADD, 2, 0, VCDIFF_INSTR_COPY, 4, 2}, 190, 2 },
    { {VCDIFF_INSTR_ADD, 2, 0, VCDIFF_INSTR_COPY, 5, 2}, 191, 2 },
    { {VCDIFF_INSTR_ADD, 2, 0, VCDIFF_INSTR_COPY, 6, 2}, 192, 2 },
    { {VCDIFF_INSTR_ADD, 3, 0, VCDIFF_INSTR_COPY, 4, 2}, 193, 2 },
    { {VCDIFF_INSTR_ADD, 3, 0, VCDIFF_INSTR_COPY, 5, 2}, 194, 2 },
    { {VCDIFF_INSTR_ADD, 3, 0, VCDIFF_INSTR_COPY, 6, 2}, 195, 2 },
    { {VCDIFF_INSTR_ADD, 4, 0, VCDIFF_INSTR_COPY, 4, 2}, 196, 2 },
    { {VCDIFF_INSTR_ADD, 4, 0, VCDIFF_INSTR_COPY, 5, 2}, 197, 2 },
    { {VCDIFF_INSTR_ADD, 4, 0, VCDIFF_INSTR_COPY, 6, 2}, 198, 2 },
    { {VCDIFF_INSTR_ADD, 1, 0, VCDIFF_INSTR_COPY, 4, 3}, 199, 2 },
    { {VCDIFF_INSTR_ADD, 1, 0, VCDIFF_INSTR_COPY, 5, 3}, 200, 2 },
    { {VCDIFF_INSTR_ADD, 1, 0, VCDIFF_INSTR_COPY, 6, 3}, 201, 2 },
    { {VCDIFF_INSTR_ADD, 2, 0, VCDIFF_INSTR_COPY, 4, 3}, 202, 2 },
    { {VCDIFF_INSTR_ADD, 2, 0, VCDIFF_INSTR_COPY, 5, 3}, 203, 2 },
    { {VCDIFF_INSTR_ADD, 2, 0, VCDIFF_INSTR_COPY, 6, 3}, 204, 2 },
    { {VCDIFF_INSTR_ADD, 3, 0, VCDIFF_INSTR_COPY, 4, 3}, 205, 2 },
    { {VCDIFF_INSTR_ADD, 3, 0, VCDIFF_INSTR_COPY, 5, 3}, 206, 2 },
    { {VCDIFF_INSTR_ADD, 3, 0, VCDIFF_INSTR_COPY, 6, 3}, 207, 2 },
    { {VCDIFF_INSTR_ADD, 4, 0, VCDIFF_INSTR_COPY, 4, 3}, 208, 2 },
    { {VCDIFF_INSTR_ADD, 4, 0, VCDIFF_INSTR_COPY, 5, 3}, 209, 2 },
    { {VCDIFF_INSTR_ADD, 4, 0, VCDIFF_INSTR_COPY, 6, 3}, 210, 2 },
    { {VCDIFF_INSTR_ADD, 1, 0, VCDIFF_INSTR_COPY, 4, 4}, 211, 2 },
    { {VCDIFF_INSTR_ADD, 1, 0, VCDIFF_INSTR_COPY, 5, 4}, 212, 2 },
    { {VCDIFF_INSTR_ADD, 1, 0, VCDIFF_INSTR_COPY, 6, 4}, 213, 2 },
    { {VCDIFF_INSTR_ADD, 2, 0, VCDIFF_INSTR_COPY, 4, 4}, 214, 2 },
    { {VCDIFF_INSTR_ADD, 2, 0, VCDIFF_INSTR_COPY, 5, 4}, 215, 2 },
    { {VCDIFF_INSTR_ADD, 2, 0, VCDIFF_INSTR_COPY, 6, 4}, 216, 2 },
    { {VCDIFF_INSTR_ADD, 3, 0, VCDIFF_INSTR_COPY, 4, 4}, 217, 2 },
    { {VCDIFF_INSTR_ADD, 3, 0, VCDIFF_INSTR_COPY, 5, 4}, 218, 2 },
    { {VCDIFF_INSTR_ADD, 3, 0, VCDIFF_INSTR_COPY, 6, 4}, 219, 2 },
    { {VCDIFF_INSTR_ADD, 4, 0, VCDIFF_INSTR_COPY, 4, 4}, 220, 2 },
    { {VCDIFF_INSTR_ADD, 4, 0, VCDIFF_INSTR_COPY, 5, 4}, 221, 2 },
    { {VCDIFF_INSTR_ADD, 4, 0, VCDIFF_INSTR_COPY, 6, 4}, 222, 2 },
    { {VCDIFF_INSTR_ADD, 1, 0, VCDIFF_INSTR_COPY, 4, 5}, 223, 2 },
    { {VCDIFF_INSTR_ADD, 1, 0, VCDIFF_INSTR_COPY, 5, 5}, 224, 2 },
    { {VCDIFF_INSTR_ADD, 1, 0, VCDIFF_INSTR_COPY, 6, 5}, 225, 2 },
    { {VCDIFF_INSTR_ADD, 2, 0, VCDIFF_INSTR_COPY, 4, 5}, 226, 2 },
    { {VCDIFF_INSTR_ADD, 2, 0, VCDIFF_INSTR_COPY, 5, 5}, 227, 2 },
    { {VCDIFF_INSTR_ADD, 2, 0, VCDIFF_INSTR_COPY, 6, 5}, 228, 2 },
    { {VCDIFF_INSTR_ADD, 3, 0, VCDIFF_INSTR_COPY, 4, 5}, 229, 2 },
    { {VCDIFF_INSTR_ADD, 3, 0, VCDIFF_INSTR_COPY, 5, 5}, 230, 2 },
    { {VCDIFF_INSTR_ADD, 3, 0, VCDIFF_INSTR_COPY, 6, 5}, 231, 2 },
    { {VCDIFF_INSTR_ADD, 4, 0, VCDIFF_INSTR_COPY, 4, 5}, 232, 2 },
    { {VCDIFF_INSTR_ADD, 4, 0, VCDIFF_INSTR_COPY, 5, 5}, 233, 2 },
    { {VCDIFF_INSTR_ADD, 4, 0, VCDIFF_INSTR_COPY, 6, 5}, 234, 2 },
    { {VCDIFF_INSTR_ADD, 1, 0, VCDIFF_INSTR_COPY, 4, 6}, 235, 2 },
    { {VCDIFF_INSTR_ADD, 2, 0, VCDIFF_INSTR_COPY, 4, 6}, 236, 2 },
    { {VCDIFF_INSTR_ADD, 3, 0, VCDIFF_INSTR_COPY, 4, 6}, 237, 2 },
    { {VCDIFF_INSTR_ADD, 4, 0, VCDIFF_INSTR_COPY, 4, 6}, 238, 2 },
    { {VCDIFF_INSTR_ADD, 1, 0, VCDIFF_INSTR_COPY, 4, 7}, 239, 2 },
    { {VCDIFF_INSTR_ADD, 2, 0, VCDIFF_INSTR_COPY, 4, 7}, 240, 2 },
    { {VCDIFF_INSTR_ADD, 3, 0, VCDIFF_INSTR_COPY, 4, 7}, 241, 2 },
    { {VCDIFF_INSTR_ADD, 4, 0, VCDIFF_INSTR_COPY, 4, 7}, 242, 2 },
    { {VCDIFF_INSTR_ADD, 1, 0, VCDIFF_INSTR_COPY, 4, 8}, 243, 2 },
    { {VCDIFF_INSTR_ADD, 2, 0, VCDIFF_INSTR_COPY, 4, 8}, 244, 2 },
    { {VCDIFF_INSTR_ADD, 3, 0, VCDIFF_INSTR_COPY, 4, 8}, 245, 2 },
    { {VCDIFF_INSTR_ADD, 4, 0, VCDIFF_INSTR_COPY, 4, 8}, 246, 2 },
    { {VCDIFF_INSTR_COPY, 4, 0, VCDIFF_INSTR_ADD, 1, 0}, 247, 2 },
    { {VCDIFF_INSTR_COPY, 4, 1, VCDIFF_INSTR_ADD, 1, 0}, 248, 2 },
    { {VCDIFF_INSTR_COPY, 4, 2, VCDIFF_INSTR_ADD, 1, 0}, 249, 2 },
    { {VCDIFF_INSTR_COPY, 4, 3, VCDIFF_INSTR_ADD, 1, 0}, 250, 2 },
    { {VCDIFF_INSTR_COPY, 4, 4, VCDIFF_INSTR_ADD, 1, 0}, 251, 2 },
    { {VCDIFF_INSTR_COPY, 4, 5, VCDIFF_INSTR_ADD, 1, 0}, 252, 2 },
    { {VCDIFF_INSTR_COPY, 4, 6, VCDIFF_INSTR_ADD, 1, 0}, 253, 2 },
    { {VCDIFF_INSTR_COPY, 4, 7, VCDIFF_INSTR_ADD, 1, 0}, 254, 2 },
    { {VCDIFF_INSTR_COPY, 4, 8, VCDIFF_INSTR_ADD, 1, 0}, 255, 2 },

    { {VCDIFF_INSTR_RUN, 5, 0, VCDIFF_INSTR_NOOP, 0, 0}, 0, 0 },
    { {VCDIFF_INSTR_RUN, 20, 0, VCDIFF_INSTR_NOOP, 0, 0}, 0, 0 },
    { {VCDIFF_INSTR_RUN, 100, 0, VCDIFF_INSTR_NOOP, 0, 0}, 0, 0 },
    { {VCDIFF_INSTR_RUN, 255, 0, VCDIFF_INSTR_NOOP, 0, 0}, 0, 0 },
    { {VCDIFF_INSTR_ADD, 20, 0, VCDIFF_INSTR_NOOP, 0, 0}, 1, 0 },
    { {VCDIFF_INSTR_ADD, 100, 0, VCDIFF_INSTR_NOOP, 0, 0}, 1, 0 },
    { {VCDIFF_INSTR_ADD, 255, 0, VCDIFF_INSTR_NOOP, 0, 0}, 1, 0 },
    { {VCDIFF_INSTR_COPY, 20, 0, VCDIFF_INSTR_NOOP, 0, 0}, 19, 0 },
    { {VCDIFF_INSTR_COPY, 100, 0, VCDIFF_INSTR_NOOP, 0, 0}, 19, 0 },
    { {VCDIFF_INSTR_COPY, 255, 0, VCDIFF_INSTR_NOOP, 0, 0}, 19, 0 },
    { {VCDIFF_INSTR_COPY, 20, 1, VCDIFF_INSTR_NOOP, 0, 0}, 35, 0 },
    { {VCDIFF_INSTR_COPY, 100, 1, VCDIFF_INSTR_NOOP, 0, 0}, 35, 0 },
    { {VCDIFF_INSTR_COPY, 255, 1, VCDIFF_INSTR_NOOP, 0, 0}, 35, 0 },
    { {VCDIFF_INSTR_COPY, 20, 2, VCDIFF_INSTR_NOOP, 0, 0}, 51, 0 },
    { {VCDIFF_INSTR_COPY, 100, 2, VCDIFF_INSTR_NOOP, 0, 0}, 51, 0 },
    { {VCDIFF_INSTR_COPY, 255, 2, VCDIFF_INSTR_NOOP, 0, 0}, 51, 0 },
    { {VCDIFF_INSTR_COPY, 20, 3, VCDIFF_INSTR_NOOP, 0, 0}, 67, 0 },
    { {VCDIFF_INSTR_COPY, 100, 3, VCDIFF_INSTR_NOOP, 0, 0}, 67, 0 },
    { {VCDIFF_INSTR_COPY, 255, 3, VCDIFF_INSTR_NOOP, 0, 0}, 67, 0 },
    { {VCDIFF_INSTR_COPY, 20, 4, VCDIFF_INSTR_NOOP, 0, 0}, 83, 0 },
    { {VCDIFF_INSTR_COPY, 100, 4, VCDIFF_INSTR_NOOP, 0, 0}, 83, 0 },
    { {VCDIFF_INSTR_COPY, 255, 4, VCDIFF_INSTR_NOOP, 0, 0}, 83, 0 },
    { {VCDIFF_INSTR_COPY, 20, 5, VCDIFF_INSTR_NOOP, 0, 0}, 99, 0 },
    { {VCDIFF_INSTR_COPY, 100, 5, VCDIFF_INSTR_NOOP, 0, 0}, 99, 0 },
    { {VCDIFF_INSTR_COPY, 255, 5, VCDIFF_INSTR_NOOP, 0, 0}, 99, 0 },
    { {VCDIFF_INSTR_COPY, 20, 6, VCDIFF_INSTR_NOOP, 0, 0}, 115, 0 },
    { {VCDIFF_INSTR_COPY, 100, 6, VCDIFF_INSTR_NOOP, 0, 0}, 115, 0 },
    { {VCDIFF_INSTR_COPY, 255, 6, VCDIFF_INSTR_NOOP, 0, 0}, 115, 0 },
    { {VCDIFF_INSTR_COPY, 20, 7, VCDIFF_INSTR_NOOP, 0, 0}, 131, 0 },
    { {VCDIFF_INSTR_COPY, 100, 7, VCDIFF_INSTR_NOOP, 0, 0}, 131, 0 },
    { {VCDIFF_INSTR_COPY, 255, 7, VCDIFF_INSTR_NOOP, 0, 0}, 131, 0 },
    { {VCDIFF_INSTR_COPY, 20, 8, VCDIFF_INSTR_NOOP, 0, 0}, 147, 0 },
    { {VCDIFF_INSTR_COPY, 100, 8, VCDIFF_INSTR_NOOP, 0, 0}, 147, 0 },
    { {VCDIFF_INSTR_COPY, 255, 8, VCDIFF_INSTR_NOOP, 0, 0}, 147, 0 }
};


static const uint16_t default_instructions_expected_indices[4] = {
    VCDIFF_CODE_TABLE_INVALID_OPCODE,
    1,
    19,
    0
};


static const uint16_t default_copy_instructions_expected_indices[9] = {
    19,  /* COPY 0 0 */
    35,  /* COPY 0 1 */
    51,  /* COPY 0 2 */
    67,  /* COPY 0 3 */
    83,  /* COPY 0 4 */
    99,  /* COPY 0 5 */
    115, /* COPY 0 6 */
    131, /* COPY 0 7 */
    147  /* COPY 0 8 */
};


/**
 * Test for "default" instructions.
 */
START_TEST(test_default_instruction_indices)
{
    vcdiff_codetable_tree *tree = vcdiff_new_codetable_tree(vcdiff_default_instr_table);

    ck_assert_mem_eq(tree->default_opcodes,
                     default_instructions_expected_indices,
                     VCDIFF_INSTR_INVALID);

    ck_assert_mem_eq(tree->default_copy_instr,
                     default_copy_instructions_expected_indices,
                     sizeof(default_copy_instructions_expected_indices) / sizeof(uint16_t));

    vcdiff_free_codetable_tree(tree);
}
END_TEST


/**
 * Valgrind test for codetree memory allocation.
 */
START_TEST(test_codetree_allocation)
{
    vcdiff_codetable_tree *tree = vcdiff_new_codetable_tree(vcdiff_default_instr_table);

    ck_assert_ptr_nonnull(tree->first);
    ck_assert_ptr_nonnull(tree->second);
    ck_assert_ptr_nonnull(tree->opcodes);
    ck_assert_ptr_nonnull(tree->second_size);

    ck_assert_uint_eq(tree->first_size, 163);

    vcdiff_free_codetable_tree(tree);
}
END_TEST


/**
 * Test for vcdiff_find_instruction()
 */
START_TEST(test_find_instruction)
{
    vcdiff_codetable_tree *tree = vcdiff_new_codetable_tree(vcdiff_default_instr_table);

    uint16_t opcode;
    char *instr = (char *)&test_find_instruction_expected_pairs[_i].instr;

    unsigned int encoded_n = vcdiff_find_instruction(
        tree,
        instr,
        instr + VCDIFF_CODE_INSTR_SIZE,
        &opcode);

    vcdiff_free_codetable_tree(tree);

    ck_assert_uint_eq(opcode,
                      test_find_instruction_expected_pairs[_i].expected_opcode);
    ck_assert_uint_eq(encoded_n,
                      test_find_instruction_expected_pairs[_i].expected_n_encoded);
}
END_TEST


Suite * vcdiff_code_table_suite()
{
    Suite *s = suite_create("vcdiff_code_table");

    TCase *tc_instr = tcase_create("Instructions");
    tcase_add_loop_test(tc_instr, test_find_instruction, 0,
                        sizeof(test_find_instruction_expected_pairs) /
                        sizeof(test_read_instruction_pair));
    tcase_add_test(tc_instr, test_default_instruction_indices);

    TCase *tc_memory = tcase_create("Memory");
    tcase_add_test(tc_memory, test_codetree_allocation);

    suite_add_tcase(s, tc_instr);
    suite_add_tcase(s, tc_memory);

    return s;
}
