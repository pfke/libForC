#include "mem/lfcMemPool.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcMemPool__lfcMemPool_ctor

Test(
    TEST_SUITE_NAME,
    passing_null
) {
    lfcMemPool_t *tto = lfcMemPool_ctor(NULL, 0, memPool_WRITE_W_INITIALIZE);

    should_be_null(tto);
}

Test(
    TEST_SUITE_NAME,
    passing_size_0
) {
    char buf[10];
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, 0, memPool_WRITE_W_INITIALIZE);

    should_be_null(tto);
}

Test(
    TEST_SUITE_NAME,
    return_not_nil
) {
    char buf[100];
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    should_not_be_null(tto);
    should_be_same_int_wText(tto->used_block_count, 0, "used_block_count not 0");
    should_be_same_int_wText(tto->used_data_size, 0, "used_data_size not 0");
    should_be_same_int_wText(tto->used_size, 0, "used_size not 0");
    should_be_same_int_wText(tto->highwatermark_used_size, 0, "highwatermark_used_size not 0");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIfisInitialized_1stFieldUnused
) {
    char buf[50] = { (char)0xff };
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    should_be_same_int_wText(buf[0] & 0x01, 0, "not marked as unused");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    too_small
) {
    char buf[5] = { (char)0xff };
    should_be_null(lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE));
}

Test(
    TEST_SUITE_NAME,
    checkIfisInitialized_2stFieldLength
) {
    char buf[100] = { (char)0xff };
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    size_t len = (size_t)((memPool_headr_t *)buf)->len;
    should_be_same_int_wText(len, 100 - sizeof(memPool_headr_t), "free length not correct");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    return_obj_w_correct_class
) {
    char buf[100];
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    should_be_same_str_wText(tto->_.class, lfcMemPool(), "class not correctly set");
    should_be_same_str_wText(nameOf(getClass(tto)), "lfcMemPool", "class name not correctly set");
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    modeIs_READONLY
) {
    char buf[200];
    lfcMemPool_t *shm = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret1 __attribute__((unused)) = lfcMemPool_alloc(shm, 1);
    char *ret2 __attribute__((unused)) = lfcMemPool_alloc(shm, 1);
    char *ret3 __attribute__((unused)) = lfcMemPool_alloc(shm, 1);
    char *ret4 __attribute__((unused)) = lfcMemPool_alloc(shm, 1);

    lfcMemPool_free(shm, ret2);
    lfcMemPool_free(shm, ret3);
    delete(shm);

    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_READ_ONLY);

    should_not_be_null(tto);
    should_be_same_int_wText(tto->mode, memPool_READ_ONLY, "mode not set");
    should_be_same_int_wText(tto->used_block_count, 2, "used_block_count not set");
    should_be_same_int_wText(tto->used_data_size, ALIGN(1) * 2, "used_data_size not set");
    should_be_same_int_wText(tto->used_size, sizeof(memPool_headr_t) * 2 + ALIGN(1) * 2, "used_size not set");
    should_be_same_int_wText(tto->highwatermark_used_size, sizeof(memPool_headr_t) * 2 + ALIGN(1) * 2, "highwatermark_used_size not set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    modeIs_WRITE_WO_INITIALIZE
) {
    char buf[200];
    lfcMemPool_t *shm = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    char *ret1 __attribute__((unused)) = lfcMemPool_alloc(shm, 1);
    char *ret2 __attribute__((unused)) = lfcMemPool_alloc(shm, 1);
    char *ret3 __attribute__((unused)) = lfcMemPool_alloc(shm, 1);
    char *ret4 __attribute__((unused)) = lfcMemPool_alloc(shm, 1);

    lfcMemPool_free(shm, ret2);
    lfcMemPool_free(shm, ret3);
    delete(shm);

    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_WO_INITIALIZE);

    should_not_be_null(tto);
    should_be_same_int_wText(tto->mode, memPool_WRITE_WO_INITIALIZE, "mode not set");
    should_be_same_int_wText(tto->used_block_count, 2, "used_block_count not set");
    should_be_same_int_wText(tto->used_data_size, ALIGN(2) * 2, "used_data_size not set");
    should_be_same_int_wText(tto->used_size, sizeof(memPool_headr_t) * 2 + ALIGN(1) * 2, "used_size not set");
    should_be_same_int_wText(tto->highwatermark_used_size, sizeof(memPool_headr_t) * 2 + ALIGN(1) * 2, "highwatermark_used_size not set");

    delete(tto);
}
