#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcList__IIterable_isEmpty

Test(
    TEST_SUITE_NAME,
    on_empty_list
) {
    lfcList_t *tto = lfcList_ctor();

    should_be_same_int(lfcIIterable_isEmpty(tto), 1);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmpty_list
) {
    lfcList_t *tto = lfcList_ctor();
    lfcList_add(tto, "1");

    should_be_same_int(lfcIIterable_isEmpty(tto), 0);

    delete(tto);
}
