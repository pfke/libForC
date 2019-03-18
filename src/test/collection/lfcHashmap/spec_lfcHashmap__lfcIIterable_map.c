#include <core/lfcCore.h>
#include "collection/lfcHashmap.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcHashmap__IIterable_map

Test(
    TEST_SUITE_NAME,
    check_count
) {
    lfcHashmap_t *src = LFCHASH__CREATE_INTKEY_HASHMAP();
    lfcHashmap_put(src, (void *)1, (void *)1);
    lfcHashmap_put(src, (void *)2, (void *)2);
    lfcHashmap_put(src, (void *)3, (void *)3);
    lfcHashmap_put(src, (void *)4, (void *)4);
    lfcHashmap_put(src, (void *)5, (void *)5);
    lfcHashmap_t *tto = lfcIIterable_map(src, lfcifoc_FREE_OLD_COLLECION, lambda_vptr((void *item isAnIncognito_param) {
        lfcHashmap_keyValuePair_t *it = (lfcHashmap_keyValuePair_t *)item;
        it->data = (void *) (intptr_t) ((int) (intptr_t) it->data + 10);

        return it;
    }));

    should_be_same_int_wText(lfcHashmap_methods()->lfcIIterable->count(tto), 5, "should return a map of the correct size");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_class
) {
    lfcHashmap_t *src = LFCHASH__CREATE_INTKEY_HASHMAP();
    lfcHashmap_put(src, (void *)1, (void *)1);
    lfcHashmap_put(src, (void *)2, (void *)2);
    lfcHashmap_put(src, (void *)3, (void *)3);
    lfcHashmap_put(src, (void *)4, (void *)4);
    lfcHashmap_put(src, (void *)5, (void *)5);
    lfcHashmap_t *tto = lfcHashmap_methods()
        ->lfcIIterable
        ->map(src, lfcifoc_FREE_OLD_COLLECION, lambda_vptr((void *item isAnIncognito_param) {
            lfcHashmap_keyValuePair_t *pair = (lfcHashmap_keyValuePair_t *)item;

            pair->data = (void *)(intptr_t)((int)(intptr_t)pair->data + 10);

            return pair;
        }));

    should_be_same_int_wText(isInstanceOf(lfcHashmap(), tto), 1, "should be an instance of lfcHashmap");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_notSamePtr
) {
    lfcHashmap_t *src = LFCHASH__CREATE_INTKEY_HASHMAP();
    lfcHashmap_put(src, (void *)1, (void *)1);
    lfcHashmap_put(src, (void *)2, (void *)2);
    lfcHashmap_put(src, (void *)3, (void *)3);
    lfcHashmap_put(src, (void *)4, (void *)4);
    lfcHashmap_put(src, (void *)5, (void *)5);
    lfcHashmap_t *tto = lfcHashmap_methods()
        ->lfcIIterable
        ->map(src, lfcifoc_FREE_OLD_COLLECION, lambda_vptr((void *item isAnIncognito_param) {
            lfcHashmap_keyValuePair_t *pair = (lfcHashmap_keyValuePair_t *)item;

            pair->data = (void *)(intptr_t)((int)(intptr_t)pair->data + 10);

            return pair;
        }));

    should_not_be_same_ptr_wText(tto, src, "should return a new map");

    delete(tto);
}

//Test(
//    TEST_SUITE_NAME,
//    check_noFree_onFalse
//) {
//    lfcHashmap_t *src = LFCHASH__CREATE_INTKEY_HASHMAP();
//    lfcHashmap_put(src, (void *)1, (void *)1);
//    lfcHashmap_put(src, (void *)2, (void *)2);
//    lfcHashmap_put(src, (void *)3, (void *)3);
//    lfcHashmap_put(src, (void *)4, (void *)4);
//    lfcHashmap_put(src, (void *)5, (void *)5);
//    lfcHashmap_t *tto = lfcHashmap_methods()
//        ->lfcIIterable
//        ->map(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda_vptr((void *item isAnIncognito_param) {
//            lfcHashmap_keyValuePair_t *pair = (lfcHashmap_keyValuePair_t *)item;
//
//            pair->data = (void *)(intptr_t)((int)(intptr_t)pair->data + 10);
//
//            return pair;
//        }));
//
//    should_not_be_freed(src);
//
//    delete(src);
//    delete(tto);
//}

//Test(
//    TEST_SUITE_NAME,
//    check_noFree_onTrue
//) {
//    lfcHashmap_t *src = LFCHASH__CREATE_INTKEY_HASHMAP();
//    lfcHashmap_put(src, (void *)1, (void *)1);
//    lfcHashmap_put(src, (void *)2, (void *)2);
//    lfcHashmap_put(src, (void *)3, (void *)3);
//    lfcHashmap_put(src, (void *)4, (void *)4);
//    lfcHashmap_put(src, (void *)5, (void *)5);
//    lfcHashmap_t *tto = lfcIIterable_map(src, lfcifoc_FREE_OLD_COLLECION, lambda_vptr((void *item isAnIncognito_param) {
//    lfcHashmap_keyValuePair_t *it = (lfcHashmap_keyValuePair_t *)item;
//        it->data = (void *) (intptr_t) ((int) (intptr_t) it->data + 10);
//
//        return it;
//    }));
//
//    should_be_freed(src);
//
//delete(tto);
//}

