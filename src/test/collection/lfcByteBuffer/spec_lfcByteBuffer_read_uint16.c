#include "collection/lfcByteBuffer.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcByteBuffer_read_uint16

Test(
    TEST_SUITE_NAME,
    return_0_after_start
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();

    should_be_same_int(lfcByteBuffer_read_uint16(tto, NULL), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    return_0_if_capacity_is_0
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();
    lfcByteBuffer_write_int8(tto, 1);
    lfcByteBuffer_write_int8(tto, 1);
    lfcByteBuffer_write_int8(tto, 1);
    lfcByteBuffer_read_int16(tto, NULL);
    lfcByteBuffer_read_int8(tto, NULL);

    should_be_same_int(lfcByteBuffer_read_uint16(tto, NULL), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    return_not_0_if_capacity_is_not0__passNULL
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();
    lfcByteBuffer_write_uint16(tto, 125);

    should_be_same_int(lfcByteBuffer_read_uint16(tto, NULL), 2);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    read_correct
) {
    uint16_t result = 0;
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();
    lfcByteBuffer_write_uint16(tto, 1145);

    lfcByteBuffer_read_uint16(tto, &result);
    should_be_same_int(result, 1145);

    delete(tto);
}
