#ifndef VCDIFF_WINDOW_H
#define VCDIFF_WINDOW_H

#include <stdlib.h>

#include "vcdiff_cache.h"
#include "vcdiff_io.h"


enum vcdiff_window_constants {
    VCDIFF_WINDOW_DEFAULT_DATA_SECTION_SIZE = 128,
    VCDIFF_WINDOW_DEFAULT_INSTR_SECTION_SIZE = 128,
    VCDIFF_WINDOW_DEFAULT_ADDR_SECTION_SIZE = 128
};


typedef struct {
    size_t source_segment_size;
    size_t source_segment_pos;
    char *source_segment;

    size_t win_indicator;
    size_t delta_indicator;

    char *data_section;
    size_t data_section_size;
    char *data_ptr;

    char *instr_section;
    size_t instr_section_size;
    char *instr_ptr;

    char *addr_section;
    size_t addr_section_size;
    char *addr_ptr;

    size_t target_len;
    size_t total_delta_size;
    size_t here;
} vcdiff_window;


/** Allocate memory for a window.
 *
 * Also set the window indicator to VCDIFF_SOURCE.
 *
 * @param data_section_size
 * @param instr_section_size
 * @param addr_section_size
 * @return Pointer to the window.
 */
vcdiff_window * vcdiff_new_window(
    size_t data_section_size,
    size_t instr_section_size,
    size_t addr_section_size
    );


/** Free used memory.
 * @param win Window to be freed.
 */
void vcdiff_free_window(vcdiff_window *win);


/** Reuse memory used by window.
 *
 * If any section requires memory expansion, realloc() is called.
 * If not, the pointers are reset back to the starts of corresponding
 * buffers.
 *
 * @param win
 * @param data_size
 * @param instr_size
 * @param addr_size
 */
void vcdiff_realloc_window(
    vcdiff_window *win,
    size_t data_size,
    size_t instr_size,
    size_t addr_size);


/**
 * Reset pointers to data, instr and addr sections.
 * @param win
 */
void vcdiff_reset_window_pointers(vcdiff_window *win);


/**
 * Write the window to IO handler.
 * @param handler
 * @param win
 * @return
 */
size_t vcdiff_write_window(
    vcdiff_io_handler *handler,
    vcdiff_window *win);


/**
 * Dump window's context to stdout (used for debugging).
 * @param win
 */
void vcdiff_dump_window(vcdiff_window *win);


#endif // VCDIFF_WINDOW_H
