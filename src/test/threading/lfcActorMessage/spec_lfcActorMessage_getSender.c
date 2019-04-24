#include "threading/lfcActor.h"
#include "threading/lfcActorSystem.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcActorMessage__lfcActorMessage_getSender

Test(
    TEST_SUITE_NAME,
    passing_null
) {
    should_be_null(lfcActorMessage_getSender(NULL));
}

Test(
    TEST_SUITE_NAME,
    passing_valid
) {
    void my_receive_fn_cb(lfcActor_t *self, lfcActorMessage_t *msg) {}

    lfcActorSystem_t *tto_system = lfcActorSystem_ctor("jkljkl");
    lfcActorRef_t *tto_actor_01 = lfcActorSystem_create(tto_system, "sumsi_01", my_receive_fn_cb);
    lfcActorRef_t *tto_actor_02 = lfcActorSystem_create(tto_system, "sumsi_02", my_receive_fn_cb);
    const char *msg = ";lköklö";
    lfcActorMessage_t *tto = lfcActorMessage_ctor(tto_actor_01, tto_actor_02, msg, strlen(msg));

    should_be_same_ptr(lfcActorMessage_getSender(tto), tto_actor_01);

    delete(system);
    delete(tto);
    delete(tto_actor_01);
    delete(tto_actor_02);
}
