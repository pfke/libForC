#include "collection/lfcByteBuffer.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcByteBuffer_write_uint8

Test(
    TEST_SUITE_NAME,
    return_0_after_start
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();

    should_be_same_int_wText(lfcByteBuffer_write_uint8(tto, 1), 1, "return 1");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_capacity_is_incremented
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();
    lfcByteBuffer_write_uint8(tto, 1);

    should_be_same_int_wText(lfcByteBuffer_capacity(tto), 1, "should be incremented");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    read_correct
) {
    uint8_t result = 0;
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();
    lfcByteBuffer_write_uint8(tto, 11);

    lfcByteBuffer_read_uint8(tto, &result);
    should_be_same_int(result, 11);

    delete(tto);
}
