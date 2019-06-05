#include <ipc/lfcMQueueReceiver.h>
#include <net/lfcSocketHandler.h>
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcSocketHandler_ctor

/**
 * --single=spec_lfcSocketHandler_singleton::singleton_should_return_not_null
 */
Test(
    TEST_SUITE_NAME,
    singleton_should_return_not_null
) {
    lfcSocketHandler_t *tto = lfcSocketHandler_ctor();

    should_not_be_null(tto);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    not_a_singleton
) {
    lfcSocketHandler_t *tto = lfcSocketHandler_ctor();
    lfcSocketHandler_t *tto2 = lfcSocketHandler_ctor();

    should_not_be_same_ptr(tto, tto2);

    delete(tto);
    delete(tto2);
}
