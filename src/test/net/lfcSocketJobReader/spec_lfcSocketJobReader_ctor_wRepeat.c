#include <net/lfcSocketJobReader.h>
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcSocketJobReader_ctor_wRepeat

Test(
    TEST_SUITE_NAME,
    should_set_super__fd
) {
    int fn_mock;
    char buf_mock;
    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor_wRepeat(1345, NULL, NULL, 2, 1, &buf_mock, 1, false, (fn_onReadComplete_cb)&fn_mock);

    should_be_same_int(tto->_.fd, 1345);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set_super__context
) {
    int context;
    int fn_mock;
    char buf_mock;
    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor_wRepeat(1345, &context, NULL, 2, 1, &buf_mock, 1, false, (fn_onReadComplete_cb)&fn_mock);

    should_be_same_ptr(tto->_.context, &context);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set_super__ident
) {
    int context;
    int ident;
    int fn_mock;
    char buf_mock;
    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor_wRepeat(1345, &context, &ident, 2, 1, &buf_mock, 1, false, (fn_onReadComplete_cb)&fn_mock);

    should_be_same_ptr(tto->_.ident, &ident);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set_super__timeout_in_s
) {
    int context;
    int fn_mock;
    char buf_mock;
    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor_wRepeat(1345, &context, NULL, 456461, 1, &buf_mock, 1, false, (fn_onReadComplete_cb)&fn_mock);

    should_be_same_int(tto->_.timeout_in_s, 456461);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set_super__repeat
) {
    int context;
    int fn_mock;
    char buf_mock;
    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor_wRepeat(1345, &context, NULL, 456461, 11221, &buf_mock, 1, false, (fn_onReadComplete_cb)&fn_mock);

    should_be_same_int(tto->_.repeat, 11221);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set__buf
) {
    int context;
    int fn_mock;
    char buf_mock;
    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor_wRepeat(1345, &context, NULL, 456461, 1, &buf_mock, 1, false, (fn_onReadComplete_cb)&fn_mock);

    should_be_same_ptr(tto->buf, &buf_mock);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set__buf_len
) {
    int context;
    int fn_mock;
    char buf_mock[21];
    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor_wRepeat(1345, &context, NULL, 456461, 1, buf_mock, 21, false, (fn_onReadComplete_cb)&fn_mock);

    should_be_same_int(tto->buf_len, 21);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set__onReadComplete
) {
    int context;
    int fn_mock;
    char buf_mock;
    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor_wRepeat(1345, &context, NULL, 456461, 1, &buf_mock, 1, false, (fn_onReadComplete_cb)&fn_mock);

    should_be_same_int(tto->onReadComplete, (fn_onReadComplete_cb)&fn_mock);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_alloc__buf
) {
    int context;
    int fn_mock;
    char buf_mock;
    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor_wRepeat(1345, &context, NULL, 456461, 1, &buf_mock, 1, false, (fn_onReadComplete_cb)&fn_mock);

    should_not_be_null(tto->buf);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_memset__buf
) {
    int context;
    int fn_mock;
    size_t array_size = 1024;
    char buf_mock[1024];
    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor_wRepeat(1345, &context, NULL, 456461, 1, buf_mock, array_size, false, (fn_onReadComplete_cb)&fn_mock);

    should_be_a_cleared_array(tto->buf, 0, array_size)

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_zero__already_read
) {
    int context;
    int fn_mock;
    char buf_mock;
    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor_wRepeat(1345, &context, NULL, 456461, 1, &buf_mock, 1, false, (fn_onReadComplete_cb)&fn_mock);

    should_be_same_int(tto->already_read, 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    err_if_buf_is_null
) {
    int context;
    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor_wRepeat(1345, &context, NULL, 456461, 1, NULL, 1, false, NULL);

    should_be_null(tto);
}

Test(
    TEST_SUITE_NAME,
    err_if_callback_is_null
) {
    int context;
    char buf_mock;
    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor_wRepeat(1345, &context, NULL, 456461, 1, &buf_mock, 1, false, NULL);

    should_be_null(tto);
}
