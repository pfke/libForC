#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcList__lfcList_getAt

Test(
    TEST_SUITE_NAME,
    on_empty_list
) {
    lfcList_t *tto = lfcList_ctor();

    should_be_null(lfcList_methods()->getAt(tto, 13));

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmpty_list
) {
    lfcList_t *tto = lfcList_ctor();
    lfcList_add(tto, "1");
    lfcList_add(tto, "2");
    lfcList_add(tto, "3");

    should_be_same_ptr_wText(lfcList_getAt(tto, 1), "2", "soll den richtigen ptr zurueck geben");

    delete(tto);
}
