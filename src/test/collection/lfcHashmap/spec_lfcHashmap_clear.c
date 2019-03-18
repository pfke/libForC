#include <collection/lfcHashmap.h>
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcHashmap__lfcHashmap_clear

Test(
    TEST_SUITE_NAME,
    on_filled__return_0
) {
    lfcHashmap_t *tto = lfcHashmap_ctor(lfcHashmap_hash_string);
    lfcHashmap_put(tto, "1st key", "huhu1");
    lfcHashmap_put(tto, "2nd key", "huhu2");
    lfcHashmap_put(tto, "3rd key", "huhu3");
    lfcHashmap_put(tto, "4th key", "huhu4");

    should_be_same_int(lfcHashmap_methods()->clear(tto), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    on_filled__set_size_to_0
) {
    lfcHashmap_t *tto = lfcHashmap_ctor(lfcHashmap_hash_string);
    lfcHashmap_put(tto, "1st key", "huhu1");
    lfcHashmap_put(tto, "2nd key", "huhu2");
    lfcHashmap_put(tto, "3rd key", "huhu3");
    lfcHashmap_put(tto, "4th key", "huhu4");
    lfcHashmap_clear(tto);

    should_be_same_int(tto->size, 0);

    delete(tto);
}
