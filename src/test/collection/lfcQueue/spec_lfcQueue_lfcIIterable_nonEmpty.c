#include "collection/lfcQueue.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcQueue__IIterable_nonEmpty

Test(
    TEST_SUITE_NAME,
    on_empty_list
) {
    lfcQueue_t *tto = lfcQueue_ctor();

    should_be_same_int(lfcIIterable_nonEmpty(tto), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmpty_list
) {
    lfcQueue_t *tto = lfcQueue_ctor();
    lfcQueue_enqueue(tto, "1");

    should_be_same_int(lfcIIterable_nonEmpty(tto), 1);

    delete(tto);
}
