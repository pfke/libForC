#include "collection/lfcQueue.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcQueue__lfcQueue_ctorWithSize

Test(
    TEST_SUITE_NAME,
    return_not_NIL
) {
    lfcQueue_t *tto = lfcQueue_ctorWithSize(5, 13);

    should_not_be_null(tto);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    correct_initial_data
) {
    lfcQueue_t *tto = lfcQueue_ctorWithSize(5, 13);

    should_not_be_null(tto->table);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    correct_initial_count
) {
    lfcQueue_t *tto = lfcQueue_ctorWithSize(5, 13);

    should_be_same_int_wText(lfcQueue_methods()->lfcIIterable->count(tto), 0, "initial 'size' not set");

    delete(tto);
}
