#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "vcdiff.h"
#include "vcdiff_window.h"
#include "vcdiff_cache.h"
#include "vcdiff_io.h"


vcdiff_window * vcdiff_new_window(
    size_t data_section_size,
    size_t instr_section_size,
    size_t addr_section_size)
{
    vcdiff_window *win = (vcdiff_window *)calloc(1, sizeof(vcdiff_window));
    vcdiff_realloc_window(win,
                          data_section_size,
                          instr_section_size,
                          addr_section_size);

    win->win_indicator = VCDIFF_SOURCE;

    return win;
}


void vcdiff_free_window(vcdiff_window *win)
{
    if (win == NULL) {
        return;
    }

    if (win->data_section != NULL) {
        free(win->data_section);
    }

    if (win->instr_section != NULL) {
        free(win->instr_section);
    }

    if (win->addr_section != NULL) {
        free(win->addr_section);
    }

    if (win->source_segment != NULL) {
        free(win->source_segment);
    }

    free(win);
}


static void * realloc_with_check(void *ptr, size_t size)
{
    if (size == 0 && ptr == NULL) {
        return NULL; // UB for realloc?
    }

    return realloc(ptr, size);
}


void vcdiff_realloc_window(
    vcdiff_window *win,
    size_t data_size,
    size_t instr_size,
    size_t addr_size)
{
    assert(win != NULL);

    win->data_section = (char *)realloc_with_check(win->data_section, data_size);
    win->data_section_size = data_size;

    win->instr_section = (char *)realloc_with_check(win->instr_section, instr_size);
    win->instr_section_size = instr_size;

    win->addr_section = (char *)realloc_with_check(win->addr_section, addr_size);
    win->addr_section_size = addr_size;

    vcdiff_reset_window_pointers(win);
}


void vcdiff_reset_window_pointers(vcdiff_window *win)
{
    win->data_ptr = win->data_section;
    win->instr_ptr = win->instr_section;
    win->addr_ptr = win->addr_section;

    win->here = 0;
}


size_t vcdiff_write_window(
    vcdiff_io_handler *handler,
    vcdiff_window *win)
{
    char *header_data = malloc(40);
    char *delta_header_data = malloc(20);

    char *header_ptr = delta_header_data;
    size_t size;

    win->total_delta_size = 0;
    win->total_delta_size += win->data_section_size;
    win->total_delta_size += win->instr_section_size;
    win->total_delta_size += win->addr_section_size;

    header_ptr += vcdiff_write_integer_buffer(header_ptr, win->target_len);
    *header_ptr++ = win->delta_indicator;
    header_ptr += vcdiff_write_integer_buffer(header_ptr, win->data_section_size);
    header_ptr += vcdiff_write_integer_buffer(header_ptr, win->instr_section_size);
    header_ptr += vcdiff_write_integer_buffer(header_ptr, win->addr_section_size);

    size_t delta_data_size = header_ptr - delta_header_data;
    win->total_delta_size += delta_data_size;

    header_ptr = header_data;

    *header_ptr++ = win->win_indicator;

    if (win->win_indicator != 0) {
        header_ptr += vcdiff_write_integer_buffer(header_ptr, win->source_segment_size);
        header_ptr += vcdiff_write_integer_buffer(header_ptr, win->source_segment_pos);
    }

    header_ptr += vcdiff_write_integer_buffer(header_ptr, win->total_delta_size);

    memcpy(header_ptr, delta_header_data, delta_data_size);

    size = header_ptr + delta_data_size - header_data;
    size = vcdiff_write_bytes(handler, header_data, size);
    size += vcdiff_write_bytes(handler, win->data_section, win->data_section_size);
    size += vcdiff_write_bytes(handler, win->instr_section, win->instr_section_size);
    size += vcdiff_write_bytes(handler, win->addr_section, win->addr_section_size);

    free(header_data);
    free(delta_header_data);

    return size;
}


void vcdiff_dump_window(vcdiff_window *win)
{
    printf("Window indicator: %s\n",
           win->win_indicator == VCDIFF_SOURCE ? "VCDIFF_SOURCE" :
           win->win_indicator == VCDIFF_TARGET ? "VCDIFF_TARGET" : "UNKNOWN");

    printf("Delta indicator: %s\n",
           win->delta_indicator == 0 ? "NONE" :
           win->delta_indicator &VCDIFF_DATACOMP ? "VCDIFF_DATACOMP" :
           win->delta_indicator &VCDIFF_ADDRCOMP ? "VCDIFF_ADDRCOMP" :
           win->delta_indicator &VCDIFF_INSTCOMP ? "VCDIFF_INSTCOMP" : "UNKNOWN");

    printf("Target length: %zu\n", win->target_len);
    printf("Data section size: %zu\n", win->data_section_size);
    printf("Instructions section size: %zu\n", win->instr_section_size);
    printf("Address section size: %zu\n", win->addr_section_size);
    printf("Source segment size %zu\n", win->source_segment_size);
    printf("Source segment position: %zu\n", win->source_segment_pos);

    printf("Data: ");
    for (size_t i = 0; i < win->data_section_size; i++) {
        printf("%02hhX ", win->data_section[i]);
    }
    printf("\n");

    printf("Instr: ");
    for (size_t i = 0; i < win->instr_section_size; i++) {
        printf("%02hhX ", win->instr_section[i]);
    }
    printf("\n");

    printf("Addr: ");
    for (size_t i = 0; i < win->addr_section_size; i++) {
        printf("%02hhX ", win->addr_section[i]);
    }
    printf("\n");
}
