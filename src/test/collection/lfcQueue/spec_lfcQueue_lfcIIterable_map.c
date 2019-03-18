#include <core/lfcCore.h>
#include "collection/lfcQueue.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcQueue__IIterable_map

Test(
    TEST_SUITE_NAME,
    check_count
) {
    lfcQueue_t *src = lfcQueue_ctor();
    lfcQueue_enqueue(src, (void *)1);
    lfcQueue_enqueue(src, (void *)2);
    lfcQueue_enqueue(src, (void *)3);
    lfcQueue_enqueue(src, (void *)4);
    lfcQueue_enqueue(src, (void *)5);
    lfcQueue_t *tto = lfcIIterable_map(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(void *, (void *item isAnIncognito_param) {
        return (void *)(intptr_t)(((int)(intptr_t)item) + 10);
    }));

    should_be_same_int_wText(lfcIIterable_count(tto), 5, "soll 5 Element gemappt haben");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_notTheSame
) {
    lfcQueue_t *src = lfcQueue_ctor();
    lfcQueue_enqueue(src, (void *)1);
    lfcQueue_enqueue(src, (void *)2);
    lfcQueue_enqueue(src, (void *)3);
    lfcQueue_enqueue(src, (void *)4);
    lfcQueue_enqueue(src, (void *)5);
    lfcQueue_t *tto = lfcIIterable_map(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(void *, (void *item isAnIncognito_param) {
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
    lfcQueue_t *src = lfcQueue_ctor();
    lfcQueue_enqueue(src, (void *)1);
    lfcQueue_enqueue(src, (void *)2);
    lfcQueue_enqueue(src, (void *)3);
    lfcQueue_enqueue(src, (void *)4);
    lfcQueue_enqueue(src, (void *)5);
    lfcQueue_t *tto = lfcIIterable_map(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(void *, (void *item isAnIncognito_param) {
        return (void *)(intptr_t)(((int)(intptr_t)item) + 10);
    }));

    should_be_same_int_wText((int)(intptr_t)lfcQueue_methods()->dequeue(tto), 11, "correct 1st item");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_2ndItem
) {
    lfcQueue_t *src = lfcQueue_ctor();
    lfcQueue_enqueue(src, (void *)1);
    lfcQueue_enqueue(src, (void *)2);
    lfcQueue_enqueue(src, (void *)3);
    lfcQueue_enqueue(src, (void *)4);
    lfcQueue_enqueue(src, (void *)5);
    lfcQueue_t *tto = lfcIIterable_map(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(void *, (void *item isAnIncognito_param) {
        return (void *)(intptr_t)(((int)(intptr_t)item) + 10);
    }));

    lfcQueue_methods()->dequeue(tto);
    should_be_same_int_wText((int)(intptr_t)lfcQueue_methods()->dequeue(tto), 12, "correct 2nd item");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_3rdItem
) {
    lfcQueue_t *src = lfcQueue_ctor();
    lfcQueue_enqueue(src, (void *)1);
    lfcQueue_enqueue(src, (void *)2);
    lfcQueue_enqueue(src, (void *)3);
    lfcQueue_enqueue(src, (void *)4);
    lfcQueue_enqueue(src, (void *)5);
    lfcQueue_t *tto = lfcIIterable_map(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(void *, (void *item isAnIncognito_param) {
        return (void *)(intptr_t)(((int)(intptr_t)item) + 10);
    }));

    lfcQueue_methods()->dequeue(tto);
    lfcQueue_methods()->dequeue(tto);
    should_be_same_int_wText((int)(intptr_t)lfcQueue_methods()->dequeue(tto), 13, "correct 3rd item");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_4thItem
) {
    lfcQueue_t *src = lfcQueue_ctor();
    lfcQueue_enqueue(src, (void *)1);
    lfcQueue_enqueue(src, (void *)2);
    lfcQueue_enqueue(src, (void *)3);
    lfcQueue_enqueue(src, (void *)4);
    lfcQueue_enqueue(src, (void *)5);
    lfcQueue_t *tto = lfcIIterable_map(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(void *, (void *item isAnIncognito_param) {
        return (void *)(intptr_t)(((int)(intptr_t)item) + 10);
    }));

    lfcQueue_methods()->dequeue(tto);
    lfcQueue_methods()->dequeue(tto);
    lfcQueue_methods()->dequeue(tto);
    should_be_same_int_wText((int)(intptr_t)lfcQueue_methods()->dequeue(tto), 14, "correct 4th item");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_5thItem
) {
    lfcQueue_t *src = lfcQueue_ctor();
    lfcQueue_enqueue(src, (void *)1);
    lfcQueue_enqueue(src, (void *)2);
    lfcQueue_enqueue(src, (void *)3);
    lfcQueue_enqueue(src, (void *)4);
    lfcQueue_enqueue(src, (void *)5);
    lfcQueue_t *tto = lfcIIterable_map(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(void *, (void *item isAnIncognito_param) {
        return (void *)(intptr_t)(((int)(intptr_t)item) + 10);
    }));

    lfcQueue_methods()->dequeue(tto);
    lfcQueue_methods()->dequeue(tto);
    lfcQueue_methods()->dequeue(tto);
    lfcQueue_methods()->dequeue(tto);
    should_be_same_int_wText((int)(intptr_t)lfcQueue_methods()->dequeue(tto), 15, "correct 5th item");

    delete(src);
    delete(tto);
}
