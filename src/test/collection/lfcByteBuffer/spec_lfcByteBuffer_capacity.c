#include "collection/lfcByteBuffer.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcByteBuffer_capacity

Test(
    TEST_SUITE_NAME,
    checkIf_capacity_is_0_at_start
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();

    should_be_same_int(lfcByteBuffer_capacity(tto), 0);

    delete(tto);
}

/**
 * --single=spec_lfcByteBuffer_capacity::checkIf_capacity_is_correct_after_write_int8
 */
Test(
    TEST_SUITE_NAME,
    checkIf_capacity_is_correct_after_write_int8
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();
    lfcByteBuffer_write_int8(tto, -1);

    should_be_same_int(lfcByteBuffer_capacity(tto), 1);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_capacity_is_correct_after_write_int16
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();
    lfcByteBuffer_write_int16(tto, -1);

    should_be_same_int(lfcByteBuffer_capacity(tto), 2);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_capacity_is_correct_after_write_int32
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();
    lfcByteBuffer_write_int32(tto, -1);

    should_be_same_int(lfcByteBuffer_capacity(tto), 4);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_capacity_is_correct_after_write_int64
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();
    lfcByteBuffer_write_int64(tto, -1);

    should_be_same_int(lfcByteBuffer_capacity(tto), 8);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_capacity_is_correct_after_write_uint8
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();
    lfcByteBuffer_write_uint8(tto, 1);

    should_be_same_int(lfcByteBuffer_capacity(tto), 1);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_capacity_is_correct_after_write_uint16
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();
    lfcByteBuffer_write_uint16(tto, 1);

    should_be_same_int(lfcByteBuffer_capacity(tto), 2);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_capacity_is_correct_after_write_uint32
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();
    lfcByteBuffer_write_uint32(tto, 1);

    should_be_same_int(lfcByteBuffer_capacity(tto), 4);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_capacity_is_correct_after_write_uint64
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();
    lfcByteBuffer_write_uint64(tto, 1);

    should_be_same_int(lfcByteBuffer_capacity(tto), 8);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_capacity_is_correct_after_write_mixed
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();
    lfcByteBuffer_write_uint8(tto, 1);
    lfcByteBuffer_write_uint16(tto, 1);
    lfcByteBuffer_write_uint8(tto, 1);
    lfcByteBuffer_write_uint8(tto, 1);
    lfcByteBuffer_write_uint32(tto, 1);

    should_be_same_int(lfcByteBuffer_capacity(tto), 9);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_capacity_is_correct_after_reading
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();
    lfcByteBuffer_write_uint8(tto, 1);
    lfcByteBuffer_write_uint16(tto, 2);
    lfcByteBuffer_write_uint8(tto, 3);
    lfcByteBuffer_write_uint8(tto, 4);
    lfcByteBuffer_write_uint32(tto, 5);

    lfcByteBuffer_read_uint8(tto, NULL);
    lfcByteBuffer_read_uint16(tto, NULL);
    lfcByteBuffer_read_uint8(tto, NULL);
    lfcByteBuffer_read_uint8(tto, NULL);

    should_be_same_int(lfcByteBuffer_capacity(tto), 4);

    delete(tto);
}
