#ifndef VCDIFF_DEFAULT_CODE_TABLE
#define VCDIFF_DEFAULT_CODE_TABLE

#include <stdlib.h>
#include <stdint.h>
#include "vcdiff_io.h"


#define VCDIFF_CODE_TABLE_SIZE 256
#define VCDIFF_CODE_INSTR_SIZE 3
#define VCDIFF_CODE_INSTR_PAIR_SIZE 6
#define VCDIFF_CODE_TABLE_INVALID_OPCODE VCDIFF_CODE_TABLE_SIZE

#define VCDIFF_POSITION_INSTR_TYPE 0
#define VCDIFF_POSITION_INSTR_SIZE 1
#define VCDIFF_POSITION_INSTR_MODE 2


enum vcdiff_instructions {
    VCDIFF_INSTR_NOOP = 0,
    VCDIFF_INSTR_ADD = 1,
    VCDIFF_INSTR_RUN = 2,
    VCDIFF_INSTR_COPY = 3,
    VCDIFF_INSTR_INVALID = 4
};


typedef struct {
    char instr_type;
    char mode;
    char *data;

    size_t size;
    size_t addr_size;
} vcdiff_raw_instr;


typedef struct {
    char instr_type;
    char size;
    char mode;

    char instr_type2;
    char size2;
    char mode2;
}
#ifdef __GNUC__
__attribute__ ((packed))
#endif
vcdiff_code_table_entry;


/*
 *     RFC:
 *
 *         TYPE      SIZE     MODE    TYPE     SIZE     MODE     INDEX
 *        ---------------------------------------------------------------
 *     1.  RUN         0        0     NOOP       0        0        0
 *     2.  ADD    0, [1,17]     0     NOOP       0        0      [1,18]
 *     3.  COPY   0, [4,18]     0     NOOP       0        0     [19,34]
 *     4.  COPY   0, [4,18]     1     NOOP       0        0     [35,50]
 *     5.  COPY   0, [4,18]     2     NOOP       0        0     [51,66]
 *     6.  COPY   0, [4,18]     3     NOOP       0        0     [67,82]
 *     7.  COPY   0, [4,18]     4     NOOP       0        0     [83,98]
 *     8.  COPY   0, [4,18]     5     NOOP       0        0     [99,114]
 *     9.  COPY   0, [4,18]     6     NOOP       0        0    [115,130]
 *    10.  COPY   0, [4,18]     7     NOOP       0        0    [131,146]
 *    11.  COPY   0, [4,18]     8     NOOP       0        0    [147,162]
 *    12.  ADD       [1,4]      0     COPY     [4,6]      0    [163,174]
 *    13.  ADD       [1,4]      0     COPY     [4,6]      1    [175,186]
 *    14.  ADD       [1,4]      0     COPY     [4,6]      2    [187,198]
 *    15.  ADD       [1,4]      0     COPY     [4,6]      3    [199,210]
 *    16.  ADD       [1,4]      0     COPY     [4,6]      4    [211,222]
 *    17.  ADD       [1,4]      0     COPY     [4,6]      5    [223,234]
 *    18.  ADD       [1,4]      0     COPY       4        6    [235,238]
 *    19.  ADD       [1,4]      0     COPY       4        7    [239,242]
 *    20.  ADD       [1,4]      0     COPY       4        8    [243,246]
 *    21.  COPY        4      [0,8]   ADD        1        0    [247,255]
 *        ---------------------------------------------------------------
 */


#define R VCDIFF_INSTR_RUN
#define A VCDIFF_INSTR_ADD
#define C VCDIFF_INSTR_COPY
#define N VCDIFF_INSTR_NOOP


