#include "mem/lfcMemPool.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcMemPool__lfcMemPool_alloc_oneBlock

Test(
    TEST_SUITE_NAME,
    passing_size_0
) {
    CALLOC_STATIC_MEM(buf, (sizeof(memPool_headr_t) * 1) + ALIGN(5), 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    should_be_null(lfcMemPool_alloc(tto, 0));

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    alloc_valid_check_return_non_null,
    .description = "einen Block alloziieren der kleiner als die maximale Größe ist"
) {
    CALLOC_STATIC_MEM(buf, (sizeof(memPool_headr_t) * 1) + ALIGN(50), 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    should_not_be_null(lfcMemPool_alloc(tto, 3));

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    alloc_valid_check_address_within_mem
) {
    CALLOC_STATIC_MEM(buf, (sizeof(memPool_headr_t) * 1) + ALIGN(5), 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret = lfcMemPool_alloc(tto, 3);

    should_be_same_ptr_wText(ret, buf + sizeof(memPool_headr_t), "return ptr inside buf");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    alloc_valid_check_field_magic
) {
    CALLOC_STATIC_MEM(buf, (sizeof(memPool_headr_t) * 1) + ALIGN(5), 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret = lfcMemPool_alloc(tto, 2);
    uint16_t magic = ((memPool_headr_t *)(ret - sizeof(memPool_headr_t)))->magic;
    should_be_same_int_wText(magic, LFCMEMPOOL_MAGIC, "magic not set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    alloc_valid_check_field_isUsed
) {
    CALLOC_STATIC_MEM(buf, (sizeof(memPool_headr_t) * 1) + ALIGN(5), 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret = lfcMemPool_alloc(tto, 2);
    unsigned isUsed = ((memPool_headr_t *)(ret - sizeof(memPool_headr_t)))->isUsed;
    should_be_same_int_wText(isUsed, 1, "not marked as used");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    alloc_valid_check_field_len,
    .description = "Nur Platz für einen Block"
) {
    CALLOC_STATIC_MEM(buf, (sizeof(memPool_headr_t) * 1) + ALIGN(5), 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret = lfcMemPool_alloc(tto, 2);
    unsigned len = ((memPool_headr_t *)(ret - sizeof(memPool_headr_t)))->len;
    should_be_same_int_wText(len, ALIGN(5), "block len not written");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    alloc_valid_check_field_prev_len
) {
    CALLOC_STATIC_MEM(buf, (sizeof(memPool_headr_t) * 1) + ALIGN(5), 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret = lfcMemPool_alloc(tto, 2);
    unsigned len = ((memPool_headr_t *)(ret - sizeof(memPool_headr_t)))->prev_len;
    should_be_same_int_wText(len, 0, "prev block len not written");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_nextHdr_magic
) {
    CALLOC_STATIC_MEM(buf, (sizeof(memPool_headr_t) * 2) + ALIGN(5) * 5, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret = lfcMemPool_alloc(tto, 3);

    uint16_t magic = ((memPool_headr_t *)(ret + ALIGN(3)))->magic;
    should_be_same_int(magic, LFCMEMPOOL_MAGIC);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_nextHdr_isUsed,
    .description = "Prüfen, ob der große Block geteilt wurde und der nächste als frei markiert wurde"
) {
    CALLOC_STATIC_MEM(buf, (sizeof(memPool_headr_t) * 2) + ALIGN(5) * 5, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret = lfcMemPool_alloc(tto, 3);

    unsigned isUsed = ((memPool_headr_t *)(ret + ALIGN(3)))->isUsed;
    should_be_same_int_wText(isUsed, 0, "next block not marked as free");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_nextHdr_len
) {
    CALLOC_STATIC_MEM(buf, (sizeof(memPool_headr_t) * 2) + ALIGN(4) * 5, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret = lfcMemPool_alloc(tto, 3);
    unsigned len = ((memPool_headr_t *)(ret + ALIGN(3)))->len;
    should_be_same_int_wText(len, ALIGN(4) * 4, "next block length not updated");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_nextHdr_prev_len
) {
    CALLOC_STATIC_MEM(buf, (sizeof(memPool_headr_t) * 2) + ALIGN(5)*5, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret = lfcMemPool_alloc(tto, 3);
    unsigned len = ((memPool_headr_t *)(ret + ALIGN(3)))->prev_len;
    should_be_same_int_wText(len, ALIGN(3), "next block prev length not updated");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    only_space_for_header_left_returnNotNil,
    .description = "Es wird soviel Platz angefordert, dass nur noch Restplatz für einen Header ist"
) {
    CALLOC_STATIC_MEM(buf, (sizeof(memPool_headr_t) * 2) + ALIGN(20), 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    should_not_be_null(lfcMemPool_alloc(tto, ALIGN(20)));

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    only_space_for_header_left_returnCorrectAddress,
    .description = "Es wird soviel Platz angefordert, dass nur noch Restplatz für einen Header ist"
) {
    CALLOC_STATIC_MEM(buf, (sizeof(memPool_headr_t) * 2) + ALIGN(20), 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    should_be_same_ptr_wText(lfcMemPool_alloc(tto, ALIGN(20)), buf + sizeof(memPool_headr_t), "incorrect address");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    only_space_for_header_left_returnUsedMarkedBlock,
    .description = "Es wird soviel Platz angefordert, dass nur noch Restplatz für einen Header ist"
) {
    CALLOC_STATIC_MEM(buf, (sizeof(memPool_headr_t) * 2) + ALIGN(20), 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret = lfcMemPool_alloc(tto, ALIGN(20));
    unsigned isUsed = (*((memPool_headr_t *)(ret - sizeof(memPool_headr_t)))).isUsed;
    should_be_same_int_wText(isUsed, 1, "not marked as used");

    delete(tto);
}

/**
 * --single=lfcMemPool__lfcMemPool_alloc_oneBlock::only_space_for_header_left_returnBlockLen
 */
Test(
    TEST_SUITE_NAME,
    only_space_for_header_left_returnBlockLen,
    .description = "Es wird soviel Platz angefordert, dass nur noch Restplatz für einen Header ist -> volle Blocklänge returnen"
) {
    CALLOC_STATIC_MEM(buf, (sizeof(memPool_headr_t) * 2) + ALIGN(20), 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret = lfcMemPool_alloc(tto, ALIGN(20));
    unsigned len = (*((memPool_headr_t *)(ret - sizeof(memPool_headr_t)))).len;
    should_be_same_int_wText(len, ALIGN(20) + sizeof(memPool_headr_t), "not marked as used");

    delete(tto);
}
