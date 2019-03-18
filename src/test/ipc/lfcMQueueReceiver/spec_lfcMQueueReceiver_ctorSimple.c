#include <ipc/lfcMQueueReceiver.h>
#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcMQueueReceiver__lfcMQueueReceiver_ctorSimple

/**
 * --single=lfcMQueueReceiver__lfcMQueueReceiver_ctorSimple::ctor_ret_no_null
 */
Test(
    TEST_SUITE_NAME,
    ctor_ret_no_null
) {
    mq_unlink("/MQueueReceiver__lfcMQueueReceiver_ctorSimple::ctor_ret_no_null");
    lfcMQueueReceiver_t *tto = lfcMQueueReceiver_ctorSimple("/MQueueReceiver__lfcMQueueReceiver_ctorSimple::ctor_ret_no_null");

    should_not_be_null(tto);

    delete(tto);
}
