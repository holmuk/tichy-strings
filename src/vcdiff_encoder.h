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


/**
 * Encode instructions in the current window.
 * @param window Current window.
 * @param code_table Codetable tree.
 * @param stream Instructions stream.
 * @param stream_size Stream size.
 */
void vcdiff_encode_window_instructions(
    vcdiff_window *window,
    vcdiff_codetable_tree *code_table,
    vcdiff_raw_instr *stream,
    size_t stream_size);


/**
 * Encode the current block using the Tichy's algorithm.
 * @param handler IO handler.
 * @param vcdiff VCDIFF file handler.
 * @param[out] instructions_size Resulted array size.
 * @return Array of Vcdiff instructions that encode the current block.
 */
vcdiff_raw_instr * vcdiff_tichy_encode_block(
    vcdiff_io_handler *handler,
    vcdiff_file *vcdiff,
    size_t *instructions_size);

#endif // VCDIFF_ENCODER_H
