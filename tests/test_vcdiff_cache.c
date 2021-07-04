#include <stdlib.h>
#include <stdint.h>
#include <check.h>

#include "vcdiff_cache.h"


typedef struct {
    size_t addr_raw;
    size_t expected_addr;
    size_t expected_mode;
} encode_addr_test_entry;


static const encode_addr_test_entry encode_addr_default_cache_test_data[6] = {
    { 0x0000FFFF, 0x0000FFFF, 0 },
    { 0x10000000, 0x0FFF0001, 2 },
    { 0x10000001, 0x00000001, 4 },
    { 0x00010002, 0x00010002, 0 },
    { 0x0000FFFF, 0x000000FF, 6 },
    { 0x00010002, 0x00000002, 7 }
};


/**
 * Zero-sized cache fixture.
 */
vcdiff_cache *empty_cache;


/**
 * Default cache fixture.
 */
vcdiff_cache *default_cache;


void init_empty_cache()
{
    empty_cache = vcdiff_new_cache(0, 0);
}


void free_empty_cache()
{
    vcdiff_free_cache(empty_cache);
}


void init_default_cache()
{
    default_cache = vcdiff_new_cache(
        VCDIFF_CACHE_DEFAULT_NEAR_SIZE,
        VCDIFF_CACHE_DEFAULT_SAME_SIZE);
}


void free_default_cache()
{
    vcdiff_free_cache(default_cache);
}


/**
 * Valgrind test for cache allocation.
 */
START_TEST(test_vcdiff_cache_alloc_zero)
{
    vcdiff_cache *cache = vcdiff_new_cache(0, 0);

    ck_assert_ptr_nonnull(cache);

    ck_assert_ptr_null(cache->near);
    ck_assert_ptr_null(cache->same);

    ck_assert_uint_eq(cache->next_slot, 0);
    ck_assert_uint_eq(cache->near_cache_size, 0);
    ck_assert_uint_eq(cache->same_cache_size, 0);

    vcdiff_free_cache(cache);
}
END_TEST


/**
 * Valgrind test for cache allocation.
 */
START_TEST(test_vcdiff_cache_alloc_nonzero)
{
    size_t near_size = 12;
    size_t same_size = 16;

    vcdiff_cache *cache = vcdiff_new_cache(near_size, same_size);

    ck_assert_ptr_nonnull(cache);
    ck_assert_ptr_nonnull(cache->near);
    ck_assert_ptr_nonnull(cache->same);

    ck_assert_uint_eq(cache->next_slot, 0);
    ck_assert_uint_eq(cache->near_cache_size, near_size);
    ck_assert_uint_eq(cache->same_cache_size, same_size);

    vcdiff_free_cache(cache);
}
END_TEST


/**
 * Valgrind test for cache reallocation.
 */
START_TEST(test_vcdiff_cache_realloc)
{
    vcdiff_cache *cache = vcdiff_new_cache(
        VCDIFF_CACHE_DEFAULT_NEAR_SIZE,
        VCDIFF_CACHE_DEFAULT_SAME_SIZE);

    ck_assert_ptr_nonnull(cache);

    vcdiff_realloc_cache(cache, 13, 16);
    ck_assert_ptr_nonnull(cache);

    vcdiff_realloc_cache(cache, 0, 1);
    ck_assert_ptr_nonnull(cache);

    vcdiff_realloc_cache(
        cache,
        VCDIFF_CACHE_DEFAULT_NEAR_SIZE,
        VCDIFF_CACHE_DEFAULT_SAME_SIZE);

    ck_assert_ptr_nonnull(cache);
    ck_assert_ptr_nonnull(cache->near);
    ck_assert_ptr_nonnull(cache->same);

    vcdiff_free_cache(cache);
}
END_TEST


/**
 * Address encoding test for default-sized cache.
 */
START_TEST(test_vcdiff_cache_encode_addr_default)
{
    size_t mode;
    size_t test_size = sizeof(encode_addr_default_cache_test_data) /
                       sizeof(encode_addr_test_entry);

    for (size_t i = 0; i < test_size; ++i) {
        size_t addr_raw = encode_addr_default_cache_test_data[i].addr_raw;
        size_t addr = vcdiff_cache_encode_addr(
            default_cache,
            addr_raw,
            &mode);

        vcdiff_cache_update(default_cache, addr_raw);

        ck_assert_uint_eq(
            mode, encode_addr_default_cache_test_data[i].expected_mode);
        ck_assert_uint_eq(
            addr, encode_addr_default_cache_test_data[i].expected_addr);
    }
}
END_TEST


Suite * vcdiff_cache_suite()
{
    Suite *s = suite_create("vcdiff_cache");

    TCase *tc_memory = tcase_create("Memory allocation");

    tcase_add_test(tc_memory, test_vcdiff_cache_alloc_zero);
    tcase_add_test(tc_memory, test_vcdiff_cache_alloc_nonzero);
    tcase_add_test(tc_memory, test_vcdiff_cache_realloc);

    TCase *tc_encode_default = tcase_create("Encoding (default-sized cache)");

    tcase_add_unchecked_fixture(
        tc_encode_default, init_default_cache, free_default_cache);
    tcase_add_test(tc_encode_default, test_vcdiff_cache_encode_addr_default);

    suite_add_tcase(s, tc_memory);
    suite_add_tcase(s, tc_encode_default);

    return s;
}
