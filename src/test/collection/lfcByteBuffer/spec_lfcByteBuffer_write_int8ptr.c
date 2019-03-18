#include "collection/lfcByteBuffer.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcByteBuffer_write_int8ptr

Test(
    TEST_SUITE_NAME,
    called_with_len_0
) {
    int8_t buf[10];
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();
    lfcByteBuffer_write_int8(tto, 1);
    lfcByteBuffer_write_int8(tto, 1);
    lfcByteBuffer_write_int8(tto, 1);

    should_be_same_int(lfcByteBuffer_write_int8ptr(tto, 0, buf), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    called_with_buf_null
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();
    lfcByteBuffer_write_int8(tto, 1);
    lfcByteBuffer_write_int8(tto, 1);
    lfcByteBuffer_write_int8(tto, 1);

    should_be_same_int(lfcByteBuffer_write_int8ptr(tto, 3, NULL), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    return_0_after_start
) {
    int8_t buf[10];
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();

    should_be_same_int(lfcByteBuffer_write_int8ptr(tto, 3, buf), 3);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    return_correct_if_capacity_is_not0
) {
    int8_t buf[10];
    int8_t compare_to[] = { 125, 1, 1 };
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();

    lfcByteBuffer_write_int8ptr(tto, 3, compare_to);
    lfcByteBuffer_read_int8ptr(tto, 3, buf);

    int8_t *c_buf = buf;
    int8_t *comp_buf = compare_to;
    should_be_same_array(comp_buf, c_buf, 3);

    delete(tto);
}
