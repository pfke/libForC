#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcList__IIterable_head

Test(
    TEST_SUITE_NAME,
    on_empty_list
) {
    lfcList_t *tto = lfcList_ctor();

    should_be_null(lfcIIterable_head(tto));

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

    should_be_same_ptr_wText(lfcIIterable_head(tto), "1", "soll ptr zu \"1\" zurueck geben");

    delete(tto);
}
