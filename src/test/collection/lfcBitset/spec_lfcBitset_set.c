#include <collection/lfcBitset.h>
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcBitset_set

Test(
    TEST_SUITE_NAME,
    set_below_8bit_check_arrayNotNull
) {
    lfcBitset_t *tto = lfcBitset_ctor();
    lfcBitset_set(tto, 1);

    should_not_be_null(tto->array);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    set_below_8bit_check_arrayCount_is1
) {
    lfcBitset_t *tto = lfcBitset_ctor();
    lfcBitset_set(tto, 7);

    should_be_same_int_wText(tto->array_count, 1, "'array_count' not set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    set_below_8bit_check_bitIsSet
) {
    lfcBitset_t *tto = lfcBitset_ctor();
    lfcBitset_set(tto, 7);

    should_be_same_int_wText(lfcBitset_get(tto, 0), 0, "bit #0 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 1), 0, "bit #1 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 2), 0, "bit #2 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 3), 0, "bit #3 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 4), 0, "bit #4 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 5), 0, "bit #5 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 6), 0, "bit #6 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 7), 1, "bit #7 not set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    set_below_16bit_check_arrayNotNull
) {
    lfcBitset_t *tto = lfcBitset_ctor();
    lfcBitset_set(tto, 15);

    should_not_be_null(tto->array);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    set_below_16bit_check_arrayCount_is1
) {
    lfcBitset_t *tto = lfcBitset_ctor();
    lfcBitset_set(tto, 9);

    should_be_same_int_wText(tto->array_count, 2, "'array_count' not set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    set_below_16bit_check_bitIsSet
) {
    lfcBitset_t *tto = lfcBitset_ctor();
    lfcBitset_set(tto, 13);

    should_be_same_int_wText(lfcBitset_get(tto,  0), 0, "bit # 0 is set");
    should_be_same_int_wText(lfcBitset_get(tto,  1), 0, "bit # 1 is set");
    should_be_same_int_wText(lfcBitset_get(tto,  2), 0, "bit # 2 is set");
    should_be_same_int_wText(lfcBitset_get(tto,  3), 0, "bit # 3 is set");
    should_be_same_int_wText(lfcBitset_get(tto,  4), 0, "bit # 4 is set");
    should_be_same_int_wText(lfcBitset_get(tto,  5), 0, "bit # 5 is set");
    should_be_same_int_wText(lfcBitset_get(tto,  6), 0, "bit # 6 is set");
    should_be_same_int_wText(lfcBitset_get(tto,  7), 0, "bit # 7 is set");
    should_be_same_int_wText(lfcBitset_get(tto,  8), 0, "bit # 8 is set");
    should_be_same_int_wText(lfcBitset_get(tto,  9), 0, "bit # 9 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 10), 0, "bit #10 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 11), 0, "bit #11 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 12), 0, "bit #12 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 13), 1, "bit #13 not set");
    should_be_same_int_wText(lfcBitset_get(tto, 14), 0, "bit #14 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 15), 0, "bit #15 is set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    set_below_32bit_check_arrayNotNull
) {
    lfcBitset_t *tto = lfcBitset_ctor();
    lfcBitset_set(tto, 29);

    should_not_be_null(tto->array);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    set_below_32bit_check_arrayCount_is1
) {
    lfcBitset_t *tto = lfcBitset_ctor();
    lfcBitset_set(tto, 25);

    should_be_same_int_wText(tto->array_count, 4, "'array_count' not set");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    set_below_32bit_check_bitIsSet
) {
    lfcBitset_t *tto = lfcBitset_ctor();
    lfcBitset_set(tto, 30);

    should_be_same_int_wText(lfcBitset_get(tto,  0), 0, "bit # 0 is set");
    should_be_same_int_wText(lfcBitset_get(tto,  1), 0, "bit # 1 is set");
    should_be_same_int_wText(lfcBitset_get(tto,  2), 0, "bit # 2 is set");
    should_be_same_int_wText(lfcBitset_get(tto,  3), 0, "bit # 3 is set");
    should_be_same_int_wText(lfcBitset_get(tto,  4), 0, "bit # 4 is set");
    should_be_same_int_wText(lfcBitset_get(tto,  5), 0, "bit # 5 is set");
    should_be_same_int_wText(lfcBitset_get(tto,  6), 0, "bit # 6 is set");
    should_be_same_int_wText(lfcBitset_get(tto,  7), 0, "bit # 7 is set");
    should_be_same_int_wText(lfcBitset_get(tto,  8), 0, "bit # 8 is set");
    should_be_same_int_wText(lfcBitset_get(tto,  9), 0, "bit # 9 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 10), 0, "bit #10 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 11), 0, "bit #11 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 12), 0, "bit #12 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 14), 0, "bit #14 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 15), 0, "bit #15 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 16), 0, "bit #16 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 17), 0, "bit #17 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 18), 0, "bit #18 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 19), 0, "bit #19 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 20), 0, "bit #20 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 21), 0, "bit #21 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 22), 0, "bit #22 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 23), 0, "bit #23 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 24), 0, "bit #24 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 25), 0, "bit #25 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 26), 0, "bit #26 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 27), 0, "bit #27 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 28), 0, "bit #28 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 29), 0, "bit #29 is set");
    should_be_same_int_wText(lfcBitset_get(tto, 30), 1, "bit #30 not set");
    should_be_same_int_wText(lfcBitset_get(tto, 31), 0, "bit #31 is set");

    delete(tto);
}
