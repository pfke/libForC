#include <core/lfcCore.h>
#include "collection/lfcHashmap.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcHashmap__IIterable_foreach

Test(
    TEST_SUITE_NAME,
    on_emptyList
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_INTKEY_HASHMAP();
    int result = 0;

    lfcIIterable_foreach(tto, lambda(void, (void *item isAnIncognito_param) {
        lfcHashmap_keyValuePair_t *it = (lfcHashmap_keyValuePair_t *)item;

        result <<= 8;
        result += (int) (intptr_t) it->data;
    }));

    should_be_same_int_wText(result, 0, "soll alle Element durchlaufen haben");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_INTKEY_HASHMAP();
    lfcHashmap_put(tto, (void *)1, (void *)1);
    lfcHashmap_put(tto, (void *)2, (void *)2);
    lfcHashmap_put(tto, (void *)3, (void *)3);
    int result = 0;

    lfcIIterable_foreach(tto, lambda(void, (void *item isAnIncognito_param) {
        lfcHashmap_keyValuePair_t *it = (lfcHashmap_keyValuePair_t *)item;

        result += (int) (intptr_t) it->data;
    }));

    should_be_same_int_wText(result, 6, "soll alle Element durchlaufen haben");

    delete(tto);
}
