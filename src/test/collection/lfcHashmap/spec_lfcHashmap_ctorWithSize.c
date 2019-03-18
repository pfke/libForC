#include <collection/lfcHashmap.h>
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcHashmap__lfcHashmap_ctorWithSize

Test(
    TEST_SUITE_NAME,
    return_not_NIL
) {
    lfcHashmap_t *tto = lfcHashmap_ctorWithSize(13, lfcHashmap_hash_int);

    should_not_be_null(tto);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    return_obj_w_correct_name
) {
    lfcHashmap_t *tto = lfcHashmap_ctorWithSize(13, lfcHashmap_hash_int);

    should_be_same_str_wText(nameOf(getClass(tto)), "lfcHashmap", "class name not correctly set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    return_obj_w_correct_class
) {
    lfcHashmap_t *tto = lfcHashmap_ctorWithSize(13, lfcHashmap_hash_int);

    should_be_same_ptr_wText(tto->_.class, lfcHashmap(), "class not correctly set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    correct_initial_table
) {
    lfcHashmap_t *tto = lfcHashmap_ctorWithSize(13, lfcHashmap_hash_int);

    should_not_be_null(tto->table);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    correct_initial_size
) {
    lfcHashmap_t *tto = lfcHashmap_ctorWithSize(13, lfcHashmap_hash_int);

    should_be_same_int_wText(tto->size, 0, "initial 'size' not set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    correct_initial_table_size
) {
    lfcHashmap_t *tto = lfcHashmap_ctorWithSize(13, lfcHashmap_hash_int);

    should_be_same_int_wText(tto->table_size, 13, "initial 'table_size' not set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    pass_0_return_not_nil
) {
    lfcHashmap_t *tto = lfcHashmap_ctorWithSize(0, lfcHashmap_hash_int);

    should_not_be_null(tto);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    pass_0_correct_initial_table_size
) {
    lfcHashmap_t *tto = lfcHashmap_ctorWithSize(0, lfcHashmap_hash_int);

    should_be_same_int_wText(tto->table_size, DEFAULT_HASHMAP_INITIAL_SIZE, "initial 'table_size' not set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    define__LFCHASH__CREATE_INTKEY_HASHMAP__set_correct_hash_fn
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_INTKEY_HASHMAP();

    should_be_same_ptr_wText(tto->hash_fn, lfcHashmap_hash_int, "hash fn not correct");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    define__LFCHASH__CREATE_PTRKEY_HASHMAP__set_correct_hash_fn
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_PTRKEY_HASHMAP();

    should_be_same_ptr_wText(tto->hash_fn, lfcHashmap_hash_ptr, "hash fn not correct");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    define__LFCHASH__CREATE_STRINGKEY_HASHMAP__set_correct_hash_fn
) {
    lfcHashmap_t *tto = LFCHASH__CREATE_STRINGKEY_HASHMAP();

    should_be_same_ptr_wText(tto->hash_fn, lfcHashmap_hash_string, "hash fn not correct");

    delete(tto);
}