Test(
    TEST_SUITE_NAME,
    check_item_1
) {
    lfcHashmap_t *src = LFCHASH__CREATE_INTKEY_HASHMAP();
    lfcHashmap_put(src, (void *)1, (void *)1);
    lfcHashmap_put(src, (void *)2, (void *)2);
    lfcHashmap_put(src, (void *)3, (void *)3);
    lfcHashmap_put(src, (void *)4, (void *)4);
    lfcHashmap_put(src, (void *)5, (void *)5);
    lfcHashmap_t *tto = lfcHashmap_methods()
        ->lfcIIterable
        ->map(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda_vptr((void *item isAnIncognito_param) {
            lfcHashmap_keyValuePair_t *pair = (lfcHashmap_keyValuePair_t *)item;

            pair->data = (void *)(intptr_t)((int)(intptr_t)pair->data + 10);

            return pair;
        }));

    should_be_same_ptr_wText(lfcHashmap_methods()->get(tto, (void *)1), (void *)11, "item #1 not correct mapped");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_item_2
) {
    lfcHashmap_t *src = LFCHASH__CREATE_INTKEY_HASHMAP();
    lfcHashmap_put(src, (void *)1, (void *)1);
    lfcHashmap_put(src, (void *)2, (void *)2);
    lfcHashmap_put(src, (void *)3, (void *)3);
    lfcHashmap_put(src, (void *)4, (void *)4);
    lfcHashmap_put(src, (void *)5, (void *)5);
    lfcHashmap_t *tto = lfcHashmap_methods()
        ->lfcIIterable
        ->map(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda_vptr((void *item isAnIncognito_param) {
            lfcHashmap_keyValuePair_t *pair = (lfcHashmap_keyValuePair_t *)item;

            pair->data = (void *)(intptr_t)((int)(intptr_t)pair->data + 10);

            return pair;
        }));

    should_be_same_ptr_wText(lfcHashmap_methods()->get(tto, (void *)2), (void *)12, "item #2 not correct mapped");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_item_3
) {
    lfcHashmap_t *src = LFCHASH__CREATE_INTKEY_HASHMAP();
    lfcHashmap_put(src, (void *)1, (void *)1);
    lfcHashmap_put(src, (void *)2, (void *)2);
    lfcHashmap_put(src, (void *)3, (void *)3);
    lfcHashmap_put(src, (void *)4, (void *)4);
    lfcHashmap_put(src, (void *)5, (void *)5);
    lfcHashmap_t *tto = lfcHashmap_methods()
        ->lfcIIterable
        ->map(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda_vptr((void *item isAnIncognito_param) {
            lfcHashmap_keyValuePair_t *pair = (lfcHashmap_keyValuePair_t *)item;

            pair->data = (void *)(intptr_t)((int)(intptr_t)pair->data + 10);

            return pair;
        }));

    should_be_same_ptr_wText(lfcHashmap_methods()->get(tto, (void *)3), (void *)13, "item #3 not correct mapped");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_item_4
) {
    lfcHashmap_t *src = LFCHASH__CREATE_INTKEY_HASHMAP();
    lfcHashmap_put(src, (void *)1, (void *)1);
    lfcHashmap_put(src, (void *)2, (void *)2);
    lfcHashmap_put(src, (void *)3, (void *)3);
    lfcHashmap_put(src, (void *)4, (void *)4);
    lfcHashmap_put(src, (void *)5, (void *)5);
    lfcHashmap_t *tto = lfcHashmap_methods()
        ->lfcIIterable
        ->map(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda_vptr((void *item isAnIncognito_param) {
            lfcHashmap_keyValuePair_t *pair = (lfcHashmap_keyValuePair_t *)item;

            pair->data = (void *)(intptr_t)((int)(intptr_t)pair->data + 10);

            return pair;
        }));

    should_be_same_ptr_wText(lfcHashmap_methods()->get(tto, (void *)4), (void *)14, "item #4 not correct mapped");

    delete(src);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_item_5
) {
    lfcHashmap_t *src = LFCHASH__CREATE_INTKEY_HASHMAP();
    lfcHashmap_put(src, (void *)1, (void *)1);
    lfcHashmap_put(src, (void *)2, (void *)2);
    lfcHashmap_put(src, (void *)3, (void *)3);
    lfcHashmap_put(src, (void *)4, (void *)4);
    lfcHashmap_put(src, (void *)5, (void *)5);
    lfcHashmap_t *tto = lfcHashmap_methods()
        ->lfcIIterable
        ->map(src, lfcifoc_DONT_FREE_OLD_COLLECION, lambda_vptr((void *item isAnIncognito_param) {
            lfcHashmap_keyValuePair_t *pair = (lfcHashmap_keyValuePair_t *)item;

            pair->data = (void *)(intptr_t)((int)(intptr_t)pair->data + 10);

            return pair;
        }));

    should_be_same_ptr_wText(lfcHashmap_methods()->get(tto, (void *)5), (void *)15, "item #5 not correct mapped");

    delete(src);
    delete(tto);
}
