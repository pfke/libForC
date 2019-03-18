#include "mem/lfcMemPool.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcMemPool__lfcMemPool_alloc_moreBlocks

Test(
    TEST_SUITE_NAME,
    checkIf_1st_isGoodPtr
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 3 + ALIGN(1) * 3, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret1 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret2 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret3 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);

    should_be_same_ptr_wText(ret1, buf + sizeof(memPool_headr_t), "returned ptr not good");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_2nd_isGoodPtr
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 3 + ALIGN(1) * 3, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret1 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret2 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret3 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);

    should_be_same_ptr_wText(ret2, buf + sizeof(memPool_headr_t) * 2 + ALIGN(1), "returned ptr not good");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_3rd_isGoodPtr
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 3 + ALIGN(1) * 3, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret1 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret2 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret3 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);

    should_be_same_ptr_wText(ret3, buf + sizeof(memPool_headr_t) * 3 + ALIGN(1) * 2, "returned ptr not good");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_1st_header
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 3 + ALIGN(1) * 3, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret1 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret2 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret3 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);

    memPool_headr_t *hdr = (memPool_headr_t *)(ret1 - sizeof(memPool_headr_t));
    should_be_same_int_wText((unsigned)hdr->isUsed, 1, "not marked as used");
    should_be_same_int_wText((unsigned)hdr->len, ALIGN(1), "len not set");
    should_be_same_int_wText((unsigned)hdr->prev_len, 0, "prev-len not set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_2nd_header
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 3 + ALIGN(1) * 3, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret1 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret2 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret3 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);

    memPool_headr_t *hdr = (memPool_headr_t *)(ret2 - sizeof(memPool_headr_t));
    should_be_same_int_wText((unsigned)hdr->isUsed, 1, "not marked as used");
    should_be_same_int_wText((unsigned)hdr->len, ALIGN(1), "len not set");
    should_be_same_int_wText((unsigned)hdr->prev_len, ALIGN(1), "prev-len not set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_3rd_header
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 3 + ALIGN(1) * 3, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret1 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret2 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret3 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);

    memPool_headr_t *hdr = (memPool_headr_t *)(ret3 - sizeof(memPool_headr_t));
    should_be_same_int_wText((unsigned)hdr->isUsed, 1, "not marked as used");
    should_be_same_int_wText((unsigned)hdr->len, ALIGN(1), "len not set");
    should_be_same_int_wText((unsigned)hdr->prev_len, ALIGN(1), "prev-len not set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    no_space_for_new
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 1 + ALIGN(1) * 1, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret1 __attribute__((unused)) = lfcMemPool_alloc(tto, 2);
    char *ret2 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);

    should_be_null(ret2);
    memPool_headr_t *hdr = (memPool_headr_t *)(ret1 - sizeof(memPool_headr_t));
    should_be_same_int_wText((unsigned)hdr->isUsed, 1, "not marked as used");
    should_be_same_int_wText((unsigned)hdr->len, ALIGN(1), "len not set");
    should_be_same_int_wText((unsigned)hdr->prev_len, 0, "prev-len not set");

    delete(tto);
}
