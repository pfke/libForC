#include <collection/lfcHashmap.h>
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcHashmap__lfcHashmap_wIntKeys

Test(
    TEST_SUITE_NAME,
    check_put__1stITem
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_INTKEY_HASHMAP();
    lfcHashmap_put(tto, (void *)1, "huhu1");
    lfcHashmap_put(tto, (void *)2, "huhu2");
    lfcHashmap_put(tto, (void *)3, "huhu3");
    lfcHashmap_put(tto, (void *)4, "huhu4");
    lfcHashmap_put(tto, (void *)5, "huhu5");
    lfcHashmap_put(tto, (void *)6, "huhu6");

    should_be_same_ptr_wText(lfcHashmap_get(tto, (void *)1), "huhu1", "value should be added");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_increase__2nd_item
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_INTKEY_HASHMAP();
    lfcHashmap_put(tto, (void *)1, "huhu1");
    lfcHashmap_put(tto, (void *)2, "huhu2");
    lfcHashmap_put(tto, (void *)3, "huhu3");
    lfcHashmap_put(tto, (void *)4, "huhu4");
    lfcHashmap_put(tto, (void *)5, "huhu5");
    lfcHashmap_put(tto, (void *)6, "huhu6");

    should_be_same_ptr_wText(lfcHashmap_get(tto, (void *)2), "huhu2", "value should be added");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_increase__3rd_item
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_INTKEY_HASHMAP();
    lfcHashmap_put(tto, (void *)1, "huhu1");
    lfcHashmap_put(tto, (void *)2, "huhu2");
    lfcHashmap_put(tto, (void *)3, "huhu3");
    lfcHashmap_put(tto, (void *)4, "huhu4");
    lfcHashmap_put(tto, (void *)5, "huhu5");
    lfcHashmap_put(tto, (void *)6, "huhu6");

    should_be_same_ptr_wText(lfcHashmap_get(tto, (void *)3), "huhu3", "value should be added");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_increase__4th_item
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_INTKEY_HASHMAP();
    lfcHashmap_put(tto, (void *)1, "huhu1");
    lfcHashmap_put(tto, (void *)2, "huhu2");
    lfcHashmap_put(tto, (void *)3, "huhu3");
    lfcHashmap_put(tto, (void *)4, "huhu4");
    lfcHashmap_put(tto, (void *)5, "huhu5");
    lfcHashmap_put(tto, (void *)6, "huhu6");

    should_be_same_ptr_wText(lfcHashmap_get(tto, (void *)4), "huhu4", "value should be added");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_increase__5th_item
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_INTKEY_HASHMAP();
    lfcHashmap_put(tto, (void *)1, "huhu1");
    lfcHashmap_put(tto, (void *)2, "huhu2");
    lfcHashmap_put(tto, (void *)3, "huhu3");
    lfcHashmap_put(tto, (void *)4, "huhu4");
    lfcHashmap_put(tto, (void *)5, "huhu5");
    lfcHashmap_put(tto, (void *)6, "huhu6");

    should_be_same_ptr_wText(lfcHashmap_get(tto, (void *)5), "huhu5", "value should be added");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_increase__6th_item
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_INTKEY_HASHMAP();
    lfcHashmap_put(tto, (void *)1, "huhu1");
    lfcHashmap_put(tto, (void *)2, "huhu2");
    lfcHashmap_put(tto, (void *)3, "huhu3");
    lfcHashmap_put(tto, (void *)4, "huhu4");
    lfcHashmap_put(tto, (void *)5, "huhu5");
    lfcHashmap_put(tto, (void *)6, "huhu6");

    should_be_same_ptr_wText(lfcHashmap_get(tto, (void *)6), "huhu6", "value should be added");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_key_0
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_INTKEY_HASHMAP();
    lfcHashmap_put(tto, (void *)0, "huhu0");
    lfcHashmap_put(tto, (void *)1, "huhu1");
    lfcHashmap_put(tto, (void *)2, "huhu2");
    lfcHashmap_put(tto, (void *)3, "huhu3");
    lfcHashmap_put(tto, (void *)4, "huhu4");
    lfcHashmap_put(tto, (void *)5, "huhu5");
    lfcHashmap_put(tto, (void *)6, "huhu6");

    should_be_same_ptr_wText(lfcHashmap_get(tto, (void *)0), "huhu0", "value should be added");

    delete(tto);
}
