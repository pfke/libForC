#include "collection/lfcQueue.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcQueue__IIterable_count

Test(
    TEST_SUITE_NAME,
    on_emptyList
) {
    lfcQueue_t *tto = lfcQueue_ctor();

    should_be_same_int(lfcIIterable_count(tto), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList
) {
    lfcQueue_t *tto = lfcQueue_ctor();
    lfcQueue_enqueue(tto, "klölkö");

    should_be_same_int(lfcIIterable_count(tto), 1);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_wrongType
) {
    lfcQueue_t *tto = lfcQueue_ctor();
    lfcQueue_enqueue(tto, "klölkö");

    should_be_same_int(lfcIIterable_count("edsdfs"), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_nullType
) {
    lfcQueue_t *tto = lfcQueue_ctor();
    lfcQueue_enqueue(tto, "klölkö");

    should_be_same_int(lfcIIterable_count(NULL), 0);

    delete(tto);
}
