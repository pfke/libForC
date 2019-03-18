#include <collection/lfcBitset.h>
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcBitset_clear

Test(
    TEST_SUITE_NAME,
    on_fresh_instantiated
) {
    lfcBitset_t *tto = lfcBitset_ctor();

    should_be_same_int(lfcBitset_clear(tto, 54), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    clear_on_after_aSetBit
) {
    lfcBitset_t *tto = lfcBitset_ctor();
    lfcBitset_set(tto, 67);

    should_be_same_int(lfcBitset_clear(tto, 67), 0);

    delete(tto);
}
