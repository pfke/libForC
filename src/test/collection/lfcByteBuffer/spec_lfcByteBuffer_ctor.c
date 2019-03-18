#include "collection/lfcByteBuffer.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcByteBuffer_ctor

Test(
    TEST_SUITE_NAME,
    checkIf_return_is_not_null
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();

    should_not_be_null(tto);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_table_is_not_null
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();

    should_not_be_null(tto->table);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_table_has_default_size
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();

    should_be_same_int_wText(tto->table_size, CBB_DEFAULT_INITIAL_SIZE, "not initialized with default size");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_table_has_default_increaseBy
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();

    should_be_same_int_wText(tto->increaseBy, CBB_DEFAULT_INCREASE_BY, "not initialized with default increase by");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_read_ptr_is_0
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();

    should_be_same_int_wText(tto->read_ptr, 0, "read_ptr is not 0");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    checkIf_write_ptr_is_0
) {
    lfcByteBuffer_t *tto = lfcByteBuffer_ctor();

    should_be_same_int_wText(tto->write_ptr, 0,"write_ptr is not 0");

    delete(tto);
}
