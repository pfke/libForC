#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcList__lfcList_indexOf

Test(
    TEST_SUITE_NAME,
    not_existing_on_nonEmpty_list
) {
    lfcList_t *tto = lfcList_ctor();
    lfcList_add(tto, "1");

    should_be_same_int_wText(lfcList_methods()->indexOf(tto, "2"), UINT32_MAX, "soll UINT32_MAX zurueck geben");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    not_existing_on_empty_list
) {
    lfcList_t *tto = lfcList_ctor();

    should_be_same_int_wText(lfcList_indexOf(tto, "2"), UINT32_MAX, "soll UINT32_MAX zurueck geben");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    existing
) {
    lfcList_t *tto = lfcList_ctor();
    lfcList_add(tto, "1");
    lfcList_add(tto, "2");
    lfcList_add(tto, "3");

    should_be_same_int_wText(lfcList_indexOf(tto, "2"), 1, "soll den korrekten Index zurueck geben");

    delete(tto);
}
