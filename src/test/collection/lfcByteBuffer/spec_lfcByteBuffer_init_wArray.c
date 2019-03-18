#include "collection/lfcByteBuffer.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcByteBuffer_init_wArray

Test(
    TEST_SUITE_NAME,
    checkIf_return_is_not_null
) {
    int8_t array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    lfcByteBuffer_t *tto = lfcByteBuffer_init_wArray(lfcBB_BIG_ENDIAN, sizeof(array), array);

    should_not_be_null(tto);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_arrayWasWritten
) {
    int8_t array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    lfcByteBuffer_t *tto = lfcByteBuffer_init_wArray(lfcBB_BIG_ENDIAN, sizeof(array), array);
    int8_t result;

    lfcByteBuffer_read_int8(tto, &result);
    should_be_same_int_wText(result, 1, "1st byte stimmt nicht")

    lfcByteBuffer_read_int8(tto, &result);
    should_be_same_int_wText(result, 2, "2nd byte stimmt nicht")

    lfcByteBuffer_read_int8(tto, &result);
    should_be_same_int_wText(result, 3, "3rd byte stimmt nicht")

    lfcByteBuffer_read_int8(tto, &result);
    should_be_same_int_wText(result, 4, "4th byte stimmt nicht")

    lfcByteBuffer_read_int8(tto, &result);
    should_be_same_int_wText(result, 5, "5th byte stimmt nicht")

    lfcByteBuffer_read_int8(tto, &result);
    should_be_same_int_wText(result, 6, "6th byte stimmt nicht")

    lfcByteBuffer_read_int8(tto, &result);
    should_be_same_int_wText(result, 7, "7th byte stimmt nicht")

    lfcByteBuffer_read_int8(tto, &result);
    should_be_same_int_wText(result, 8, "8th byte stimmt nicht")

    lfcByteBuffer_read_int8(tto, &result);
    should_be_same_int_wText(result, 9, "9th byte stimmt nicht")

    should_be_same_int_wText(lfcByteBuffer_read_int8(tto, NULL), 0, "konnte wohl nochwas lesen")

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_read_wDiffSizesWorks_BE
) {
    int8_t array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    lfcByteBuffer_t *tto = lfcByteBuffer_init_wArray(lfcBB_BIG_ENDIAN, sizeof(array), array);
    int8_t result_8;
    int16_t result_16;
    int32_t result_32;

    lfcByteBuffer_read_int8(tto, &result_8);
    should_be_same_int_wText(result_8, 1, "1st byte stimmt nicht")

    lfcByteBuffer_read_int16(tto, &result_16);
    should_be_same_int_wText(result_16, 0x0203, "2nd+3th byte stimmt nicht")

    lfcByteBuffer_read_int8(tto, &result_8);
    should_be_same_int_wText(result_8, 4, "4th byte stimmt nicht")

    lfcByteBuffer_read_int32(tto, &result_32);
    should_be_same_int_wText(result_32, 0x05060708, "5th+6th+7th+8th byte stimmt nicht")

    lfcByteBuffer_read_int8(tto, &result_8);
    should_be_same_int_wText(result_8, 9, "9th byte stimmt nicht")

    should_be_same_int_wText(lfcByteBuffer_read_int8(tto, NULL), 0, "konnte wohl nochwas lesen")

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_read_wDiffSizesWorks_LE
) {
    int8_t array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    lfcByteBuffer_t *tto = lfcByteBuffer_init_wArray(lfcBB_LITTLE_ENDIAN, sizeof(array), array);
    int8_t result_8;
    int16_t result_16;
    int32_t result_32;

    lfcByteBuffer_read_int8(tto, &result_8);
    should_be_same_int_wText(result_8, 1, "1st byte stimmt nicht")

    lfcByteBuffer_read_int16(tto, &result_16);
    should_be_same_int_wText(result_16, 0x0302, "2nd+3th byte stimmt nicht")

    lfcByteBuffer_read_int8(tto, &result_8);
    should_be_same_int_wText(result_8, 4, "4th byte stimmt nicht")

    lfcByteBuffer_read_int32(tto, &result_32);
    should_be_same_int_wText(result_32, 0x08070605, "5th+6th+7th+8th byte stimmt nicht")

    lfcByteBuffer_read_int8(tto, &result_8);
    should_be_same_int_wText(result_8, 9, "9th byte stimmt nicht")

    should_be_same_int_wText(lfcByteBuffer_read_int8(tto, NULL), 0, "konnte wohl nochwas lesen")

    delete(tto);
}
