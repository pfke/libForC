#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcList__lfcList_clear

Test(
    TEST_SUITE_NAME,
    clearOnEmpty
) {
    lfcList_t *tto = lfcList_ctor();

    should_be_same_int(lfcList_methods()->clear(tto), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    clearOnNonEmpty__checkReturn
) {
    lfcList_t *tto = lfcList_ctor();
    lfcList_add(tto, "1");
    lfcList_add(tto, "2");
    lfcList_add(tto, "3");

    should_be_same_int(lfcList_clear(tto), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    clearOnNonEmpty__checkCount
) {
    lfcList_t *tto = lfcList_ctor();
    lfcList_add(tto, "1");
    lfcList_add(tto, "2");
    lfcList_add(tto, "3");
    lfcList_clear(tto);

    should_be_same_int(lfcIIterable_count(tto), 0);

    delete(tto);
}
