#include "threading/actor/lfcActor.h"
#include "threading/actor/lfcActorSystem.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcActorMessage__lfcActorMessage_hasSender

Test(
    TEST_SUITE_NAME,
    passing_null
) {
    should_be_false(lfcActorMessage_hasSender(NULL));
}

Test(
    TEST_SUITE_NAME,
    passing_sender
) {
    void my_receive_fn_cb(lfcActor_t *self isAnUnused_param, lfcActorMessage_t *msg isAnUnused_param) {}

    lfcActorSystem_t *tto_system = lfcActorSystem_ctor("jkljkl");
    lfcActorRef_t *tto_actor_01 = lfcActorSystem_create(tto_system, "sumsi_01", my_receive_fn_cb);
    lfcActorRef_t *tto_actor_02 = lfcActorSystem_create(tto_system, "sumsi_02", my_receive_fn_cb);
    const char *msg = ";lköklö";
    lfcActorMessage_t *tto = lfcActorMessage_ctor(tto_actor_01, tto_actor_02, msg, strlen(msg));

    should_be_true(lfcActorMessage_hasSender(tto));

    delete(system);
    delete(tto);
    delete(tto_actor_01);
    delete(tto_actor_02);
}

Test(
    TEST_SUITE_NAME,
    passing_noSender
) {
    void my_receive_fn_cb(lfcActor_t *self isAnUnused_param, lfcActorMessage_t *msg isAnUnused_param) {}

    lfcActorSystem_t *tto_system = lfcActorSystem_ctor("jkljkl");
    lfcActorRef_t *tto_actor_02 = lfcActorSystem_create(tto_system, "sumsi_02", my_receive_fn_cb);
    const char *msg = ";lköklö";
    lfcActorMessage_t *tto = lfcActorMessage_ctor(NULL, tto_actor_02, msg, strlen(msg));

    should_be_false(lfcActorMessage_hasSender(tto));

    delete(system);
    delete(tto);
    delete(tto_actor_02);
}
