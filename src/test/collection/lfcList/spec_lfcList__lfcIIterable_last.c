#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcList__IIterable_last

Test(
    TEST_SUITE_NAME,
    on_empty_list
) {
    lfcList_t *tto = lfcList_ctor();

    should_be_null(lfcIIterable_last(tto));

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmpty_list_onlyOne
) {
    lfcList_t *tto = lfcList_ctor();
    lfcList_add(tto, "1");

    should_be_same_ptr_wText(lfcIIterable_last(tto), "1", "soll ptr zu \"1\" zurueck geben");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmpty_list_more
) {
    lfcList_t *tto = lfcList_ctor();
    lfcList_add(tto, "1");
    lfcList_add(tto, "2");
    lfcList_add(tto, "3");

    should_be_same_ptr_wText(lfcIIterable_last(tto), "3", "soll ptr zu \"3\" zurueck geben");

    delete(tto);
}