static vcdiff_code_table_entry
    vcdiff_default_instr_table[VCDIFF_CODE_TABLE_SIZE] = {
    {R, 0, 0, N, 0, 0},  /*   0 */
    {A, 0, 0, N, 0, 0},  /*   1 */
    {A, 1, 0, N, 0, 0},  /*   2 */
    {A, 2, 0, N, 0, 0},  /*   3 */
    {A, 3, 0, N, 0, 0},  /*   4 */
    {A, 4, 0, N, 0, 0},  /*   5 */
    {A, 5, 0, N, 0, 0},  /*   6 */
    {A, 6, 0, N, 0, 0},  /*   7 */
    {A, 7, 0, N, 0, 0},  /*   8 */
    {A, 8, 0, N, 0, 0},  /*   9 */
    {A, 9, 0, N, 0, 0},  /*  10 */
    {A, 10, 0, N, 0, 0},  /*  11 */
    {A, 11, 0, N, 0, 0},  /*  12 */
    {A, 12, 0, N, 0, 0},  /*  13 */
    {A, 13, 0, N, 0, 0},  /*  14 */
    {A, 14, 0, N, 0, 0},  /*  15 */
    {A, 15, 0, N, 0, 0},  /*  16 */
    {A, 16, 0, N, 0, 0},  /*  17 */
    {A, 17, 0, N, 0, 0},  /*  18 */
    {C, 0, 0, N, 0, 0},  /*  19 */
    {C, 4, 0, N, 0, 0},  /*  20 */
    {C, 5, 0, N, 0, 0},  /*  21 */
    {C, 6, 0, N, 0, 0},  /*  22 */
    {C, 7, 0, N, 0, 0},  /*  23 */
    {C, 8, 0, N, 0, 0},  /*  24 */
    {C, 9, 0, N, 0, 0},  /*  25 */
    {C, 10, 0, N, 0, 0},  /*  26 */
    {C, 11, 0, N, 0, 0},  /*  27 */
    {C, 12, 0, N, 0, 0},  /*  28 */
    {C, 13, 0, N, 0, 0},  /*  29 */
    {C, 14, 0, N, 0, 0},  /*  30 */
    {C, 15, 0, N, 0, 0},  /*  31 */
    {C, 16, 0, N, 0, 0},  /*  32 */
    {C, 17, 0, N, 0, 0},  /*  33 */
    {C, 18, 0, N, 0, 0},  /*  34 */
    {C, 0, 1, N, 0, 0},  /*  35 */
    {C, 4, 1, N, 0, 0},  /*  36 */
    {C, 5, 1, N, 0, 0},  /*  37 */
    {C, 6, 1, N, 0, 0},  /*  38 */
    {C, 7, 1, N, 0, 0},  /*  39 */
    {C, 8, 1, N, 0, 0},  /*  40 */
    {C, 9, 1, N, 0, 0},  /*  41 */
    {C, 10, 1, N, 0, 0},  /*  42 */
    {C, 11, 1, N, 0, 0},  /*  43 */
    {C, 12, 1, N, 0, 0},  /*  44 */
    {C, 13, 1, N, 0, 0},  /*  45 */
    {C, 14, 1, N, 0, 0},  /*  46 */
    {C, 15, 1, N, 0, 0},  /*  47 */
    {C, 16, 1, N, 0, 0},  /*  48 */
    {C, 17, 1, N, 0, 0},  /*  49 */
    {C, 18, 1, N, 0, 0},  /*  50 */
    {C, 0, 2, N, 0, 0},  /*  51 */
    {C, 4, 2, N, 0, 0},  /*  52 */
    {C, 5, 2, N, 0, 0},  /*  53 */
    {C, 6, 2, N, 0, 0},  /*  54 */
    {C, 7, 2, N, 0, 0},  /*  55 */
    {C, 8, 2, N, 0, 0},  /*  56 */
    {C, 9, 2, N, 0, 0},  /*  57 */
    {C, 10, 2, N, 0, 0},  /*  58 */
    {C, 11, 2, N, 0, 0},  /*  59 */
    {C, 12, 2, N, 0, 0},  /*  60 */
    {C, 13, 2, N, 0, 0},  /*  61 */
    {C, 14, 2, N, 0, 0},  /*  62 */
    {C, 15, 2, N, 0, 0},  /*  63 */
    {C, 16, 2, N, 0, 0},  /*  64 */
    {C, 17, 2, N, 0, 0},  /*  65 */
    {C, 18, 2, N, 0, 0},  /*  66 */
    {C, 0, 3, N, 0, 0},  /*  67 */
    {C, 4, 3, N, 0, 0},  /*  68 */
    {C, 5, 3, N, 0, 0},  /*  69 */
    {C, 6, 3, N, 0, 0},  /*  70 */
    {C, 7, 3, N, 0, 0},  /*  71 */
    {C, 8, 3, N, 0, 0},  /*  72 */
    {C, 9, 3, N, 0, 0},  /*  73 */
    {C, 10, 3, N, 0, 0},  /*  74 */
    {C, 11, 3, N, 0, 0},  /*  75 */
    {C, 12, 3, N, 0, 0},  /*  76 */
    {C, 13, 3, N, 0, 0},  /*  77 */
    {C, 14, 3, N, 0, 0},  /*  78 */
    {C, 15, 3, N, 0, 0},  /*  79 */
    {C, 16, 3, N, 0, 0},  /*  80 */
    {C, 17, 3, N, 0, 0},  /*  81 */
    {C, 18, 3, N, 0, 0},  /*  82 */
    {C, 0, 4, N, 0, 0},  /*  83 */
    {C, 4, 4, N, 0, 0},  /*  84 */
    {C, 5, 4, N, 0, 0},  /*  85 */
    {C, 6, 4, N, 0, 0},  /*  86 */
    {C, 7, 4, N, 0, 0},  /*  87 */
    {C, 8, 4, N, 0, 0},  /*  88 */
    {C, 9, 4, N, 0, 0},  /*  89 */
    {C, 10, 4, N, 0, 0},  /*  90 */
    {C, 11, 4, N, 0, 0},  /*  91 */
    {C, 12, 4, N, 0, 0},  /*  92 */
    {C, 13, 4, N, 0, 0},  /*  93 */
    {C, 14, 4, N, 0, 0},  /*  94 */
    {C, 15, 4, N, 0, 0},  /*  95 */
    {C, 16, 4, N, 0, 0},  /*  96 */
    {C, 17, 4, N, 0, 0},  /*  97 */
    {C, 18, 4, N, 0, 0},  /*  98 */
    {C, 0, 5, N, 0, 0},  /*  99 */
    {C, 4, 5, N, 0, 0},  /* 100 */
    {C, 5, 5, N, 0, 0},  /* 101 */
    {C, 6, 5, N, 0, 0},  /* 102 */
    {C, 7, 5, N, 0, 0},  /* 103 */
    {C, 8, 5, N, 0, 0},  /* 104 */
    {C, 9, 5, N, 0, 0},  /* 105 */
    {C, 10, 5, N, 0, 0},  /* 106 */
    {C, 11, 5, N, 0, 0},  /* 107 */
    {C, 12, 5, N, 0, 0},  /* 108 */
    {C, 13, 5, N, 0, 0},  /* 109 */
    {C, 14, 5, N, 0, 0},  /* 110 */
    {C, 15, 5, N, 0, 0},  /* 111 */
    {C, 16, 5, N, 0, 0},  /* 112 */
    {C, 17, 5, N, 0, 0},  /* 113 */
    {C, 18, 5, N, 0, 0},  /* 114 */
    {C, 0, 6, N, 0, 0},  /* 115 */
    {C, 4, 6, N, 0, 0},  /* 116 */
    {C, 5, 6, N, 0, 0},  /* 117 */
    {C, 6, 6, N, 0, 0},  /* 118 */
    {C, 7, 6, N, 0, 0},  /* 119 */
    {C, 8, 6, N, 0, 0},  /* 120 */
    {C, 9, 6, N, 0, 0},  /* 121 */
    {C, 10, 6, N, 0, 0},  /* 122 */
    {C, 11, 6, N, 0, 0},  /* 123 */
    {C, 12, 6, N, 0, 0},  /* 124 */
    {C, 13, 6, N, 0, 0},  /* 125 */
    {C, 14, 6, N, 0, 0},  /* 126 */
    {C, 15, 6, N, 0, 0},  /* 127 */
    {C, 16, 6, N, 0, 0},  /* 128 */
    {C, 17, 6, N, 0, 0},  /* 129 */
    {C, 18, 6, N, 0, 0},  /* 130 */
    {C, 0, 7, N, 0, 0},  /* 131 */
    {C, 4, 7, N, 0, 0},  /* 132 */
    {C, 5, 7, N, 0, 0},  /* 133 */
    {C, 6, 7, N, 0, 0},  /* 134 */
    {C, 7, 7, N, 0, 0},  /* 135 */
    {C, 8, 7, N, 0, 0},  /* 136 */
    {C, 9, 7, N, 0, 0},  /* 137 */
    {C, 10, 7, N, 0, 0},  /* 138 */
    {C, 11, 7, N, 0, 0},  /* 139 */
    {C, 12, 7, N, 0, 0},  /* 140 */
    {C, 13, 7, N, 0, 0},  /* 141 */
    {C, 14, 7, N, 0, 0},  /* 142 */
    {C, 15, 7, N, 0, 0},  /* 143 */
    {C, 16, 7, N, 0, 0},  /* 144 */
    {C, 17, 7, N, 0, 0},  /* 145 */
    {C, 18, 7, N, 0, 0},  /* 146 */
    {C, 0, 8, N, 0, 0},  /* 147 */
    {C, 4, 8, N, 0, 0},  /* 148 */
    {C, 5, 8, N, 0, 0},  /* 149 */
    {C, 6, 8, N, 0, 0},  /* 150 */
    {C, 7, 8, N, 0, 0},  /* 151 */
    {C, 8, 8, N, 0, 0},  /* 152 */
    {C, 9, 8, N, 0, 0},  /* 153 */
    {C, 10, 8, N, 0, 0},  /* 154 */
    {C, 11, 8, N, 0, 0},  /* 155 */
    {C, 12, 8, N, 0, 0},  /* 156 */
    {C, 13, 8, N, 0, 0},  /* 157 */
    {C, 14, 8, N, 0, 0},  /* 158 */
    {C, 15, 8, N, 0, 0},  /* 159 */
    {C, 16, 8, N, 0, 0},  /* 160 */
    {C, 17, 8, N, 0, 0},  /* 161 */
    {C, 18, 8, N, 0, 0},  /* 162 */
    {A, 1, 0, C, 4, 0},  /* 163 */
    {A, 1, 0, C, 5, 0},  /* 164 */
    {A, 1, 0, C, 6, 0},  /* 165 */
    {A, 2, 0, C, 4, 0},  /* 166 */
    {A, 2, 0, C, 5, 0},  /* 167 */
    {A, 2, 0, C, 6, 0},  /* 168 */
    {A, 3, 0, C, 4, 0},  /* 169 */
    {A, 3, 0, C, 5, 0},  /* 170 */
    {A, 3, 0, C, 6, 0},  /* 171 */
    {A, 4, 0, C, 4, 0},  /* 172 */
    {A, 4, 0, C, 5, 0},  /* 173 */
    {A, 4, 0, C, 6, 0},  /* 174 */
    {A, 1, 0, C, 4, 1},  /* 175 */
    {A, 1, 0, C, 5, 1},  /* 176 */
    {A, 1, 0, C, 6, 1},  /* 177 */
    {A, 2, 0, C, 4, 1},  /* 178 */
    {A, 2, 0, C, 5, 1},  /* 179 */
    {A, 2, 0, C, 6, 1},  /* 180 */
    {A, 3, 0, C, 4, 1},  /* 181 */
    {A, 3, 0, C, 5, 1},  /* 182 */
    {A, 3, 0, C, 6, 1},  /* 183 */
    {A, 4, 0, C, 4, 1},  /* 184 */
    {A, 4, 0, C, 5, 1},  /* 185 */
    {A, 4, 0, C, 6, 1},  /* 186 */
    {A, 1, 0, C, 4, 2},  /* 187 */
    {A, 1, 0, C, 5, 2},  /* 188 */
    {A, 1, 0, C, 6, 2},  /* 189 */
    {A, 2, 0, C, 4, 2},  /* 190 */
    {A, 2, 0, C, 5, 2},  /* 191 */
    {A, 2, 0, C, 6, 2},  /* 192 */
    {A, 3, 0, C, 4, 2},  /* 193 */
    {A, 3, 0, C, 5, 2},  /* 194 */
    {A, 3, 0, C, 6, 2},  /* 195 */
    {A, 4, 0, C, 4, 2},  /* 196 */
    {A, 4, 0, C, 5, 2},  /* 197 */
    {A, 4, 0, C, 6, 2},  /* 198 */
    {A, 1, 0, C, 4, 3},  /* 199 */
    {A, 1, 0, C, 5, 3},  /* 200 */
    {A, 1, 0, C, 6, 3},  /* 201 */
    {A, 2, 0, C, 4, 3},  /* 202 */
    {A, 2, 0, C, 5, 3},  /* 203 */
    {A, 2, 0, C, 6, 3},  /* 204 */
    {A, 3, 0, C, 4, 3},  /* 205 */
    {A, 3, 0, C, 5, 3},  /* 206 */
    {A, 3, 0, C, 6, 3},  /* 207 */
    {A, 4, 0, C, 4, 3},  /* 208 */
    {A, 4, 0, C, 5, 3},  /* 209 */
    {A, 4, 0, C, 6, 3},  /* 210 */
    {A, 1, 0, C, 4, 4},  /* 211 */
    {A, 1, 0, C, 5, 4},  /* 212 */
    {A, 1, 0, C, 6, 4},  /* 213 */
    {A, 2, 0, C, 4, 4},  /* 214 */
    {A, 2, 0, C, 5, 4},  /* 215 */
    {A, 2, 0, C, 6, 4},  /* 216 */
    {A, 3, 0, C, 4, 4},  /* 217 */
    {A, 3, 0, C, 5, 4},  /* 218 */
    {A, 3, 0, C, 6, 4},  /* 219 */
    {A, 4, 0, C, 4, 4},  /* 220 */
    {A, 4, 0, C, 5, 4},  /* 221 */
    {A, 4, 0, C, 6, 4},  /* 222 */
    {A, 1, 0, C, 4, 5},  /* 223 */
    {A, 1, 0, C, 5, 5},  /* 224 */
    {A, 1, 0, C, 6, 5},  /* 225 */
    {A, 2, 0, C, 4, 5},  /* 226 */
    {A, 2, 0, C, 5, 5},  /* 227 */
    {A, 2, 0, C, 6, 5},  /* 228 */
    {A, 3, 0, C, 4, 5},  /* 229 */
    {A, 3, 0, C, 5, 5},  /* 230 */
    {A, 3, 0, C, 6, 5},  /* 231 */
    {A, 4, 0, C, 4, 5},  /* 232 */
    {A, 4, 0, C, 5, 5},  /* 233 */
    {A, 4, 0, C, 6, 5},  /* 234 */
    {A, 1, 0, C, 4, 6},  /* 235 */
    {A, 2, 0, C, 4, 6},  /* 236 */
    {A, 3, 0, C, 4, 6},  /* 237 */
    {A, 4, 0, C, 4, 6},  /* 238 */
    {A, 1, 0, C, 4, 7},  /* 239 */
    {A, 2, 0, C, 4, 7},  /* 240 */
    {A, 3, 0, C, 4, 7},  /* 241 */
    {A, 4, 0, C, 4, 7},  /* 242 */
    {A, 1, 0, C, 4, 8},  /* 243 */
    {A, 2, 0, C, 4, 8},  /* 244 */
    {A, 3, 0, C, 4, 8},  /* 245 */
    {A, 4, 0, C, 4, 8},  /* 246 */
    {C, 4, 0, A, 1, 0},  /* 247 */
    {C, 4, 1, A, 1, 0},  /* 248 */
    {C, 4, 2, A, 1, 0},  /* 249 */
    {C, 4, 3, A, 1, 0},  /* 250 */
    {C, 4, 4, A, 1, 0},  /* 251 */
    {C, 4, 5, A, 1, 0},  /* 252 */
    {C, 4, 6, A, 1, 0},  /* 253 */
    {C, 4, 7, A, 1, 0},  /* 254 */
    {C, 4, 8, A, 1, 0}   /* 255 */
};


