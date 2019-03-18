#include <collection/lfcBitset.h>
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcBitset_ctor_withBits

Test(
    TEST_SUITE_NAME,
    pass_0Args_checkReturn
) {
    lfcBitset_t *tto = lfcBitset_ctor_withBits(0);

    should_not_be_null(tto);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    pass_0Args_checkArray
) {
    lfcBitset_t *tto = lfcBitset_ctor_withBits(0);

    should_be_null(tto->array);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    pass_0Args_checkArrayCount
) {
    lfcBitset_t *tto = lfcBitset_ctor_withBits(0);

    should_be_same_int_wText(tto->array_count, 0, "'array-count' is not 0");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    pass_xArgs_checkReturn
) {
    lfcBitset_t *tto = lfcBitset_ctor_withBits(3, 1, 5, 9);

    should_not_be_null(tto);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    pass_xArgs_checkBits
) {
    lfcBitset_t *tto = lfcBitset_ctor_withBits(5, 1, 5, 9, 54, 102);

    should_be_same_int_wText(lfcBitset_get(tto,   1), 1, "1st bit not set");
    should_be_same_int_wText(lfcBitset_get(tto,   5), 1, "2nd bit not set");
    should_be_same_int_wText(lfcBitset_get(tto,   9), 1, "3rd bit not set");
    should_be_same_int_wText(lfcBitset_get(tto,  54), 1, "4th bit not set");
    should_be_same_int_wText(lfcBitset_get(tto, 102), 1, "5th bit not set");

    delete(tto);
}
