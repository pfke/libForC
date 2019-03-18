#include <collection/lfcHashmap.h>
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcHashmap__lfcHashmap_remove

Test(
    TEST_SUITE_NAME,
    onEmpty
) {
    lfcHashmap_t *tto = lfcHashmap_ctor(lfcHashmap_hash_string);

    should_not_be_same_int_wText(lfcHashmap_methods()->remove(tto, "1st key"), 0, "put should return != 0");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    nonEmpty_existing_return0
) {
    lfcHashmap_t *tto = lfcHashmap_ctorWithSize(1, lfcHashmap_hash_string);
    lfcHashmap_methods()->put(tto, "1st key", "huhu1");
    lfcHashmap_put(tto, "2nd key", "huhu2");
    lfcHashmap_put(tto, "3rd key", "huhu3");
    lfcHashmap_put(tto, "4th key", "huhu4");
    lfcHashmap_put(tto, "5th key", "huhu5");
    lfcHashmap_put(tto, "6th key", "huhu6");

    should_be_same_int_wText(lfcHashmap_remove(tto, "4th key"), 0, "put should return == 0");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    nonEmpty_existing_checkRemoved
) {
    lfcHashmap_t *tto = lfcHashmap_ctorWithSize(1, lfcHashmap_hash_string);
    lfcHashmap_methods()->put(tto, "1st key", "huhu1");
    lfcHashmap_put(tto, "2nd key", "huhu2");
    lfcHashmap_put(tto, "3rd key", "huhu3");
    lfcHashmap_put(tto, "4th key", "huhu4");
    lfcHashmap_put(tto, "5th key", "huhu5");
    lfcHashmap_put(tto, "6th key", "huhu6");

    lfcHashmap_remove(tto, "4th key");

    should_be_same_int_wText(lfcHashmap_methods()->lfcIIterable->exists(tto, "4th key"), 0, "pair is not remove");

    delete(tto);
}