#undef R
#undef A
#undef C
#undef N


// Data structure to search for opcodes.
/// TODO: Add docs.
typedef struct vcdiff_codetable_tree {
    char *first;
    char **second;
    uint16_t **opcodes;
    uint16_t default_opcodes[4];
    uint16_t *default_copy_instr;

    size_t *second_size;
    size_t first_size;
    char max_mode;
} vcdiff_codetable_tree;


/**
 * Convert the opcodes table into a codetable tree.
 * @param code_table Source opcodes table.
 * @return codetable tree.
 */
vcdiff_codetable_tree * vcdiff_new_codetable_tree(vcdiff_code_table_entry *code_table);


/**
 * Free the codetable tree.
 * @param codetable_tree Codetable tree.
 */
void vcdiff_free_codetable_tree(vcdiff_codetable_tree *tree);


/**
 * Search for the instruction pair in the tree.
 * @param tree Codetable tree.
 * @param encoded_first First instruction encoded as three bytes.
 * @param encoded_second Second instruction encoded as three bytes.
 * @param[out] opcode Resulted opcode.
 * @return Number of found instructions, 0 if default instructions are used.
 */
uint16_t vcdiff_find_instruction(
    vcdiff_codetable_tree *tree,
    const char *encoded_first,
    const char *encoded_second,
    uint16_t *opcode);


/**
 * Allocate memory for new instruction stream.
 * @param size Size of the stream.
 * @return New instruction stream.
 */
vcdiff_raw_instr * vcdiff_new_instruction_stream(const size_t size);


/**
 * Free instruction stream memory.
 * @param stream Instruction stream.
 * @param size Stream size.
 */
void vcdiff_free_instruction_stream(
    vcdiff_raw_instr *stream,
    const size_t size);


/**
 * Dump instruction to stdout (for debug).
 * @param table Code table.
 * @param opcode Current opcode.
 */
void dump_instruction(
    const vcdiff_code_table_entry *table,
    uint16_t opcode);


#endif // VCDIFF_DEFAULT_CODE_TABLE
