#include "collection/lfcHashmap.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcHashmap__IIterable_exists

Test(
    TEST_SUITE_NAME,
    on_emptyList
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_STRINGKEY_HASHMAP();

    should_be_same_int(lfcHashmap_methods()->lfcIIterable->exists(tto, "1"), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_existing
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_STRINGKEY_HASHMAP();
    lfcHashmap_put(tto, "2", "klöklö");

    should_be_same_int(lfcIIterable_exists(tto, "2"), 1);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_notExisting
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_STRINGKEY_HASHMAP();
    lfcHashmap_put(tto, "2", "klöklö");

    should_be_same_int(lfcIIterable_exists(tto, "1"), 0);

    delete(tto);
}
