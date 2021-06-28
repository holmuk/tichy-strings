#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "vcdiff.h"
#include "vcdiff_code_table.h"
#include "vcdiff_cache.h"
#include "vcdiff_window.h"


vcdiff_file * vcdiff_new_file()
{
    vcdiff_file *file = malloc(sizeof(vcdiff_file));
    file->version = VCDIFF_VERSION_RFC;

    file->current_window = vcdiff_new_window(
        VCDIFF_WINDOW_DEFAULT_DATA_SECTION_SIZE,
        VCDIFF_WINDOW_DEFAULT_INSTR_SECTION_SIZE,
        VCDIFF_WINDOW_DEFAULT_ADDR_SECTION_SIZE);

    file->cache = vcdiff_new_cache(
        VCDIFF_CACHE_DEFAULT_NEAR_SIZE,
        VCDIFF_CACHE_DEFAULT_SAME_SIZE);

    file->code_tree = vcdiff_new_codetable_tree(vcdiff_default_instr_table);

    file->use_default_code_table = true;

    return file;
}


void vcdiff_free_file(vcdiff_file *file)
{
    if (file == NULL) {
        return;
    }

    vcdiff_free_cache(file->cache);
    vcdiff_free_window(file->current_window);
    vcdiff_free_codetable_tree(file->code_tree);

    free(file);
}
