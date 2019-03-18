#include <collection/lfcBitset.h>
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcBitset_get

Test(
    TEST_SUITE_NAME,
    get_on_fresh_intpantiated
) {
    lfcBitset_t *tto = lfcBitset_ctor();

    should_be_same_int(lfcBitset_get(tto, 8), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    get_after_aSetBit
) {
    lfcBitset_t *tto = lfcBitset_ctor();
    lfcBitset_set(tto, 67);

    should_be_same_int(lfcBitset_get(tto, 67), 1);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    get_after_aClearBit
) {
    lfcBitset_t *tto = lfcBitset_ctor();
    lfcBitset_set(tto, 67);
    lfcBitset_clear(tto, 67);

    should_be_same_int(lfcBitset_get(tto, 67), 0);

    delete(tto);
}
