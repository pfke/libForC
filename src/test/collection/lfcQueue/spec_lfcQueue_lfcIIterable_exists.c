#include "collection/lfcQueue.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcQueue__IIterable_exists

Test(
    TEST_SUITE_NAME,
    on_emptyList
) {
    lfcQueue_t *tto = lfcQueue_ctor();

    should_be_same_int(lfcIIterable_exists(tto, "1"), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_existing
) {
    lfcQueue_t *tto = lfcQueue_ctor();
    lfcQueue_enqueue(tto, "1");

    should_be_same_int(lfcIIterable_exists(tto, "1"), 1);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_notExisting
) {
    lfcQueue_t *tto = lfcQueue_ctor();
    lfcQueue_enqueue(tto, "1");

    should_be_same_int(lfcIIterable_exists(tto, "2"), 0);

    delete(tto);
}
