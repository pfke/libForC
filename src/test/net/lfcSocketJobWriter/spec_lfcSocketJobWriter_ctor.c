#include <net/lfcSocketJobWriter.h>
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcSocketJobWriter_ctor

Test(
    TEST_SUITE_NAME,
    should_set_super__fd
) {
    char buf;
    int fn_mock;
    lfcSocketJobWriter_t *tto = lfcSocketJobWriter_ctor(1345, NULL, NULL, 2, &buf, 1, (fn_onWriteComplete_cb)&fn_mock);

    should_be_same_int(tto->_.fd, 1345);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set_super__context
) {
    int context;
    char buf;
    int fn_mock;
    lfcSocketJobWriter_t *tto = lfcSocketJobWriter_ctor(1345, &context, NULL, 2, &buf, 1, (fn_onWriteComplete_cb)&fn_mock);

    should_be_same_ptr(tto->_.context, &context);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set_super__ident
) {
    int ident;
    char buf;
    int fn_mock;
    lfcSocketJobWriter_t *tto = lfcSocketJobWriter_ctor(1345, NULL, &ident, 2, &buf, 1, (fn_onWriteComplete_cb)&fn_mock);

    should_be_same_ptr(tto->_.ident, &ident);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set_super__timeout_in_s
) {
    int context;
    char buf;
    int fn_mock;
    lfcSocketJobWriter_t *tto = lfcSocketJobWriter_ctor(1345, &context, NULL, 456461, &buf, 1, (fn_onWriteComplete_cb)&fn_mock);

    should_be_same_int(tto->_.timeout_in_s, 456461);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set_super__repeat
) {
    int context;
    char buf;
    int fn_mock;
    lfcSocketJobWriter_t *tto = lfcSocketJobWriter_ctor(1345, &context, NULL, 456461, &buf, 1, (fn_onWriteComplete_cb)&fn_mock);

    should_be_same_int(tto->_.repeat, 1);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set__write_buf
) {
    int context;
    char buf;
    int fn_mock;
    lfcSocketJobWriter_t *tto = lfcSocketJobWriter_ctor(1345, &context, NULL, 456461, &buf, 1, (fn_onWriteComplete_cb)&fn_mock);

    should_be_same_ptr(tto->buf, &buf);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set__buf_len
) {
    int context;
    char buf;
    int fn_mock;
    lfcSocketJobWriter_t *tto = lfcSocketJobWriter_ctor(1345, &context, NULL, 456461, &buf, 123, (fn_onWriteComplete_cb)&fn_mock);

    should_be_same_int(tto->buf_len, 123);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set__onWriteComplete
) {
    int context;
    char buf;
    int fn_mock;
    lfcSocketJobWriter_t *tto = lfcSocketJobWriter_ctor(1345, &context, NULL, 456461, &buf, 1, (fn_onWriteComplete_cb)&fn_mock);

    should_be_same_int(tto->onWriteComplete, (fn_onWriteComplete_cb)&fn_mock);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_zero__already_written
) {
    int context;
    char buf;
    int fn_mock;
    lfcSocketJobWriter_t *tto = lfcSocketJobWriter_ctor(1345, &context, NULL, 456461, &buf, 1, (fn_onWriteComplete_cb)&fn_mock);

    should_be_same_int(tto->already_written, 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    err_if_write_buf_is_null
) {
    int context;
    int fn_mock;
    lfcSocketJobWriter_t *tto = lfcSocketJobWriter_ctor(1345, &context, NULL, 456461, NULL, 1, (fn_onWriteComplete_cb)&fn_mock);

    should_be_null(tto);
}
