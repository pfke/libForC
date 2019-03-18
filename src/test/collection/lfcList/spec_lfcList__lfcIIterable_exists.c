#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcList__IIterable_exists

Test(
    TEST_SUITE_NAME,
    on_emptyList
) {
    lfcList_t *tto = lfcList_ctor();

    should_be_same_int(lfcIIterable_exists(tto, "1"), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_existing
) {
    lfcList_t *tto = lfcList_ctor();
    lfcList_add(tto, "1");

    should_be_same_int(lfcIIterable_exists(tto, "1"), 1);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_notExisting
) {
    lfcList_t *tto = lfcList_ctor();
    lfcList_add(tto, "1");

    should_be_same_int(lfcIIterable_exists(tto, "2"), 0);

    delete(tto);
}
