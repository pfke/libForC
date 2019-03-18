#include "mem/lfcMemPool.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcMemPool__lfcMemPool_calloc

Test(
    TEST_SUITE_NAME,
    passing_size_0
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 1 + 2, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    should_be_null(lfcMemPool_calloc(tto, 0));

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    needed_size_smaller_checkReturn_notNil
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 2 + 65, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    should_not_be_null(lfcMemPool_calloc(tto, 3));

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    needed_size_smaller_checkReturn_address
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 2 + 4, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret = lfcMemPool_calloc(tto, 3);

    should_be_same_ptr_wText(ret, buf + sizeof(memPool_headr_t), "return ptr inside buf");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    needed_size_smaller_returnUsedMarkedBlock
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 2 + 4, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret = lfcMemPool_calloc(tto, 2);
    unsigned used = (unsigned)(((memPool_headr_t *)(ret - sizeof(memPool_headr_t)))->isUsed);
    should_be_same_int_wText(used, 1, "not marked as used");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    needed_size_smaller_returnBlockLen
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 2 + ALIGN(4) * 2, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret = lfcMemPool_calloc(tto, 2);
    size_t len = (size_t)(((memPool_headr_t *)(ret - sizeof(memPool_headr_t)))->len);
    should_be_same_int_wText(len, ALIGN(2), "not marked as used");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    needed_size_smaller_returnZeroed
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 2 + 2, 0xff)
    CALLOC_STATIC_MEM(toCompare, 20, 0)

    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret = lfcMemPool_calloc(tto, 2);
    should_be_same_int_wText(memcmp(ret, toCompare, 2), 0, "block not zeroed");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    needed_size_smaller_checkNextFreeBlockMarked,
    .description = "Prüfen, ob der große Block geteilt wurde und der nächste als frei markiert wurde"
) {
    CALLOC_STATIC_MEM(buf, 20, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    lfcMemPool_calloc(tto, 3);

    should_be_same_int_wText((*(unsigned int *)(buf + 4 + ALIGN(3))) & 0x00000001, 0, "next block not marked as free");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    needed_size_smaller_checkNextFreeBlockLength,
    .description = "Prüfen, ob der große Block geteilt wurde und der nächste als frei markiert wurde"
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 2 + ALIGN(4) * 2, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    void *ptr = lfcMemPool_calloc(tto, 3);
    size_t len = (size_t)(((memPool_headr_t *)(ptr + ALIGN(3)))->len);
    should_be_same_int_wText(len, ALIGN(1), "next block length not updated");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    only_space_for_header_left_returnNotNil,
    .description = "Es wird soviel Platz angefordert, dass nur noch Restplatz für einen Header ist"
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 2 + 2, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    should_not_be_null(lfcMemPool_calloc(tto, 2));

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    only_space_for_header_left_returnCorrectAddress,
    .description = "Es wird soviel Platz angefordert, dass nur noch Restplatz für einen Header ist"
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 2 + 2, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    should_be_same_ptr_wText(lfcMemPool_calloc(tto, 2), buf + sizeof(memPool_headr_t), "incorrect address");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    only_space_for_header_left_returnMagicMarkedBlock,
    .description = "Es wird soviel Platz angefordert, dass nur noch Restplatz für einen Header ist"
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 2 + 2, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret = lfcMemPool_calloc(tto, 2);
    uint16_t magic = ((memPool_headr_t *)(ret - sizeof(memPool_headr_t)))->magic;
    should_be_same_int_wText(magic, LFCMEMPOOL_MAGIC, "not marked as used");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    only_space_for_header_left_returnUsedMarkedBlock,
    .description = "Es wird soviel Platz angefordert, dass nur noch Restplatz für einen Header ist"
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 2 + 2, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret = lfcMemPool_calloc(tto, 2);
    unsigned isUsed = ((memPool_headr_t *)(ret - sizeof(memPool_headr_t)))->isUsed;
    should_be_same_int_wText(isUsed, 1, "not marked as used");

    delete(tto);
}

/**
 * --single=lfcMemPool__lfcMemPool_calloc::only_space_for_header_left_returnBlockLen
 */
Test(
    TEST_SUITE_NAME,
    only_space_for_header_left_returnBlockLen,
    .description = "Es wird soviel Platz angefordert, dass nur noch Restplatz für einen Header ist -> volle Blocklänge returnen"
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 2 + 2, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret = lfcMemPool_calloc(tto, 2);
    unsigned len = ((memPool_headr_t *)(ret - sizeof(memPool_headr_t)))->len;
    should_be_same_int_wText(len, sizeof(memPool_headr_t) + 2, "not marked as used");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    only_space_for_header_left_returnZeroed
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 2 + 2, 0xff)
    CALLOC_STATIC_MEM(toCompare, 20, 0)

    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret = lfcMemPool_calloc(tto, 2);
    should_be_same_int_wText(memcmp(ret, toCompare, 2), 0, "block not zeroed");

    delete(tto);
}
