#include "collection/lfcHashmap.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcHashmap__IIterable_last

Test(
    TEST_SUITE_NAME,
    on_empty_list
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_STRINGKEY_HASHMAP();

    should_be_null(lfcHashmap_methods()->lfcIIterable->last(tto));

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmpty_list_onlyOne
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_STRINGKEY_HASHMAP();
    lfcHashmap_put(tto, "1", "klöl");

    should_be_null(lfcIIterable_last(tto));

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmpty_list_more
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_STRINGKEY_HASHMAP();
    lfcHashmap_put(tto, "1", "klököl");
    lfcHashmap_put(tto, "2", "klököl");
    lfcHashmap_put(tto, "3", "klököl");

    should_be_null(lfcIIterable_last(tto));

    delete(tto);
}
