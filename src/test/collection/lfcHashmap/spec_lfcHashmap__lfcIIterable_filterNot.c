#include <core/lfcCore.h>
#include "collection/lfcHashmap.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcHashmap__IIterable_filterNot

Test(
    TEST_SUITE_NAME,
    on_emptyList
) {
    lfcHashmap_t *src = LFCHASH__CREATE_STRINGKEY_HASHMAP();
    lfcHashmap_t *tto = lfcHashmap_methods()
        ->lfcIIterable
        ->filterNot(src, lfcifoc_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
            lfcHashmap_keyValuePair_t *pair = (lfcHashmap_keyValuePair_t *)item;

            return !strncmp(pair->data, "_1", 2);
        }));

    should_be_same_int_wText(lfcIIterable_count(tto), 0, "gibt eine leere Liste zurueck");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_emptyList_notSameList
) {
    lfcHashmap_t *src = LFCHASH__CREATE_STRINGKEY_HASHMAP();
    lfcHashmap_t *tto = lfcIIterable_filterNot(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
        lfcHashmap_keyValuePair_t *pair = (lfcHashmap_keyValuePair_t *)item;

        return !strncmp(pair->data, "_1", 2);
    }));

    should_not_be_same_ptr_wText(src, tto, "gibt eine andere Liste zurueck");

    delete(src);
    delete(tto);
}

//Test(
//    TEST_SUITE_NAME,
//    on_emptyList_freeSource
//) {
//    lfcHashmap_t *src = LFCHASH__CREATE_STRINGKEY_HASHMAP();
//    lfcHashmap_t *tto = lfcIIterable_filterNot(src, lfcifoc_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
//        lfcHashmap_keyValuePair_t *pair = (lfcHashmap_keyValuePair_t *)item;
//
//        return !strncmp(pair->data, "_1", 2);
//    }));
//
//    should_be_freed(src);
//
//    delete(tto);
//}

//Test(
//    TEST_SUITE_NAME,
//    on_emptyList_dontFreeSource
//) {
//    lfcHashmap_t *src = LFCHASH__CREATE_STRINGKEY_HASHMAP();
//    lfcHashmap_t *tto = lfcIIterable_filterNot(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
//        lfcHashmap_keyValuePair_t *pair = (lfcHashmap_keyValuePair_t *)item;
//
//        return !strncmp(pair->data, "_1", 2);
//    }));
//
//    should_not_be_freed(src);
//
//    delete(src);
//    delete(tto);
//}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_filter1_testCount
) {
    lfcHashmap_t *src = LFCHASH__CREATE_STRINGKEY_HASHMAP();
    lfcHashmap_put(src, "1", "_1");
    lfcHashmap_put(src, "2", "_2");
    lfcHashmap_put(src, "3", "_3");
    lfcHashmap_t *tto = lfcIIterable_filterNot(src, lfcifoc_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
        lfcHashmap_keyValuePair_t *it = (lfcHashmap_keyValuePair_t *)item;
        return !strncmp(it->data, "_1", 2);
    }));

    should_be_same_int_wText(lfcIIterable_count(tto), 2, "soll 2 Element gefiltert haben");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_filter1_testItem1
) {
    lfcHashmap_t *src = LFCHASH__CREATE_STRINGKEY_HASHMAP();
    lfcHashmap_put(src, "1", "_1");
    lfcHashmap_put(src, "2", "_2");
    lfcHashmap_put(src, "3", "_3");
    lfcHashmap_t *tto = lfcIIterable_filterNot(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
        lfcHashmap_keyValuePair_t *pair = (lfcHashmap_keyValuePair_t *)item;

        return !strncmp(pair->data, "_1", 2);
    }));

    should_be_same_int_wText(lfcIIterable_exists(tto, "2"), 1, "soll 1. Element gefiltert haben");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_filter1_testItem2
) {
    lfcHashmap_t *src = LFCHASH__CREATE_STRINGKEY_HASHMAP();
    lfcHashmap_put(src, "1", "_1");
    lfcHashmap_put(src, "2", "_2");
    lfcHashmap_put(src, "3", "_3");
    lfcHashmap_t *tto = lfcIIterable_filterNot(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
        lfcHashmap_keyValuePair_t *pair = (lfcHashmap_keyValuePair_t *)item;

        return !strncmp(pair->data, "_1", 2);
    }));

    should_be_same_int_wText(lfcIIterable_exists(tto, "3"), 1, "soll 2. Element gefiltert haben");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_filterMore_testItem1
) {
    lfcHashmap_t *src = LFCHASH__CREATE_INTKEY_HASHMAP();
    lfcHashmap_put(src, (void *)1, "1");
    lfcHashmap_put(src, (void *)2, "_2");
    lfcHashmap_put(src, (void *)3, "3");
    lfcHashmap_put(src, (void *)4, "_4");
    lfcHashmap_put(src, (void *)5, "%");
    lfcHashmap_t *tto = lfcIIterable_filterNot(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
        lfcHashmap_keyValuePair_t *pair = (lfcHashmap_keyValuePair_t *)item;

        return !strncmp(pair->data, "_", 1);
    }));

    should_be_same_ptr_wText(lfcHashmap_get(tto, (void *)1), "1", "soll das richtige 1. Element haben");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_filterMore_testItem2
) {
    lfcHashmap_t *src = LFCHASH__CREATE_INTKEY_HASHMAP();
    lfcHashmap_put(src, (void *)1, "1");
    lfcHashmap_put(src, (void *)2, "_2");
    lfcHashmap_put(src, (void *)3, "3");
    lfcHashmap_put(src, (void *)4, "_4");
    lfcHashmap_put(src, (void *)5, "%");
    lfcHashmap_t *tto = lfcIIterable_filterNot(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
        lfcHashmap_keyValuePair_t *pair = (lfcHashmap_keyValuePair_t *)item;

        return !strncmp(pair->data, "_", 1);
    }));

    should_be_same_ptr_wText(lfcHashmap_get(tto, (void *)3), "3", "soll das richtige 2. Element haben");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_filterMore_testItem3
) {
    lfcHashmap_t *src = LFCHASH__CREATE_INTKEY_HASHMAP();
    lfcHashmap_put(src, (void *)1, "1");
    lfcHashmap_put(src, (void *)2, "_2");
    lfcHashmap_put(src, (void *)3, "3");
    lfcHashmap_put(src, (void *)4, "_4");
    lfcHashmap_put(src, (void *)5, "%");
    lfcHashmap_t *tto = lfcIIterable_filterNot(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda(int, (void *item isAnIncognito_param) {
        lfcHashmap_keyValuePair_t *pair = (lfcHashmap_keyValuePair_t *)item;

        return !strncmp(pair->data, "_", 1);
    }));

    should_be_same_ptr_wText(lfcHashmap_get(tto, (void *)5), "%", "soll das richtige 2. Element haben");

    delete(src);
    delete(tto);
}
