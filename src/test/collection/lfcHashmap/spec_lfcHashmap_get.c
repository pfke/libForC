#include <collection/lfcHashmap.h>
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcHashmap__lfcHashmap_get

Test(
    TEST_SUITE_NAME,
    get_non_existing
) {
    lfcHashmap_t *tto = lfcHashmap_ctor(lfcHashmap_hash_string);

    should_be_null(lfcHashmap_methods()->get(tto, "nope"));

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    get_existing__w1Item
) {
    lfcHashmap_t *tto = lfcHashmap_ctor(lfcHashmap_hash_string);
    lfcHashmap_put(tto, "1st key", "huhu");

    should_be_same_ptr_wText(lfcHashmap_get(tto, "1st key"), "huhu", "should return the correct value");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    get_existing__wxItem
) {
    lfcHashmap_t *tto = lfcHashmap_ctor(lfcHashmap_hash_string);
    lfcHashmap_put(tto, "1st key", "huhu1");
    lfcHashmap_put(tto, "2nd key", "huhu2");
    lfcHashmap_put(tto, "3rd key", "huhu3");
    lfcHashmap_put(tto, "4th key", "huhu4");

    should_be_same_ptr_wText(lfcHashmap_get(tto, "3rd key"), "huhu3", "should return the correct value");

    delete(tto);
}
