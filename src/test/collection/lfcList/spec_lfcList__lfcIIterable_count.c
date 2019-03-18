#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcList__IIterable_count

Test(
    TEST_SUITE_NAME,
    on_emptyList
) {
    lfcList_t *tto = lfcList_ctor();

    should_be_same_int(lfcIIterable_count(tto), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList
) {
    lfcList_t *tto = lfcList_ctor();
    lfcList_add(tto, "klölkö");

    should_be_same_int(lfcIIterable_count(tto), 1);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_wrongSelf
) {
    lfcList_t *tto = lfcList_ctor();
    lfcList_add(tto, "klölkö");

    should_be_same_int(lfcIIterable_count("wrong"), 0);

    delete(tto);
}
