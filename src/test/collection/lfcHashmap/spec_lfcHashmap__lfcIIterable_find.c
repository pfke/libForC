#include <core/lfcCore.h>
#include "collection/lfcHashmap.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcHashmap__IIterable_find

Test(
    TEST_SUITE_NAME,
    on_emptyList
) {
    lfcHashmap_t *src = LFCHASH__CREATE_STRINGKEY_HASHMAP();
    int *tto = lfcHashmap_methods()
        ->lfcIIterable
        ->find(src, lambda(int, (void *item isAnIncognito_param) {
            lfcHashmap_keyValuePair_t *pair = (lfcHashmap_keyValuePair_t *)item;

            return !strncmp(pair->data, "_1", 2);
        }));

    should_be_null(tto);

    delete(src);
}

//Test(
//    TEST_SUITE_NAME,
//    on_emptyList_dontFreeSource
//) {
//    lfcHashmap_t *src = LFCHASH__CREATE_STRINGKEY_HASHMAP();
//    lfcIIterable_find(src, lambda(int, (void *item isAnIncognito_param) {
//        lfcHashmap_keyValuePair_t *pair = (lfcHashmap_keyValuePair_t *)item;
//
//        return !strncmp(pair->data, "_1", 2);
//    }));
//
//    should_not_be_freed(src);
//
//    delete(src);
//}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_find_testItem1_returnNotNull
) {
    lfcHashmap_t *src = LFCHASH__CREATE_STRINGKEY_HASHMAP();
    lfcHashmap_put(src, "1", "_1");
    lfcHashmap_put(src, "2", "_2");
    lfcHashmap_put(src, "3", "_3");
    lfcHashmap_keyValuePair_t *tto = lfcIIterable_find(src, lambda(int, (void *item isAnIncognito_param) {
        lfcHashmap_keyValuePair_t *pair = (lfcHashmap_keyValuePair_t *)item;

        return !strncmp(pair->data, "_3", 2);
    }));

    should_not_be_null(tto);

    delete(src);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_find_testItem1_notNull
) {
    lfcHashmap_t *src = LFCHASH__CREATE_STRINGKEY_HASHMAP();
    lfcHashmap_put(src, "1", "_1");
    lfcHashmap_put(src, "2", "_2");
    lfcHashmap_put(src, "3", "_3");
    lfcHashmap_keyValuePair_t *tto = lfcIIterable_find(src, lambda(int, (void *item isAnIncognito_param) {
        lfcHashmap_keyValuePair_t *pair = (lfcHashmap_keyValuePair_t *)item;

        return !strncmp(pair->data, "_3", 2);
    }));

    should_not_be_null(tto->data);

    delete(src);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_find_testItem1_checkValue
) {
    lfcHashmap_t *src = LFCHASH__CREATE_STRINGKEY_HASHMAP();
    lfcHashmap_put(src, "1", "_1");
    lfcHashmap_put(src, "2", "_2");
    lfcHashmap_put(src, "3", "_3");
    lfcHashmap_keyValuePair_t *tto = lfcIIterable_find(src, lambda(int, (void *item isAnIncognito_param) {
        lfcHashmap_keyValuePair_t *pair = (lfcHashmap_keyValuePair_t *)item;

        return !strncmp(pair->data, "_3", 2);
    }));

    should_be_same_str_wText(tto->data, "_3", "soll das richtige Element gefunden haben");

    delete(src);
}
