#include "mem/lfcMemPool.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcMemPool__lfcMemPool_free_moreBlocks

Test(
    TEST_SUITE_NAME,
    checkIf_1st_isFreed
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 4 + ALIGN(1) * 4, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret1 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret2 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret3 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret4 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);

    memPool_headr_t *hdr = (memPool_headr_t *)(ret1 - sizeof(memPool_headr_t));
    should_be_same_int_wText(lfcMemPool_free(tto, ret1), 0, "return value not 0");
    should_be_same_int_wText((unsigned)hdr->isUsed, 0, "not marked as unused");
    should_be_same_int_wText((unsigned)hdr->len, ALIGN(1), "len not set");
    should_be_same_int_wText((unsigned)hdr->prev_len, 0, "prev-len not set");

    should_be_same_int_wText(tto->used_block_count, 3, "used_block_count not correct");
    should_be_same_int_wText(tto->used_size, sizeof(memPool_headr_t) * 3 + ALIGN(1) * 3, "used_size not correct");
    should_be_same_int_wText(tto->used_data_size, ALIGN(1) * 3, "used_data_size not correct");
    should_be_same_int_wText(tto->highwatermark_used_size, sizeof(memPool_headr_t) * 4 + ALIGN(1) * 4, "highwatermark_used_size not correct");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_2nd_isFreed
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 4 + ALIGN(1) * 4, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret1 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret2 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret3 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret4 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);

    memPool_headr_t *hdr = (memPool_headr_t *)(ret2 - sizeof(memPool_headr_t));
    should_be_same_int_wText(lfcMemPool_free(tto, ret2), 0, "return value not 0");
    should_be_same_int_wText((unsigned)hdr->isUsed, 0, "not marked as unused");
    should_be_same_int_wText((unsigned)hdr->len, ALIGN(1), "len not set");
    should_be_same_int_wText((unsigned)hdr->prev_len, ALIGN(1) * 1, "prev-len not set");

    should_be_same_int_wText(tto->used_block_count, 3, "used_block_count not correct");
    should_be_same_int_wText(tto->used_size, sizeof(memPool_headr_t) * 3 + ALIGN(1) * 3, "used_size not correct");
    should_be_same_int_wText(tto->used_data_size, ALIGN(1) * 3, "used_data_size not correct");
    should_be_same_int_wText(tto->highwatermark_used_size, sizeof(memPool_headr_t) * 4 + ALIGN(1) * 4, "highwatermark_used_size not correct");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_3rd_freeAfter_isFreed
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 3 + ALIGN(1) * 3, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret1 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret2 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret3 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);

    memPool_headr_t *hdr = (memPool_headr_t *)(ret3 - sizeof(memPool_headr_t));
    should_be_same_int_wText(lfcMemPool_free(tto, ret3), 0, "return value not 0");
    should_be_same_int_wText((unsigned)hdr->isUsed, 0, "not marked as unused");
    should_be_same_int_wText((unsigned)hdr->len, ALIGN(1), "len not set");
    should_be_same_int_wText((unsigned)hdr->prev_len, ALIGN(1), "prev-len not set");

    should_be_same_int_wText(tto->used_block_count, 2, "used_block_count not correct");
    should_be_same_int_wText(tto->used_size, sizeof(memPool_headr_t) * 2 + ALIGN(1) * 2, "used_size not correct");
    should_be_same_int_wText(tto->used_data_size, ALIGN(1) * 2, "used_data_size not correct");
    should_be_same_int_wText(tto->highwatermark_used_size, sizeof(memPool_headr_t) * 3 + ALIGN(1) * 3, "highwatermark_used_size not correct");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_3rd_used_isFreed
) {
    CALLOC_STATIC_MEM(buf, 200, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret1 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret2 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret3 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret4 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);

    memPool_headr_t *hdr = (memPool_headr_t *)(ret3 - sizeof(memPool_headr_t));
    should_be_same_int_wText(lfcMemPool_free(tto, ret3), 0, "return value not 0");
    should_be_same_int_wText((unsigned)hdr->isUsed, 0, "not marked as unused");
    should_be_same_int_wText((unsigned)hdr->len, ALIGN(1), "len not set");
    should_be_same_int_wText((unsigned)hdr->prev_len, ALIGN(1), "prev-len not set");

    should_be_same_int_wText(tto->used_block_count, 3, "used_block_count not correct");
    should_be_same_int_wText(tto->used_size, sizeof(memPool_headr_t) * 3 + ALIGN(1) * 3, "used_size not correct");
    should_be_same_int_wText(tto->used_data_size, ALIGN(1) * 3, "used_data_size not correct");
    should_be_same_int_wText(tto->highwatermark_used_size, sizeof(memPool_headr_t) * 4 + ALIGN(1) * 4, "highwatermark_used_size not correct");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_4th_isFreed
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 4 + ALIGN(1) * 4, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret1 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret2 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret3 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret4 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);

    memPool_headr_t *hdr = (memPool_headr_t *)(ret4 - sizeof(memPool_headr_t));
    should_be_same_int_wText(lfcMemPool_free(tto, ret4), 0, "return value not 0");
    should_be_same_int_wText((unsigned)hdr->isUsed, 0, "not marked as unused");
    should_be_same_int_wText((unsigned)hdr->len, ALIGN(1), "len not set");
    should_be_same_int_wText((unsigned)hdr->prev_len, ALIGN(1), "prev-len not set");

    should_be_same_int_wText(tto->used_block_count, 3, "used_block_count not correct");
    should_be_same_int_wText(tto->used_size, sizeof(memPool_headr_t) * 3 + ALIGN(1) * 3, "used_size not correct");
    should_be_same_int_wText(tto->used_data_size, ALIGN(1) * 3, "used_data_size not correct");
    should_be_same_int_wText(tto->highwatermark_used_size, sizeof(memPool_headr_t) * 4 + ALIGN(1) * 4, "highwatermark_used_size not correct");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIfPrevIsFree
) {
    CALLOC_STATIC_MEM(buf, sizeof(memPool_headr_t) * 4 + ALIGN(4) * 4, 0xff)
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret1 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret2 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret3 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);
    char *ret4 __attribute__((unused)) = lfcMemPool_alloc(tto, 1);

    lfcMemPool_free(tto, ret2);

    memPool_headr_t *hdr = (memPool_headr_t *)(ret2 - sizeof(memPool_headr_t));
    should_be_same_int_wText(lfcMemPool_free(tto, ret3), 0, "return value not 0");
    should_be_same_int_wText((unsigned)hdr->isUsed, 0, "not marked as unused");
    should_be_same_int_wText((unsigned)hdr->len, sizeof(memPool_headr_t) + ALIGN(1) * 2, "len not set");
    should_be_same_int_wText((unsigned)hdr->prev_len, ALIGN(1), "prev-len not set");

    should_be_same_int_wText(tto->used_block_count, 2, "used_block_count not correct");
    should_be_same_int_wText(tto->used_size, sizeof(memPool_headr_t) * 2 + ALIGN(1) * 2, "used_size not correct");
    should_be_same_int_wText(tto->used_data_size, ALIGN(1) * 2, "used_data_size not correct");
    should_be_same_int_wText(tto->highwatermark_used_size, sizeof(memPool_headr_t) * 4 + ALIGN(1) * 4, "highwatermark_used_size not correct");

    delete(tto);
}
