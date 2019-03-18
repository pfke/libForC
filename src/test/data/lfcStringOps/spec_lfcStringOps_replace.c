#include <ipc/lfcMQueueReceiver.h>
#include <ipc/lfcMQueueSender.h>
#include <data/lfcStringOps.h>
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcStringOps__lfcStringOps_replace

Test(
    TEST_SUITE_NAME,
    check_1st_param_is_null
) {
    char *result = lfcStringOps_replace(
        NULL,
        "check",
        "popel"
    );

    should_be_null(result);
}

Test(
    TEST_SUITE_NAME,
    check_2nd_param_is_null
) {
    const char *in = "/lfcMQueueReceiver__lfcMQueueReceiver_receive::check_one_msg_rx";

    char *result = lfcStringOps_replace(
        in,
        NULL,
        "popel"
    );

    should_be_null(result);
}

Test(
    TEST_SUITE_NAME,
    check_3rd_param_is_null
) {
    const char *in = "/lfcMQueueReceiver__lfcMQueueReceiver_receive::check_one_msg_rx";

    char *result = lfcStringOps_replace(
        in,
        "popel",
        NULL
    );

    should_be_null(result);
}

Test(
    TEST_SUITE_NAME,
    check_one_one_match
) {
    const char *in = "/lfcMQueueReceiver__lfcMQueueReceiver_receive::check_one_msg_rx";

    char *result = lfcStringOps_replace(
        in,
        "check",
        "popel"
    );

    should_not_be_null(result);
    should_be_same_str(result, "/lfcMQueueReceiver__lfcMQueueReceiver_receive::popel_one_msg_rx");

    free(result);
}

Test(
    TEST_SUITE_NAME,
    check_one_more_matches
) {
    const char *in = "/lfcMQueueReceiver__lfcMQueueReceiver_receive::check_one_msg_rx";

    char *result = lfcStringOps_replace(
        in,
        "ei",
        "popel"
    );

    should_not_be_null(result);
    should_be_same_str(result, "/lfcMQueueRecpopelver__lfcMQueueRecpopelver_recpopelve::check_one_msg_rx");

    free(result);
}

