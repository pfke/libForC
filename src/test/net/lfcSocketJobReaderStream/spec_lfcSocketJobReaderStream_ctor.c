#include <net/lfcSocketJobReaderStream.h>
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcSocketJobReaderStream_ctor

Test(
    TEST_SUITE_NAME,
    should_set_supersuper__fd
) {
    int fn_mock;
    lfcSocketJobReaderStream_t *tto = lfcSocketJobReaderStream_ctor(1345, NULL, NULL, 2, 46546, (fn_onReadComplete_cb)&fn_mock);

    should_be_same_int(tto->_._.fd, 1345);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set_supersuper__context
) {
    int context;
    int fn_mock;
    lfcSocketJobReaderStream_t *tto = lfcSocketJobReaderStream_ctor(1345, &context, NULL, 2, 46546, (fn_onReadComplete_cb)&fn_mock);

    should_be_same_ptr(tto->_._.context, &context);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set_supersuper__ident
) {
    int context;
    int ident;
    int fn_mock;
    lfcSocketJobReaderStream_t *tto = lfcSocketJobReaderStream_ctor(1345, &context, &ident, 2, 46546, (fn_onReadComplete_cb)&fn_mock);

    should_be_same_ptr(tto->_._.ident, &ident);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set_supersuper__timeout_in_s
) {
    int context;
    int fn_mock;
    lfcSocketJobReaderStream_t *tto = lfcSocketJobReaderStream_ctor(1345, &context, NULL, 456461, 46546, (fn_onReadComplete_cb)&fn_mock);

    should_be_same_int(tto->_._.timeout_in_s, 456461);

    delete(tto);
}

/**
 * --single=spec_lfcSocketJobReaderStream_ctor::should_set_supersuper__repeat
 */
Test(
    TEST_SUITE_NAME,
    should_set_supersuper__repeat
) {
    int context;
    int fn_mock;
    lfcSocketJobReaderStream_t *tto = lfcSocketJobReaderStream_ctor(1345, &context, NULL, 456461, 46546, (fn_onReadComplete_cb)&fn_mock);

    should_be_same_int(tto->_._.repeat, 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set_super__buf_len
) {
    int context;
    int fn_mock;
    lfcSocketJobReaderStream_t *tto = lfcSocketJobReaderStream_ctor(1345, &context, NULL, 456461, 46546, (fn_onReadComplete_cb)&fn_mock);

    should_be_same_int(tto->_.buf_len, 46546);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_set_super__onReadComplete
) {
    int context;
    int fn_mock;
    lfcSocketJobReaderStream_t *tto = lfcSocketJobReaderStream_ctor(1345, &context, NULL, 456461, 46546, (fn_onReadComplete_cb)&fn_mock);

    should_be_same_int(tto->_.onReadComplete, (fn_onReadComplete_cb)&fn_mock);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_alloc_super__buf
) {
    int context;
    int fn_mock;
    lfcSocketJobReaderStream_t *tto = lfcSocketJobReaderStream_ctor(1345, &context, NULL, 456461, 46546, (fn_onReadComplete_cb)&fn_mock);

    should_not_be_null(tto->_.buf);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    should_memset_super__buf
) {
    int context;
    int fn_mock;
    char *array;
    size_t array_size = 1024;
    lfcSocketJobReaderStream_t *tto = lfcSocketJobReaderStream_ctor(1345, &context, NULL, 456461, array_size, (fn_onReadComplete_cb)&fn_mock);

    array = calloc(1, array_size);

    should_be_same_array(tto->_.buf, array, array_size);

    delete(tto);
    free(array);
}

Test(
    TEST_SUITE_NAME,
    should_zero_super__already_read
) {
    int context;
    int fn_mock;
    lfcSocketJobReaderStream_t *tto = lfcSocketJobReaderStream_ctor(1345, &context, NULL, 456461, 46546, (fn_onReadComplete_cb)&fn_mock);

    should_be_same_int(tto->_.already_read, 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    err_if_callback_is_null
) {
    int context;
    lfcSocketJobReaderStream_t *tto = lfcSocketJobReaderStream_ctor(1345, &context, NULL, 456461, 13, NULL);

    should_be_null(tto);
}
