#include "mem/lfcMemPool.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcMemPool__lfcMemPool_next

Test(
    TEST_SUITE_NAME,
    withoutHoles_iterateCount
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 4 + ALIGN(1) * 4, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret1 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret2 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret3 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret4 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);

    char foreach_counter = 0;

    ret1[0] = 1;
    ret2[0] = 2;
    ret3[0] = 3;
    ret4[0] = 4;

    void *current = NULL;
    while ((current = lfcMemPool_next(tto, current))) {
        foreach_counter++;
    }

    should_be_same_int_wText(foreach_counter, 4, "iterate 4 times");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    withoutHoles_iterateSum
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 4 + ALIGN(1) * 4, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret1 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret2 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret3 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret4 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);

    char foreach_result = 0;

    ret1[0] = 1;
    ret2[0] = 2;
    ret3[0] = 3;
    ret4[0] = 4;

    void *current = NULL;
    while ((current = lfcMemPool_next(tto, current))) {
        char *it = (char *)current;

        foreach_result += *it;
    };

    should_be_same_int_wText(foreach_result, 10, "iterate sum");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    withHoles_iterateCount
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 4 + ALIGN(1) * 4, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret1 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret2 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret3 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret4 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);

    char foreach_counter = 0;

    ret1[0] = 1;
    ret2[0] = 2;
    ret3[0] = 3;
    ret4[0] = 4;

    lfcMemPool_free(tto, ret1);
    lfcMemPool_free(tto, ret3);

    void *current = NULL;
    while ((current = lfcMemPool_next(tto, current))) {
        foreach_counter++;
    };

    should_be_same_int_wText(foreach_counter, 2, "iterate 4 times");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    withHoles_iterateSum
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 4 + ALIGN(1) * 4, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret1 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret2 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret3 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret4 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);

    char foreach_result = 0;

    ret1[0] = 1;
    ret2[0] = 2;
    ret3[0] = 3;
    ret4[0] = 4;

    lfcMemPool_free(tto, ret1);
    lfcMemPool_free(tto, ret3);

    void *current = NULL;
    while ((current = lfcMemPool_next(tto, current))) {
        char *it = (char *)current;
        foreach_result += *it;
    };

    should_be_same_int_wText(foreach_result, 6, "iterate 4 times");

    delete(tto);
}
