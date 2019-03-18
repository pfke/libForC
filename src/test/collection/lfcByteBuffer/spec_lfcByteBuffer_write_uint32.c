#include "collection/lfcByteBuffer.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcByteBuffer_write_uint32

Test(
    TEST_SUITE_NAME,
    return_0_after_start
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();

    should_be_same_int_wText(lfcByteBuffer_write_uint32(tto, 1), 4, "return 4");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_capacity_is_incremented
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();
    lfcByteBuffer_write_uint32(tto, 1);

    should_be_same_int_wText(lfcByteBuffer_capacity(tto), 4, "should be incremented");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    read_correct
) {
    uint32_t result = 0;
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();
    lfcByteBuffer_write_uint32(tto, 1145);

    lfcByteBuffer_read_uint32(tto, &result);
    should_be_same_int(result, 1145);

    delete(tto);
}
