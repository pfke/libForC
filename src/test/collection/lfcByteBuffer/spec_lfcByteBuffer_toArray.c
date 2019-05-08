#include "collection/lfcByteBuffer.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcByteBuffer_toArray

Test(
    TEST_SUITE_NAME,
    checkIf_return_matches_size
) {
    int8_t array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int8_t *cmp_array;
    lfcByteBuffer_t *tto = lfcByteBuffer_init_fromArray(lfcBB_BIG_ENDIAN, sizeof(array), array);

    size_t result = lfcByteBuffer_toArray(tto, &cmp_array);
    should_be_same_int(9, result);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_return_matches_content
) {
    int8_t array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int8_t *cmp_array;
    lfcByteBuffer_t *tto = lfcByteBuffer_init_fromArray(lfcBB_BIG_ENDIAN, sizeof(array), array);

    lfcByteBuffer_toArray(tto, &cmp_array);
    should_be_same_array((char *)array, (char *)cmp_array, 9);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_return_matches_size_afterRead
) {
    int8_t array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int8_t *cmp_array;
    int16_t read;
    lfcByteBuffer_t *tto = lfcByteBuffer_init_fromArray(lfcBB_BIG_ENDIAN, sizeof(array), array);
    lfcByteBuffer_read_int16(tto, &read);

    size_t result = lfcByteBuffer_toArray(tto, &cmp_array);
    should_be_same_int(7, result);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_return_matches_content_afterRead
) {
    int8_t array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int8_t *cmp_array;
    int16_t read;
    lfcByteBuffer_t *tto = lfcByteBuffer_init_fromArray(lfcBB_BIG_ENDIAN, sizeof(array), array);
    lfcByteBuffer_read_int16(tto, &read);

    lfcByteBuffer_toArray(tto, &cmp_array);
    should_be_same_array((char *)&array[2], (char *)cmp_array, 7);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_return_matches_size_afterWrite
) {
    int8_t array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int8_t *cmp_array;
    lfcByteBuffer_t *tto = lfcByteBuffer_init_fromArray(lfcBB_BIG_ENDIAN, sizeof(array), array);
    lfcByteBuffer_write_int16(tto, 18);

    size_t result = lfcByteBuffer_toArray(tto, &cmp_array);
    should_be_same_int(11, result);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_return_matches_content_afterWrite
) {
    int8_t array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int8_t exp_array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 18 };
    int8_t *cmp_array;
    lfcByteBuffer_t *tto = lfcByteBuffer_init_fromArray(lfcBB_BIG_ENDIAN, sizeof(array), array);
    lfcByteBuffer_write_int16(tto, 18);

    lfcByteBuffer_toArray(tto, &cmp_array);
    should_be_same_array((char *)exp_array, (char *)cmp_array, 7);

    delete(tto);
}
