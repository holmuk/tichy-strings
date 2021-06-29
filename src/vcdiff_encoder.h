#ifndef VCDIFF_ENCODER_H
#define VCDIFF_ENCODER_H

#include <stdio.h>
#include <stdlib.h>

#include "tichy_strings.h"

#include "vcdiff.h"
#include "vcdiff_io.h"
#include "vcdiff_cache.h"
#include "vcdiff_window.h"
#include "vcdiff_code_table.h"


void vcdiff_encode_instr_run(
    vcdiff_raw_instr *stream,
    const char byte,
    const size_t size);


void vcdiff_encode_instr_add(
    vcdiff_raw_instr *stream,
    const size_t size,
    const char *data);


void vcdiff_encode_instr_copy(
    vcdiff_raw_instr *stream,
    vcdiff_file *file,
    size_t addr,
    size_t size);


void vcdiff_encode_window_instructions(
    vcdiff_window *window,
    vcdiff_codetable_tree *code_table,
    vcdiff_raw_instr *stream,
    size_t stream_size);


vcdiff_raw_instr * vcdiff_tichy_encode_block(
    vcdiff_io_handler *handler,
    vcdiff_file *vcdiff,
    size_t *instructions_size);

#endif // VCDIFF_ENCODER_H
