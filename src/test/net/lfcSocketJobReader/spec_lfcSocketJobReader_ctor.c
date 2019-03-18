#include <net/lfcSocketJobReader.h>
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcSocketJobReader_ctor

Test(
    TEST_SUITE_NAME,
    should_set_super__fd
) {
    int fn_mock;
    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor(1345, NULL, NULL, 2, 46546, (fn_onReadComplete_cb)&fn_mock);

    should_be_same_int(tto->_.fd, 1345);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set_super__context
) {
    int context;
    int fn_mock;
    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor(1345, &context, NULL, 2, 46546, (fn_onReadComplete_cb)&fn_mock);

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
    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor(1345, &context, &ident, 2, 46546, (fn_onReadComplete_cb)&fn_mock);

    should_be_same_ptr(tto->_.ident, &ident);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set_super__timeout_in_s
) {
    int context;
    int fn_mock;
    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor(1345, &context, NULL, 456461, 46546, (fn_onReadComplete_cb)&fn_mock);

    should_be_same_int(tto->_.timeout_in_s, 456461);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set_super__repeat
) {
    int context;
    int fn_mock;
    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor(1345, &context, NULL, 456461, 46546, (fn_onReadComplete_cb)&fn_mock);

    should_be_same_int(tto->_.repeat, 1);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set__buf_len
) {
    int context;
    int fn_mock;
    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor(1345, &context, NULL, 456461, 46546, (fn_onReadComplete_cb)&fn_mock);

    should_be_same_int(tto->buf_len, 46546);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set__onReadComplete
) {
    int context;
    int fn_mock;
    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor(1345, &context, NULL, 456461, 46546, (fn_onReadComplete_cb)&fn_mock);

    should_be_same_int(tto->onReadComplete, (fn_onReadComplete_cb)&fn_mock);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_alloc__buf
) {
    int context;
    int fn_mock;
    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor(1345, &context, NULL, 456461, 46546, (fn_onReadComplete_cb)&fn_mock);

    should_not_be_null(tto->buf);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_memset__buf
) {
    int context;
    int fn_mock;
    char *array;
    size_t array_size = 1024;
    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor(1345, &context, NULL, 456461, array_size, (fn_onReadComplete_cb)&fn_mock);

    array = calloc(1, array_size);

    should_be_same_array(tto->buf, array, array_size);

    delete(tto);
    free(array);
}

Test(
    TEST_SUITE_NAME,
    should_zero__already_read
) {
    int context;
    int fn_mock;
    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor(1345, &context, NULL, 456461, 46546, (fn_onReadComplete_cb)&fn_mock);

    should_be_same_int(tto->already_read, 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    err_if_callback_is_null
) {
    int context;
    lfcSocketJobReader_t *tto = lfcSocketJobReader_ctor(1345, &context, NULL, 456461, 13, NULL);

    should_be_null(tto);
}
