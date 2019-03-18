#include <core/lfcCore.h>
#include "collection/lfcQueue.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcQueue__IIterable_foreach

Test(
    TEST_SUITE_NAME,
    on_emptyList
) {
    lfcQueue_t *tto = lfcQueue_ctor();
    int result = 0;

    lfcQueue_methods()->lfcIIterable->foreach(tto, lambda(void, (void *item isAnIncognito_param) {
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
    lfcQueue_t *tto = lfcQueue_ctor();
    lfcQueue_enqueue(tto, (void *)1);
    lfcQueue_enqueue(tto, (void *)2);
    lfcQueue_enqueue(tto, (void *)3);
    int result = 0;

    lfcQueue_methods()->lfcIIterable->foreach(tto, lambda(void, (void *item isAnIncognito_param) {
        result <<= 8;
        result += (int)(intptr_t)item;
    }));

    should_be_same_int_wText(result, 0x010203, "soll alle Element durchlaufen haben");

    delete(tto);
}
