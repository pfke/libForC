#include <collection/lfcBitset.h>
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcBitset_toggle

Test(
    TEST_SUITE_NAME,
    toggle_on_freshly_checkArray
) {
    lfcBitset_t *tto = lfcBitset_ctor();
    lfcBitset_toggle(tto, 33);

    should_not_be_null(tto->array);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    toggle_to_0_checkReturn
) {
    lfcBitset_t *tto = lfcBitset_ctor();
    lfcBitset_set(tto, 34);

    should_be_same_int(lfcBitset_toggle(tto, 33), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    toggle_to_0_checkResult
) {
    lfcBitset_t *tto = lfcBitset_ctor();
    lfcBitset_set(tto, 34);
    lfcBitset_toggle(tto, 34);

    should_be_same_int(lfcBitset_get(tto, 34), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    toggle_to_1_checkReturn
) {
    lfcBitset_t *tto = lfcBitset_ctor();

    should_be_same_int(lfcBitset_toggle(tto, 33), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    toggle_to_1_checkResult
) {
    lfcBitset_t *tto = lfcBitset_ctor();
    lfcBitset_toggle(tto, 34);

    should_be_same_int(lfcBitset_get(tto, 34), 1);

    delete(tto);
}
