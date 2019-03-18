#include "collection/lfcHashmap.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcHashmap__IIterable_count

Test(
    TEST_SUITE_NAME,
    on_emptyList
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_STRINGKEY_HASHMAP();

    should_be_same_int(lfcIIterable_count(tto), 0);
    lfcHashmap_methods()->lfcIIterable->count(tto);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_STRINGKEY_HASHMAP();
    lfcHashmap_put(tto, "1", "lkkökö");

    should_be_same_int(lfcIIterable_count(tto), 1);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_wrongType
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_STRINGKEY_HASHMAP();
    lfcHashmap_put(tto, "1", "lkkökö");

    should_be_same_int(lfcIIterable_count("klööök"), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmptyList_nullType
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_STRINGKEY_HASHMAP();
    lfcHashmap_put(tto, "1", "lkkökö");

    should_be_same_int(lfcIIterable_count(NULL), 0);

    delete(tto);
}
