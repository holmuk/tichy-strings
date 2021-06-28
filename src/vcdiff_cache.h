#ifndef VCDIFF_CACHE_H
#define VCDIFF_CACHE_H

#include <stdio.h>
#include <stdlib.h>


enum vcdiff_copy_modes {
    VCDIFF_COPY_MODE_SELF = 0,
    VCDIFF_COPY_MODE_HERE = 1,
    VCDIFF_COPY_MODE_NEAR = 2,
    VCDIFF_COPY_MODE_SAME = 3,
    VCDIFF_COPY_MODE_INVALID = 4
};


enum vcdiff_cache_constants {
    VCDIFF_CACHE_DEFAULT_NEAR_SIZE = 4,
    VCDIFF_CACHE_DEFAULT_SAME_SIZE = 3
};


typedef struct {
    size_t *near;
    size_t near_cache_size;

    size_t *same;
    size_t same_cache_size;

    size_t next_slot;
} vcdiff_cache;


/**
 * Allocate memory for a new cache.
 * @param near_size Near cache size.
 * @param same_size Same cache size.
 * @return New cache object.
 */
vcdiff_cache * vcdiff_new_cache(size_t near_size, size_t same_size);


/**
 * Free memory occupied by the cache.
 * @param cache Cache object.
 */
void vcdiff_free_cache(vcdiff_cache *cache);


/**
 * Reallocated the cache.
 * @param cache Input cache.
 * @param near_size New near buffer size.
 * @param same_size New same buffer size.
 */
void vcdiff_realloc_cache(
    vcdiff_cache *cache,
    size_t near_size,
    size_t same_size);


/**
 * Update the current cache.
 * @param cache Cache object.
 * @param addr Address.
 */
void vcdiff_cache_update(vcdiff_cache *cache, size_t addr);


/**
 * Get copy instruction mode.
 * @param cache Cache object.
 * @param mode Copy instruction mode.
 * @return Copy instruction mode.
 */
int vcdiff_get_copy_instruction_mode(vcdiff_cache *cache, size_t mode);


/** Encode cache addr according to the algorithm described in the RFC.
 * @param cache Cache object.
 * @param addr_raw Raw address.
 * @param here Here address.
 * @param[out] copy_mode Calculated copy mode.
 * @return Calculated address.
 */
size_t vcdiff_cache_encode_addr(
    vcdiff_cache *cache,
    size_t addr_raw,
    size_t here,
    size_t *copy_mode);


#endif // VCDIFF_CACHE_H
