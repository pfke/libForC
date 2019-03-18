#include "collection/lfcQueue.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcQueue__lfcQueue_getAt

Test(
    TEST_SUITE_NAME,
    on_empty_list
) {
    lfcQueue_t *tto = lfcQueue_ctor();

    should_be_null(lfcQueue_methods()->dequeue(tto));

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmpty_list
) {
    lfcQueue_t *tto = lfcQueue_ctor();
    lfcQueue_enqueue(tto, "1");
    lfcQueue_enqueue(tto, "2");
    lfcQueue_enqueue(tto, "3");

    should_be_same_ptr_wText(lfcQueue_dequeue(tto), "1", "soll den richtigen ptr zurueck geben");

    delete(tto);
}
