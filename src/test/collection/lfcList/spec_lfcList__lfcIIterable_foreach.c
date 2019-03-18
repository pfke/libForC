#include <core/lfcCore.h>
#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcList__IIterable_foreach

Test(
    TEST_SUITE_NAME,
    on_emptyList
) {
    lfcList_t *tto = lfcList_ctor();
    int result = 0;

    lfcList_methods()->lfcIIterable->foreach(tto, lambda(void, (void *item isAnIncognito_param) {
        result <<= 8;
        result += (int)(intptr_t)item;
    }));

    should_be_same_int_wText(result, 0, "soll alle Element durchlaufen haben");

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

    lfcList_methods()->lfcIIterable->foreach(tto, lambda(void, (void *item isAnIncognito_param) {
        result <<= 8;
        result += (int)(intptr_t)item;
    }));

    should_be_same_int_wText(result, 0x010203, "soll alle Element durchlaufen haben");

    delete(tto);
}
