#include "collection/lfcHashmap.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcHashmap__IIterable_isEmpty

Test(
    TEST_SUITE_NAME,
    on_empty_list
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_STRINGKEY_HASHMAP();

    should_be_same_int(lfcHashmap_methods()->lfcIIterable->isEmpty(tto), 1);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmpty_list
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_STRINGKEY_HASHMAP();
    lfcHashmap_put(tto, "1", "jklö");

    should_be_same_int(lfcIIterable_isEmpty(tto), 0);

    delete(tto);
}
