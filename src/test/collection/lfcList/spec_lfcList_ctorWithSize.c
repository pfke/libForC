#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcList__lfcList_ctorWithSize

Test(
    TEST_SUITE_NAME,
    return_not_NIL
) {
    lfcList_t *tto = lfcList_ctorWithSize(5, 13);

    should_not_be_null(tto);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    correct_initial_data
) {
    lfcList_t *tto = lfcList_ctorWithSize(5, 13);

    should_not_be_null(tto->table);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    correct_initial_count
) {
    lfcList_t *tto = lfcList_ctorWithSize(5, 13);

    should_be_same_int_wText(tto->size, 0, "initial 'size' not set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    correct_initial_size
) {
    lfcList_t *tto = lfcList_ctorWithSize(5, 13);

    should_be_same_int_wText(tto->table_size, 5, "initial 'table_size' not set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    correct_initial_increaseBy
) {
    lfcList_t *tto = lfcList_ctorWithSize(5, 13);

    should_be_same_int_wText(tto->increaseBy, 13, "initial 'increaseBy' not set");

    delete(tto);
}
