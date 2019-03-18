#include "collection/lfcQueue.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcQueue__lfcQueue_ctor

Test(
    TEST_SUITE_NAME,
    return_not_NIL
) {
    lfcQueue_t *tto = lfcQueue_ctor();

    should_not_be_null(tto);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    return_obj_w_correct_name
) {
    lfcQueue_t *tto = lfcQueue_ctor();

    should_be_same_str_wText(nameOf(getClass(tto)), "lfcQueue", "class name not correctly set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    return_obj_w_correct_class
) {
    lfcQueue_t *tto = lfcQueue_ctor();

    should_be_same_ptr_wText(tto->_.class, lfcQueue(), "class not correctly set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    correct_initial_data
) {
    lfcQueue_t *tto = lfcQueue_ctor();

    should_not_be_null(tto->table);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    correct_initial_count
) {
    lfcQueue_t *tto = lfcQueue_ctor();

    should_be_same_int_wText(lfcQueue_methods()->lfcIIterable->count(tto), 0, "initial 'size' not set");

    delete(tto);
}
