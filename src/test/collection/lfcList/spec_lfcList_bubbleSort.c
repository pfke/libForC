#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcList__lfcList_bubbleSort

static int compare_int_fn (
    void *a,
    void *b
) {
    int i_a = (int)a;
    int i_b = (int)b;

    if (i_a < i_b) { return -1; }
    if (i_a > i_b) { return 1; }

    return 0;
}

Test(
    TEST_SUITE_NAME,
    on_empty_list
) {
    lfcList_t *tto = lfcList_ctor();

    should_be_same_int(lfcList_bubbleSort(tto, compare_int_fn), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_sorted
) {
    lfcList_t *tto = lfcList_ctor();
    lfcList_add(tto, (void *)1);
    lfcList_add(tto, (void *)2);
    lfcList_add(tto, (void *)3);
    lfcList_add(tto, (void *)4);
    lfcList_add(tto, (void *)5);
    lfcList_add(tto, (void *)6);

    should_be_same_int(lfcList_bubbleSort(tto, compare_int_fn), 0);
    should_be_same_int((int)lfcList_getAt(tto, 0), 1);
    should_be_same_int((int)lfcList_getAt(tto, 1), 2);
    should_be_same_int((int)lfcList_getAt(tto, 2), 3);
    should_be_same_int((int)lfcList_getAt(tto, 3), 4);
    should_be_same_int((int)lfcList_getAt(tto, 4), 5);
    should_be_same_int((int)lfcList_getAt(tto, 5), 6);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_unsorted
) {
    lfcList_t *tto = lfcList_ctor();
    lfcList_add(tto, (void *)2);
    lfcList_add(tto, (void *)5);
    lfcList_add(tto, (void *)3);
    lfcList_add(tto, (void *)4);
    lfcList_add(tto, (void *)6);
    lfcList_add(tto, (void *)1);

    should_be_same_int(lfcList_bubbleSort(tto, compare_int_fn), 0);
    should_be_same_int((int)lfcList_getAt(tto, 0), 1);
    should_be_same_int((int)lfcList_getAt(tto, 1), 2);
    should_be_same_int((int)lfcList_getAt(tto, 2), 3);
    should_be_same_int((int)lfcList_getAt(tto, 3), 4);
    should_be_same_int((int)lfcList_getAt(tto, 4), 5);
    should_be_same_int((int)lfcList_getAt(tto, 5), 6);

    delete(tto);
}
