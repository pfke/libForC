#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcList__IIterable_next

Test(
    TEST_SUITE_NAME,
    on_emptyList
) {
    lfcList_t *tto = lfcList_ctor();

    should_be_null(lfcList_methods()->lfcIIterable->next(tto, NULL));

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList
) {
    lfcList_t *tto = lfcList_ctor();
    lfcList_add(tto, (void *)1);
    lfcList_add(tto, (void *)2);
    lfcList_add(tto, (void *)3);
    int result = 0;
    void *current = NULL;

    while ((current = lfcList_methods()->lfcIIterable->next(tto, current))) {
        result <<= 8;
        result += (int)(intptr_t)current;
    }

    should_be_same_int_wText(result, 0x010203, "soll alle Element durchlaufen haben");

    delete(tto);
}
