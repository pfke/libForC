#include <core/lfcCore.h>
#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcList__IIterable_filterNot

Test(
    TEST_SUITE_NAME,
    on_emptyList
) {
    lfcList_t *src = lfcList_ctor();
    lfcList_t *tto = lfcIIterable_filterNot(src, lfcifoc_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
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
    lfcList_t *src = lfcList_ctor();
    lfcList_t *tto = lfcIIterable_filterNot(src, lfcifoc_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
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
    lfcList_t *src = lfcList_ctor();
    lfcList_t *tto = lfcIIterable_filterNot(src, lfcifoc_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
        return !strncmp(item, "1", 1);
    }));

    should_be_freed(src);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_emptyList_dontFreeSource
) {
    lfcList_t *src = lfcList_ctor();
    lfcList_t *tto = lfcIIterable_filterNot(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
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
    lfcList_t *src = lfcList_ctor();
    lfcList_add(src, "1");
    lfcList_add(src, "2");
    lfcList_add(src, "3");
    lfcList_t *tto = lfcIIterable_filterNot(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
        return !strncmp(item, "1", 1);
    }));

    should_be_same_int_wText(lfcIIterable_count(tto), 2, "soll 2 Elemente gefiltert haben");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_filter1_testItem1
) {
    lfcList_t *src = lfcList_ctor();
    lfcList_add(src, "1");
    lfcList_add(src, "2");
    lfcList_add(src, "3");
    lfcList_t *tto = lfcIIterable_filterNot(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
        return !strncmp(item, "1", 1);
    }));

    should_be_same_ptr_wText(lfcList_getAt(tto, 0), "2", "soll 1 Element gefiltert haben");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_filter1_testItem2
) {
    lfcList_t *src = lfcList_ctor();
    lfcList_add(src, "1");
    lfcList_add(src, "2");
    lfcList_add(src, "3");
    lfcList_t *tto = lfcIIterable_filterNot(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
        return !strncmp(item, "1", 1);
    }));

    should_be_same_ptr_wText(lfcList_getAt(tto, 1), "3", "soll 2 Element gefiltert haben");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_filterMore_testCount
) {
    lfcList_t *src = lfcList_ctor();
    lfcList_add(src, (void *)1);
    lfcList_add(src, (void *)2);
    lfcList_add(src, (void *)3);
    lfcList_add(src, (void *)4);
    lfcList_add(src, (void *)5);
    lfcList_t *tto = lfcIIterable_filterNot(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
        return !((long int)item % 2);
    }));

    should_be_same_int_wText(lfcIIterable_count(tto), 3, "soll 3 Element gefiltert haben");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_filterMore_testItem1
) {
    lfcList_t *src = lfcList_ctor();
    lfcList_add(src, (void *)1);
    lfcList_add(src, (void *)2);
    lfcList_add(src, (void *)3);
    lfcList_add(src, (void *)4);
    lfcList_add(src, (void *)5);
    lfcList_t *tto = lfcIIterable_filterNot(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
        return !((long int)item % 2);
    }));

    should_be_same_ptr_wText(lfcList_getAt(tto, 0), (void *)1, "soll das richtige 1. Element haben");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_filterMore_testItem2
) {
    lfcList_t *src = lfcList_ctor();
    lfcList_add(src, (void *)1);
    lfcList_add(src, (void *)2);
    lfcList_add(src, (void *)3);
    lfcList_add(src, (void *)4);
    lfcList_add(src, (void *)5);
    lfcList_t *tto = lfcIIterable_filterNot(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
        return !((long int)item % 2);
    }));

    should_be_same_ptr_wText(lfcList_getAt(tto, 1), (void *)3, "soll das richtige 2. Element haben");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_filterMore_testItem3
) {
    lfcList_t *src = lfcList_ctor();
    lfcList_add(src, (void *)1);
    lfcList_add(src, (void *)2);
    lfcList_add(src, (void *)3);
    lfcList_add(src, (void *)4);
    lfcList_add(src, (void *)5);
    lfcList_t *tto = lfcIIterable_filterNot(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
        return !((long int)item % 2);
    }));

    should_be_same_ptr_wText(lfcList_getAt(tto, 2), (void *)5, "soll das richtige 3. Element haben");

    delete(src);
    delete(tto);
}
