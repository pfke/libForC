#include "collection/lfcByteBuffer.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcByteBuffer_nonEmpty

Test(
    TEST_SUITE_NAME,
    checkIf_isEmpty_is_1_at_start
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();

    should_be_same_int(lfcByteBuffer_nonEmpty(tto), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_isEmpty_afterWrites
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();
    lfcByteBuffer_write_int8(tto, 1);
    lfcByteBuffer_write_int8(tto, 1);
    lfcByteBuffer_write_int8(tto, 1);

    should_be_same_int(lfcByteBuffer_nonEmpty(tto), 1);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_isEmpty_afterReads
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();
    lfcByteBuffer_write_int8(tto, 1);
    lfcByteBuffer_write_int8(tto, 1);
    lfcByteBuffer_write_int8(tto, 1);
    lfcByteBuffer_read_int8(tto, NULL);
    lfcByteBuffer_read_int8(tto, NULL);
    lfcByteBuffer_read_int8(tto, NULL);

    should_be_same_int(lfcByteBuffer_nonEmpty(tto), 0);

    delete(tto);
}
