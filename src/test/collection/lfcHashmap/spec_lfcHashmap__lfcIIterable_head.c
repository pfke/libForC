#include "collection/lfcHashmap.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcHashmap__IIterable_head

Test(
    TEST_SUITE_NAME,
    on_empty_list
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_STRINGKEY_HASHMAP();

    should_be_null(lfcHashmap_methods()->lfcIIterable->head(tto));

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_nonEmpty_list
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_STRINGKEY_HASHMAP();
    lfcHashmap_put(tto, "1", "klklö");
    lfcHashmap_put(tto, "2", "klklö");
    lfcHashmap_put(tto, "3", "klklö");

    should_be_null(lfcIIterable_head(tto));

    delete(tto);
}
