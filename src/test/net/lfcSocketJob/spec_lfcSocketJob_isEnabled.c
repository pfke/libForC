#include <net/lfcSocketJobReader.h>
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcSocketJob_isEnabled

Test(
    TEST_SUITE_NAME,
    after_ctor
) {
    lfcSocketJob_t *tto = (lfcSocketJob_t*)new(lfcSocketJob(), 1345, NULL, NULL, 2, 123);

    should_be_true(lfcSocketJob_isEnabled(tto));

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    after_disable
) {
    lfcSocketJob_t *tto = (lfcSocketJob_t*)new(lfcSocketJob(), 1345, NULL, NULL, 2, 123);

    lfcSocketJob_disable(tto);
    should_be_false(lfcSocketJob_isEnabled(tto));

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    after_enable
) {
    lfcSocketJob_t *tto = (lfcSocketJob_t*)new(lfcSocketJob(), 1345, NULL, NULL, 2, 123);

    lfcSocketJob_disable(tto);
    lfcSocketJob_enable(tto);
    should_be_true(lfcSocketJob_isEnabled(tto));

    delete(tto);
}
