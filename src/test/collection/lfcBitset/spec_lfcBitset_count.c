#include <collection/lfcBitset.h>
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcBitset_count

Test(
    TEST_SUITE_NAME,
    on_fresh_instantiated
) {
    lfcBitset_t *tto = lfcBitset_ctor();

    should_be_same_int(lfcBitset_count(tto), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    after_aSetBit
) {
    lfcBitset_t *tto = lfcBitset_ctor();
    lfcBitset_set(tto, 67);

    should_be_same_int(lfcBitset_count(tto), 8 * 9);

    delete(tto);
}
