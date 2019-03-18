#include <core/lfcCore.h>
#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcList__IIterable_map

Test(
    TEST_SUITE_NAME,
    check_count
) {
    lfcList_t *src = lfcList_ctor();
    lfcList_add(src, (void *)1);
    lfcList_add(src, (void *)2);
    lfcList_add(src, (void *)3);
    lfcList_add(src, (void *)4);
    lfcList_add(src, (void *)5);
    lfcList_t *tto = lfcIIterable_map(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(void *, (void *item isAnIncognito_param) {
        return (void *)(intptr_t) (item + 10);
    }));

    should_be_same_int_wText(lfcIIterable_count(tto), 5, "soll 5 Element gemappt haben");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_notTheSame
) {
    lfcList_t *src = lfcList_ctor();
    lfcList_add(src, (void *)1);
    lfcList_add(src, (void *)2);
    lfcList_add(src, (void *)3);
    lfcList_add(src, (void *)4);
    lfcList_add(src, (void *)5);
    lfcList_t *tto = lfcIIterable_map(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(void *, (void *item isAnIncognito_param) {
        return (void *)(intptr_t)(((int)(intptr_t)item) + 10);
    }));

    should_not_be_same_ptr_wText(src, tto, "gibt eine andere Liste zurueck");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_1stItem
) {
    lfcList_t *src = lfcList_ctor();
    lfcList_add(src, (void *)1);
    lfcList_add(src, (void *)2);
    lfcList_add(src, (void *)3);
    lfcList_add(src, (void *)4);
    lfcList_add(src, (void *)5);
    lfcList_t *tto = lfcIIterable_map(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(void *, (void *item isAnIncognito_param) {
        return (void *)(intptr_t)(((int)(intptr_t)item) + 10);
    }));

    should_be_same_int_wText((int)(intptr_t)lfcList_methods()->getAt(tto, 0), 11, "correct 1st item");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_2ndItem
) {
    lfcList_t *src = lfcList_ctor();
    lfcList_add(src, (void *)1);
    lfcList_add(src, (void *)2);
    lfcList_add(src, (void *)3);
    lfcList_add(src, (void *)4);
    lfcList_add(src, (void *)5);
    lfcList_t *tto = lfcIIterable_map(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(void *, (void *item isAnIncognito_param) {
        return (void *)(intptr_t)(((int)(intptr_t)item) + 10);
    }));

    should_be_same_int_wText((int)(intptr_t)lfcList_methods()->getAt(tto, 1), 12, "correct 2nd item");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_3rdItem
) {
    lfcList_t *src = lfcList_ctor();
    lfcList_add(src, (void *)1);
    lfcList_add(src, (void *)2);
    lfcList_add(src, (void *)3);
    lfcList_add(src, (void *)4);
    lfcList_add(src, (void *)5);
    lfcList_t *tto = lfcIIterable_map(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(void *, (void *item isAnIncognito_param) {
        return (void *)(intptr_t)(((int)(intptr_t)item) + 10);
    }));

    should_be_same_int_wText((int)(intptr_t)lfcList_methods()->getAt(tto, 2), 13, "correct 3rd item");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_4thItem
) {
    lfcList_t *src = lfcList_ctor();
    lfcList_add(src, (void *)1);
    lfcList_add(src, (void *)2);
    lfcList_add(src, (void *)3);
    lfcList_add(src, (void *)4);
    lfcList_add(src, (void *)5);
    lfcList_t *tto = lfcIIterable_map(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(void *, (void *item isAnIncognito_param) {
        return (void *)(intptr_t)(((int)(intptr_t)item) + 10);
    }));

    should_be_same_int_wText((int)(intptr_t)lfcList_methods()->getAt(tto, 3), 14, "correct 4th item");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_5thItem
) {
    lfcList_t *src = lfcList_ctor();
    lfcList_add(src, (void *)1);
    lfcList_add(src, (void *)2);
    lfcList_add(src, (void *)3);
    lfcList_add(src, (void *)4);
    lfcList_add(src, (void *)5);
    lfcList_t *tto = lfcIIterable_map(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(void *, (void *item isAnIncognito_param) {
        return (void *)(intptr_t)(((int)(intptr_t)item) + 10);
    }));

    should_be_same_int_wText((int)(intptr_t)lfcList_methods()->getAt(tto, 4), 15, "correct 5th item");

    delete(src);
    delete(tto);
}
