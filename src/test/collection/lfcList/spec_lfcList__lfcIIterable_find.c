#include <core/lfcCore.h>
#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcList__IIterable_find

Test(
    TEST_SUITE_NAME,
    on_emptyList
) {
    lfcList_t *src = lfcList_ctor();
    int *tto = lfcIIterable_find(src, lambda(int, (void *item isAnIncognito_param) {
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
    lfcList_t *src = lfcList_ctor();
    lfcIIterable_find(src, lambda(int, (void *item isAnIncognito_param) {
        return !strncmp(item, "1", 1);
    }));

    should_not_be_freed(src);

    delete(src);
}
#endif // #ifdef CHECK_ALLOC_VIA_MPROBE

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_find_testItem1
) {
    lfcList_t *src = lfcList_ctor();
    lfcList_add(src, "1");
    lfcList_add(src, "2");
    lfcList_add(src, "3");
    int *tto = lfcIIterable_find(src, lambda(int, (void *item isAnIncognito_param) {
        return !strncmp(item, "2", 1);
    }));

    should_be_same_str_wText(tto, "2", "soll 1 Element gefunden haben");

    delete(src);
}
