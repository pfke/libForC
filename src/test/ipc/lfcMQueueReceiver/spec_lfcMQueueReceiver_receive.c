#include <ipc/lfcMQueueReceiver.h>
#include <ipc/lfcMQueueSender.h>
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcMQueueReceiver__lfcMQueueReceiver_receive

Test(
    TEST_SUITE_NAME,
    check_one_msg_rx
) {
    const char *name = "/lfcMQueueReceiver__lfcMQueueReceiver_receive::check_one_msg_rx";

    mq_unlink(name);

    lfcMQueueReceiver_t *tto = lfcMQueueReceiver_ctorSimple(name);
    lfcMQueueSender_t *sender = lfcMQueueSender_ctorSimple(name);

    lfcMQueueSender_send(sender, "hello heiko", sizeof("hello heiko"), 0);

    should_be_same_str_wText(lfcMQueueReceiver_receive(tto, NULL, NULL), "hello heiko", "no msg received");

    delete(tto);
    delete(sender);
}

Test(
    TEST_SUITE_NAME,
    check_one_msg_rx_2queues
) {
    const char *name = "/lfcMQueueReceiver__lfcMQueueReceiver_receive::check_one_msg_rx_2queues";
    const char *name2 = "/lfcMQueueReceiver__lfcMQueueReceiver_receive::check_one_msg_rx_2queues2";

    mq_unlink(name);
    mq_unlink(name2);

    lfcMQueueReceiver_t *tto = lfcMQueueReceiver_ctorSimple(name);
    lfcMQueueReceiver_t *tto2 = lfcMQueueReceiver_ctorSimple(name2);
    lfcMQueueSender_t *sender = lfcMQueueSender_ctorSimple(name);
    lfcMQueueSender_t *sender2 = lfcMQueueSender_ctorSimple(name2);

    lfcMQueueSender_send(sender, "hello heiko", sizeof("hello heiko"), 0);
    lfcMQueueSender_send(sender2, "hello heiko2", sizeof("hello heiko2"), 0);

    should_be_same_str_wText(lfcMQueueReceiver_receive(tto, NULL, NULL), "hello heiko", "no msg received on queue 1");
    should_be_same_str_wText(lfcMQueueReceiver_receive(tto2, NULL, NULL), "hello heiko2", "no msg received on queue 2");

    delete(tto);
    delete(sender);
    delete(tto2);
    delete(sender2);
}
