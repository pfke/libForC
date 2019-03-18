#include <core/lfcCore.h>
#include "collection/lfcQueue.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcQueue__IIterable_find

Test(
    TEST_SUITE_NAME,
    on_emptyList
) {
    lfcQueue_t *src = lfcQueue_ctor();
    lfcQueue_t *tto = lfcIIterable_find(src, lambda(int, (void *item isAnIncognito_param) {
        return !strncmp(item, "1", 1);
    }));

    should_be_null(tto);

    delete(src);
}

#ifdef CHECK_ALLOC_VIA_MPROBE
Test(
    TEST_SUITE_NAME,
    on_emptyList_dontFreeSource
) {
    lfcQueue_t *src = lfcQueue_ctor();
    lfcIIterable_find(src, lambda(int, (void *item isAnIncognito_param) {
        return !strncmp(item, "1", 1);
    }));

    should_not_be_freed(src);

    delete(src);
}
#endif // #ifdef CHECK_ALLOC_VIA_MPROBE

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_filter1_testItem1
) {
    lfcQueue_t *src = lfcQueue_ctor();
    lfcQueue_enqueue(src, "1");
    lfcQueue_enqueue(src, "2");
    lfcQueue_enqueue(src, "3");
    lfcQueue_t *tto = lfcIIterable_find(src, lambda(int, (void *item isAnIncognito_param) {
        return !strncmp(item, "2", 1);
    }));

    should_be_same_str_wText(tto, "2", "soll 1 Element gefunden haben");

    delete(src);
}
