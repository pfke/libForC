#include "mem/lfcMemPool.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcMemPool__lfcMemPool_defrag

Test(
    TEST_SUITE_NAME,
    no_defrag_needed
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 4 + 4, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret1 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret2 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret3 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret4 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);

    *ret1 = 1;
    *ret2 = 2;
    *ret3 = 3;
    *ret4 = 4;

    should_be_same_int(*(char*)((tto->mem_ptr + ((sizeof(memPool_headr_t) + 1) * 0)) + sizeof(memPool_headr_t)), 1);
    should_be_same_int(*(char*)((tto->mem_ptr + ((sizeof(memPool_headr_t) + 1) * 1)) + sizeof(memPool_headr_t)), 2);
    should_be_same_int(*(char*)((tto->mem_ptr + ((sizeof(memPool_headr_t) + 1) * 2)) + sizeof(memPool_headr_t)), 3);
    should_be_same_int(*(char*)((tto->mem_ptr + ((sizeof(memPool_headr_t) + 1) * 3)) + sizeof(memPool_headr_t)), 4);

    should_be_same_int_wText(tto->used_block_count, 4, "used_block_count not correct");
    should_be_same_int_wText(tto->used_size, sizeof(memPool_headr_t) * 4 + 4, "used_size not correct");
    should_be_same_int_wText(tto->used_data_size, 4, "used_data_size not correct");
    should_be_same_int_wText(tto->highwatermark_used_size, sizeof(memPool_headr_t) * 4 + 4, "highwatermark_used_size not correct");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    defrag_needed
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 4 + 4, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret1 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret2 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret3 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret4 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);

    *ret1 = 1;
    *ret2 = 2;
    *ret3 = 3;
    *ret4 = 4;

    lfcMemPool_free(tto, ret2);

    should_be_same_int(lfcMemPool_defrag(tto), 0);
    should_be_same_int(*(char*)((tto->mem_ptr + ((sizeof(memPool_headr_t) + 1) * 0)) + sizeof(memPool_headr_t)), 1);
    should_be_same_int(*(char*)((tto->mem_ptr + ((sizeof(memPool_headr_t) + 1) * 1)) + sizeof(memPool_headr_t)), 3);
    should_be_same_int(*(char*)((tto->mem_ptr + ((sizeof(memPool_headr_t) + 1) * 2)) + sizeof(memPool_headr_t)), 4);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    defrag_needed_wFragmentation
) {
    CALLOC_STATIC_MEM(buf, (sizeof(memPool_headr_t) + 1) * 7, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret1 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret2 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret3 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret4 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret5 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret6 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret7 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);

    *ret1 = 1;
    *ret2 = 2;
    *ret3 = 3;
    *ret4 = 4;
    *ret5 = 5;
    *ret6 = 6;
    *ret7 = 7;

    lfcMemPool_free(tto, ret2);
    lfcMemPool_free(tto, ret5);
    lfcMemPool_free(tto, ret6);

    should_be_same_int(lfcMemPool_defrag(tto), 0);
    should_be_same_int(*(char*)((tto->mem_ptr + ((sizeof(memPool_headr_t) + 1) * 0)) + sizeof(memPool_headr_t)), 1);
    should_be_same_int(*(char*)((tto->mem_ptr + ((sizeof(memPool_headr_t) + 1) * 1)) + sizeof(memPool_headr_t)), 3);
    should_be_same_int(*(char*)((tto->mem_ptr + ((sizeof(memPool_headr_t) + 1) * 2)) + sizeof(memPool_headr_t)), 4);
    should_be_same_int(*(char*)((tto->mem_ptr + ((sizeof(memPool_headr_t) + 1) * 3)) + sizeof(memPool_headr_t)), 7);

    delete(tto);
}
