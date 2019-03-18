#include <core/lfcCore.h>
#include "collection/lfcQueue.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcQueue__IIterable_filter

Test(
    TEST_SUITE_NAME,
    on_emptyList
) {
    lfcQueue_t *src = lfcQueue_ctor();
    lfcQueue_t *tto = lfcIIterable_filter(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
        return !strncmp(item, "1", 1);
    }));

    should_be_same_int_wText(lfcIIterable_count(tto), 0, "gibt eine leere Liste zurueck");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_emptyList_notSameList
) {
    lfcQueue_t *src = lfcQueue_ctor();
    lfcQueue_t *tto = lfcIIterable_filter(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
        return !strncmp(item, "1", 1);
    }));

    should_not_be_same_ptr_wText(src, tto, "gibt eine andere Liste zurueck");

    delete(src);
    delete(tto);
}

#ifdef CHECK_ALLOC_VIA_MPROBE
Test(
    TEST_SUITE_NAME,
    on_emptyList_freeSource
) {
    lfcQueue_t *src = lfcQueue_ctor();
    lfcQueue_t *tto = lfcIIterable_filter(src, lfcifoc_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
        return !strncmp(item, "1", 1);
    }));

    should_be_freed(src);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_emptyList_dontFreeSource
) {
    lfcQueue_t *src = lfcQueue_ctor();
    lfcQueue_t *tto = lfcIIterable_filter(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
        return !strncmp(item, "1", 1);
    }));

    should_not_be_freed(src);

    delete(src);
    delete(tto);
}
#endif // #ifdef CHECK_ALLOC_VIA_MPROBE

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_filter1_testCount
) {
    lfcQueue_t *src = lfcQueue_ctor();
    lfcQueue_enqueue(src, "1");
    lfcQueue_enqueue(src, "2");
    lfcQueue_enqueue(src, "3");
    lfcQueue_t *tto = lfcIIterable_filter(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
        return !strncmp(item, "1", 1);
    }));

    should_be_same_int_wText(lfcIIterable_count(tto), 1, "soll 1 Element gefiltert haben");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_filter1_testItem
) {
    lfcQueue_t *src = lfcQueue_ctor();
    lfcQueue_enqueue(src, "1");
    lfcQueue_enqueue(src, "2");
    lfcQueue_enqueue(src, "3");
    lfcQueue_t *tto = lfcIIterable_filter(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
        return !strncmp(item, "1", 1);
    }));

    should_be_same_int_wText(lfcIIterable_exists(tto, "1"), 1, "soll 1 Element gefiltert haben");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_filterMore_testCount
) {
    lfcQueue_t *src = lfcQueue_ctor();
    lfcQueue_enqueue(src, (void *)1);
    lfcQueue_enqueue(src, (void *)2);
    lfcQueue_enqueue(src, (void *)3);
    lfcQueue_enqueue(src, (void *)4);
    lfcQueue_enqueue(src, (void *)5);
    lfcQueue_t *tto = lfcIIterable_filter(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
        return !((long int)item % 2);
    }));

    should_be_same_int_wText(lfcIIterable_count(tto), 2, "soll 2 Element gefiltert haben");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_filterMore_testItem1
) {
    lfcQueue_t *src = lfcQueue_ctor();
    lfcQueue_enqueue(src, (void *)1);
    lfcQueue_enqueue(src, (void *)2);
    lfcQueue_enqueue(src, (void *)3);
    lfcQueue_enqueue(src, (void *)4);
    lfcQueue_enqueue(src, (void *)5);
    lfcQueue_t *tto = lfcIIterable_filter(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
        return !((long int)item % 2);
    }));

    should_be_same_ptr_wText(lfcQueue_dequeue(tto), (void *)2, "soll das richtige 1. Element haben");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_filterMore_testItem2
) {
    lfcQueue_t *src = lfcQueue_ctor();
    lfcQueue_enqueue(src, (void *)1);
    lfcQueue_enqueue(src, (void *)2);
    lfcQueue_enqueue(src, (void *)3);
    lfcQueue_enqueue(src, (void *)4);
    lfcQueue_enqueue(src, (void *)5);
    lfcQueue_t *tto = lfcIIterable_filter(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
        return !((long int)item % 2);
    }));

    should_be_same_ptr_wText(lfcQueue_dequeue(tto), (void *)2, "soll das richtige 2. Element haben");

    delete(src);
    delete(tto);
}
