#include "threading/actor/lfcActor.h"
#include "threading/actor/lfcActorSystem.h"
#include "testing/lfcCriterionHelper.h"
#include <unistd.h>

#define TEST_SUITE_NAME               spec_lfcActor__lfcActor_tell

Test(
    TEST_SUITE_NAME,
    playing_pingpong_w2Actors
) {
    bool read_wait_for = false;

    void my_receive_fn_cb_actor01(lfcActor_t *self isAnUnused_param, lfcActorMessage_t *msg isAnUnused_param) {
        read_wait_for = true;
    }
    void my_receive_fn_cb_actor02(lfcActor_t *self, lfcActorMessage_t *msg) {
        lfcActorRef_tell(
            lfcActor_getRef(self),
            msg->sender,
            "pong",
            strlen("pong")
        );
    }

    lfcActorSystem_t *tto_system = lfcActorSystem_ctor("jkljkl");
    lfcActorRef_t *tto_actor_01 = lfcActorSystem_create(tto_system, "actor_01", my_receive_fn_cb_actor01);
    lfcActorRef_t *tto_actor_02 = lfcActorSystem_create(tto_system, "actor_02", my_receive_fn_cb_actor02);

    const char *msg ="ping";
    lfcActorRef_tell(tto_actor_01, tto_actor_02, msg, strlen(msg));

    should_be_true_wText(
        lfcCriterionHelper_waitUntil_varIsTrue(1 * 1000000, 10000, &read_wait_for),
        "keine 10 Msg angekommen innerhalb von 1s"
    )

    delete(tto_system);
    delete(tto_actor_01);
    delete(tto_actor_02);
}

Test(
    TEST_SUITE_NAME,
    passing_more_msgs
) {
    bool read_wait_for = false;

    void my_receive_fn_cb_actor01(lfcActor_t *self isAnUnused_param, lfcActorMessage_t *msg isAnUnused_param) {}
    void my_receive_fn_cb_actor02(lfcActor_t *self isAnUnused_param, lfcActorMessage_t *msg isAnUnused_param) {
        static int counter = 0;

        counter++;

        if (counter >= 10) {
            read_wait_for = true;
        }
    }

    lfcActorSystem_t *tto_system = lfcActorSystem_ctor("jkljkl");
    lfcActorRef_t *tto_actor_01 = lfcActorSystem_create(tto_system, "actor_01", my_receive_fn_cb_actor01);
    lfcActorRef_t *tto_actor_02 = lfcActorSystem_create(tto_system, "actor_02", my_receive_fn_cb_actor02);

    const char *msg ="ping";
    lfcActorRef_tell(tto_actor_01, tto_actor_02, msg, strlen(msg));
    lfcActorRef_tell(tto_actor_01, tto_actor_02, msg, strlen(msg));
    lfcActorRef_tell(tto_actor_01, tto_actor_02, msg, strlen(msg));
    lfcActorRef_tell(tto_actor_01, tto_actor_02, msg, strlen(msg));
    lfcActorRef_tell(tto_actor_01, tto_actor_02, msg, strlen(msg));
    lfcActorRef_tell(tto_actor_01, tto_actor_02, msg, strlen(msg));
    lfcActorRef_tell(tto_actor_01, tto_actor_02, msg, strlen(msg));
    lfcActorRef_tell(tto_actor_01, tto_actor_02, msg, strlen(msg));
    lfcActorRef_tell(tto_actor_01, tto_actor_02, msg, strlen(msg));
    lfcActorRef_tell(tto_actor_01, tto_actor_02, msg, strlen(msg));

    should_be_true_wText(
        lfcCriterionHelper_waitUntil_varIsTrue(1 * 1000000, 10000, &read_wait_for),
        "keine 10 Msg angekommen innerhalb von 1s"
    )

    delete(tto_system);
    delete(tto_actor_01);
    delete(tto_actor_02);
}
