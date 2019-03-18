#include <collection/lfcHashmap.h>
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcHashmap__lfcHashmap_ctor

Test(
    TEST_SUITE_NAME,
    return_not_NIL
) {
    lfcHashmap_t *tto = lfcHashmap_ctor(lfcHashmap_hash_int);

    should_not_be_null(tto);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    return_obj_w_correct_name
) {
    lfcHashmap_t *tto = lfcHashmap_ctor(lfcHashmap_hash_int);

    should_be_same_str_wText(nameOf(getClass(tto)), "lfcHashmap", "class name not correctly set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    return_obj_w_correct_class
) {
    lfcHashmap_t *tto = lfcHashmap_ctor(lfcHashmap_hash_int);

    should_be_same_ptr_wText(tto->_.class, lfcHashmap(), "class not correctly set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    correct_initial_table
) {
    lfcHashmap_t *tto = lfcHashmap_ctor(lfcHashmap_hash_int);

    should_not_be_null(tto->table);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    correct_initial_size
) {
    lfcHashmap_t *tto = lfcHashmap_ctor(lfcHashmap_hash_int);

    should_be_same_int_wText(tto->size, 0, "initial 'table_size' not set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    correct_initial_table_size
) {
    lfcHashmap_t *tto = lfcHashmap_ctor(lfcHashmap_hash_int);

    should_be_same_int_wText(tto->table_size, DEFAULT_HASHMAP_INITIAL_SIZE, "initial 'table_size' not set");

    delete(tto);
}
