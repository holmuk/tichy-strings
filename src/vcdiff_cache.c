#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "vcdiff_cache.h"


vcdiff_cache * vcdiff_new_cache(size_t near_size, size_t same_size)
{
    vcdiff_cache *cache = malloc(sizeof(vcdiff_cache));
    cache->near = NULL;
    cache->same = NULL;

    vcdiff_realloc_cache(cache, near_size, same_size);

    return cache;
}


void vcdiff_free_cache(vcdiff_cache *cache)
{
    if (cache == NULL) {
        return;
    }

    if (cache->near != NULL) {
        free(cache->near);
    }

    if (cache->same != NULL) {
        free(cache->same);
    }

    free(cache);
}


void vcdiff_realloc_cache(
    vcdiff_cache *cache,
    size_t near_size,
    size_t same_size)
{
    assert(cache != NULL);

    cache->near_cache_size = near_size;
    cache->same_cache_size = same_size;

    cache->next_slot = 0;

    if (cache->near != NULL) {
        free(cache->near);
    }

    if (cache->same != NULL) {
        free(cache->same);
    }

    if (cache->near_cache_size != 0) {
        cache->near = (size_t *)calloc(cache->near_cache_size, sizeof(size_t));
    } else {
        cache->near = NULL;
    }

    if (cache->same_cache_size != 0) {
        cache->same = (size_t *)calloc(cache->same_cache_size, sizeof(size_t) * 256);
    } else {
        cache->same = NULL;
    }
}


void vcdiff_cache_update(vcdiff_cache *cache, size_t addr)
{
    if (cache->near != NULL) {
        cache->near[cache->next_slot] = addr;
        cache->next_slot = (cache->next_slot + 1) % cache->near_cache_size;
    }

    if (cache->same != NULL) {
        cache->same[addr % (cache->same_cache_size * 256)] = addr;
    }
}


/*
 *
 *     RFC:
 *
 *    VCD_SELF: This mode has value 0.  The address was encoded by
 *    itself as an integer.
 *
 *    VCD_HERE: This mode has value 1.  The address was encoded as the
 *    integer value "here - addr".
 *
 *    Near modes: The "near modes" are in the range [2,s_near+1].  Let m
 *    be the mode of the address encoding.  The address was encoded
 *    as the integer value "addr - near[m-2]".
 *
 *    Same modes: The "same modes" are in the range
 *    [s_near+2,s_near+s_same+1].  Let m be the mode of the encoding.
 *    The address was encoded as a single byte b such that "addr ==
 *    same[(m - (s_near+2))*256 + b]".
 */

int vcdiff_get_copy_instruction_mode(vcdiff_cache *cache, size_t mode)
{
    if (mode < VCDIFF_COPY_MODE_NEAR) {
        return mode;
    }

    if (mode < cache->near_cache_size + 2) {
        return VCDIFF_COPY_MODE_NEAR;
    }

    return VCDIFF_COPY_MODE_SAME;
}


size_t vcdiff_cache_encode_addr(
    vcdiff_cache *cache,
    size_t addr_raw,
    size_t here,
    size_t *copy_mode)
{
    int64_t addr = addr_raw;
    unsigned char mode = VCDIFF_COPY_MODE_SELF;
    int64_t tmp = here - addr;

    if (tmp > 0 && tmp < addr) {
        addr = tmp;
        mode = VCDIFF_COPY_MODE_HERE;
    }

    // It works even if cache->near is NULL (as cache->near_cache_size is 0)
    for (size_t i = 0; i < cache->near_cache_size; ++i) {
        tmp = addr_raw - cache->near[i];
        if (tmp >= 0 && tmp < addr) {
            addr = tmp;
            mode = i + 2;
        }
    }

    if (cache->same != NULL) {
        tmp = addr_raw % (cache->same_cache_size * 256);
        if (cache->same[tmp] == addr_raw) {
            addr = addr_raw % 256;
            mode = cache->near_cache_size + 2 + tmp / 256;
        }
    }

    *copy_mode = mode;
    return addr;
}
