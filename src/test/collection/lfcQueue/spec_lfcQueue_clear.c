#include "collection/lfcQueue.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcQueue__lfcQueue_clear

Test(
    TEST_SUITE_NAME,
    clearOnEmpty
) {
    lfcQueue_t *tto = lfcQueue_ctor();

    should_be_same_int(lfcQueue_methods()->clear(tto), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    clearOnNonEmpty__checkReturn
) {
    lfcQueue_t *tto = lfcQueue_ctor();
    lfcQueue_enqueue(tto, "1");
    lfcQueue_enqueue(tto, "2");
    lfcQueue_enqueue(tto, "3");

    should_be_same_int(lfcQueue_clear(tto), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    clearOnNonEmpty__checkCount
) {
    lfcQueue_t *tto = lfcQueue_ctor();
    lfcQueue_enqueue(tto, "1");
    lfcQueue_enqueue(tto, "2");
    lfcQueue_enqueue(tto, "3");
    lfcQueue_clear(tto);

    should_be_same_int(lfcIIterable_count(tto), 0);

    delete(tto);
}
