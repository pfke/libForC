#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcList__lfcList_ctor

Test(
    TEST_SUITE_NAME,
    return_not_NIL
) {
    lfcList_t *tto = lfcList_ctor();

    should_not_be_null(tto);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    return_obj_w_correct_name
) {
    lfcList_t *tto = lfcList_ctor();

    should_be_same_str_wText(nameOf(getClass(tto)), "lfcList", "class name not correctly set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    return_obj_w_correct_class
) {
    lfcList_t *tto = lfcList_ctor();

    should_be_same_ptr_wText(tto->_.class, lfcList(), "class not correctly set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    correct_initial_data
) {
    lfcList_t *tto = lfcList_ctor();

    should_not_be_null(tto->table);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    correct_initial_count
) {
    lfcList_t *tto = lfcList_ctor();

    should_be_same_int_wText(tto->size, 0, "initial 'size' not set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    correct_initial_size
) {
    lfcList_t *tto = lfcList_ctor();

    should_be_same_int_wText(tto->table_size, DEFAULT_LIST_INITIAL_SIZE, "initial 'table_size' not set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    correct_initial_increaseBy
) {
    lfcList_t *tto = lfcList_ctor();

    should_be_same_int_wText(tto->increaseBy, DEFAULT_LIST_INCREASE_BY, "initial 'increaseBy' not set");

    delete(tto);
}
