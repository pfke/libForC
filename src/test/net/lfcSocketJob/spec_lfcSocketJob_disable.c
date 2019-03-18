#include <net/lfcSocketJobReader.h>
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcSocketJob_disable

Test(
    TEST_SUITE_NAME,
    disable_from_true
) {
    lfcSocketJob_t *tto = (lfcSocketJob_t*)new(lfcSocketJob(), 1345, NULL, NULL, 2, 123);

    tto->enabled = true;
    should_be_true(lfcSocketJob_isEnabled(tto));
    should_be_true(lfcSocketJob_disable(tto));
    should_be_false(lfcSocketJob_isEnabled(tto));

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    disable_from_false
) {
    lfcSocketJob_t *tto = (lfcSocketJob_t*)new(lfcSocketJob(), 1345, NULL, NULL, 2, 123);

    tto->enabled = false;
    should_be_false(lfcSocketJob_isEnabled(tto));
    should_be_true(lfcSocketJob_disable(tto));
    should_be_false(lfcSocketJob_isEnabled(tto));

    delete(tto);
}
