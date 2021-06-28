#ifndef VCDIFF_H
#define VCDIFF_H

#include <stdio.h>
#include <stdlib.h>


static const char VCDIFF_VERSION_RFC = 0x00;
static const char VCDIFF_HEADER[] = {0xD6, 0xC3, 0xC4, 0x00};


struct vcdiff_cache;
struct vcdiff_window;
struct vcdiff_codetable_tree;


enum vcdiff_constants {
    // Hdr_Indicator flags
    VCDIFF_DECOMPRESS = 0x01,
    VCDIFF_CODETABLE = 0x02,

    // Win_Indicator flags
    VCDIFF_SOURCE = 0x01,
    VCDIFF_TARGET = 0x02,

    // Delta_Indicator flags
    VCDIFF_DATACOMP = 0x01,
    VCDIFF_INSTCOMP = 0x02,
    VCDIFF_ADDRCOMP = 0x04,

    // Constants
    VCDIFF_MAX_INTEGER_32 = 0x7FFFFFFF,
    VCDIFF_MAX_INTEGER_SIZE = 4
};


typedef struct _vcdiff_file {
    char version;

    struct vcdiff_cache *cache;
    struct vcdiff_window *current_window;

    struct vcdiff_codetable_tree *code_tree;

    int use_default_code_table;
} vcdiff_file;


/**
 * Allocate memory for a file.
 * @return Vcdiff file.
 */
vcdiff_file * vcdiff_new_file();


/**
 * Free the memory.
 * @param file Vcdiff file.
 */
void vcdiff_free_file(vcdiff_file *file);


#endif // VCDIFF_H
