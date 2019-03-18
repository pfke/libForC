#include <collection/lfcHashmap.h>
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcHashmap__lfcHashmap_put

Test(
    TEST_SUITE_NAME,
    add_one_item__return_0
) {
    lfcHashmap_t *tto = lfcHashmap_ctor(lfcHashmap_hash_string);

    should_be_same_int_wText(lfcHashmap_put(tto, "1st key", "huhu"), 0, "put should return 0 when ok");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    add_one_item__inc_size
) {
    lfcHashmap_t *tto = lfcHashmap_ctor(lfcHashmap_hash_string);
    lfcHashmap_put(tto, "1st key", "huhu");

    should_be_same_int_wText(tto->size, 1, "size should be incremented");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    add_one_item__shouldBeAdded
) {
    lfcHashmap_t *tto = lfcHashmap_ctor(lfcHashmap_hash_string);
    lfcHashmap_put(tto, "1st key", "huhu");

    should_be_same_ptr_wText(lfcHashmap_get(tto, "1st key"), "huhu", "value should be added");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_increase__1st_item
) {
    lfcHashmap_t *tto = lfcHashmap_ctorWithSize(1, lfcHashmap_hash_string);
    lfcHashmap_methods()->put(tto, "1st key", "huhu1");
    lfcHashmap_put(tto, "2nd key", "huhu2");
    lfcHashmap_put(tto, "3rd key", "huhu3");
    lfcHashmap_put(tto, "4th key", "huhu4");
    lfcHashmap_put(tto, "5th key", "huhu5");
    lfcHashmap_put(tto, "6th key", "huhu6");

    should_be_same_ptr_wText(lfcHashmap_methods()->get(tto, "1st key"), "huhu1", "value should be added");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_increase__2nd_item
) {
    lfcHashmap_t *tto = lfcHashmap_ctorWithSize(1, lfcHashmap_hash_string);
    lfcHashmap_put(tto, "1st key", "huhu1");
    lfcHashmap_put(tto, "2nd key", "huhu2");
    lfcHashmap_put(tto, "3rd key", "huhu3");
    lfcHashmap_put(tto, "4th key", "huhu4");
    lfcHashmap_put(tto, "5th key", "huhu5");
    lfcHashmap_put(tto, "6th key", "huhu6");

    should_be_same_ptr_wText(lfcHashmap_get(tto, "2nd key"), "huhu2", "value should be added");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_increase__3rd_item
) {
    lfcHashmap_t *tto = lfcHashmap_ctorWithSize(1, lfcHashmap_hash_string);
    lfcHashmap_put(tto, "1st key", "huhu1");
    lfcHashmap_put(tto, "2nd key", "huhu2");
    lfcHashmap_put(tto, "3rd key", "huhu3");
    lfcHashmap_put(tto, "4th key", "huhu4");
    lfcHashmap_put(tto, "5th key", "huhu5");
    lfcHashmap_put(tto, "6th key", "huhu6");

    should_be_same_ptr_wText(lfcHashmap_get(tto, "3rd key"), "huhu3", "value should be added");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_increase__4th_item
) {
    lfcHashmap_t *tto = lfcHashmap_ctorWithSize(1, lfcHashmap_hash_string);
    lfcHashmap_put(tto, "1st key", "huhu1");
    lfcHashmap_put(tto, "2nd key", "huhu2");
    lfcHashmap_put(tto, "3rd key", "huhu3");
    lfcHashmap_put(tto, "4th key", "huhu4");
    lfcHashmap_put(tto, "5th key", "huhu5");
    lfcHashmap_put(tto, "6th key", "huhu6");

    should_be_same_ptr_wText(lfcHashmap_get(tto, "4th key"), "huhu4", "value should be added");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_increase__5th_item
) {
    lfcHashmap_t *tto = lfcHashmap_ctorWithSize(1, lfcHashmap_hash_string);
    lfcHashmap_put(tto, "1st key", "huhu1");
    lfcHashmap_put(tto, "2nd key", "huhu2");
    lfcHashmap_put(tto, "3rd key", "huhu3");
    lfcHashmap_put(tto, "4th key", "huhu4");
    lfcHashmap_put(tto, "5th key", "huhu5");
    lfcHashmap_put(tto, "6th key", "huhu6");

    should_be_same_ptr_wText(lfcHashmap_get(tto, "5th key"), "huhu5", "value should be added");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_increase__6th_item
) {
    lfcHashmap_t *tto = lfcHashmap_ctorWithSize(1, lfcHashmap_hash_string);
    lfcHashmap_put(tto, "1st key", "huhu1");
    lfcHashmap_put(tto, "2nd key", "huhu2");
    lfcHashmap_put(tto, "3rd key", "huhu3");
    lfcHashmap_put(tto, "4th key", "huhu4");
    lfcHashmap_put(tto, "5th key", "huhu5");
    lfcHashmap_put(tto, "6th key", "huhu6");

    should_be_same_ptr_wText(lfcHashmap_get(tto, "6th key"), "huhu6", "value should be added");

    delete(tto);
}
