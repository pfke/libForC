#include "threading/actor/lfcActorMessage.h"
#include "threading/actor/lfcActorRef.h"
#include "threading/actor/lfcActorSystem.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcActorMessage__lfcActorMessage_ctor

Test(
    TEST_SUITE_NAME,
    passing_1stArg_null
) {
    lfcActorMessage_t *tto = lfcActorMessage_ctor(NULL, NULL, NULL, 0);

    should_be_null(tto);
}

Test(
    TEST_SUITE_NAME,
    passing_2ndArg_null
) {
    void my_receive_fn_cb(lfcActor_t *self isAnUnused_param, lfcActorMessage_t *msg isAnUnused_param) {}

    lfcActorSystem_t *tto_system = lfcActorSystem_ctor("jkljkl");
    lfcActorRef_t *tto_actor_01 = lfcActorSystem_createActor(tto_system, "sumsi", my_receive_fn_cb);
    lfcActorMessage_t *tto = lfcActorMessage_ctor(tto_actor_01, NULL, NULL, 0);

    should_be_null(tto);

    delete(tto_system);
    delete(tto_actor_01);
    delete(tto_system);
}

Test(
    TEST_SUITE_NAME,
    passing_3rdArg_null
) {
    void my_receive_fn_cb(lfcActor_t *self isAnUnused_param, lfcActorMessage_t *msg isAnUnused_param) {}

    lfcActorSystem_t *tto_system = lfcActorSystem_ctor("jkljkl");
    lfcActorRef_t *tto_actor_01 = lfcActorSystem_createActor(tto_system, "sumsi_01", my_receive_fn_cb);
    lfcActorRef_t *tto_actor_02 = lfcActorSystem_createActor(tto_system, "sumsi_02", my_receive_fn_cb);
    lfcActorMessage_t *tto = lfcActorMessage_ctor(tto_actor_01, tto_actor_02, NULL, 0);

    should_be_null(tto);

    delete(tto_system);
    delete(tto_actor_01);
    delete(tto_actor_02);
    delete(tto_system);
}

Test(
    TEST_SUITE_NAME,
    passing_4thArg_null
) {
    void my_receive_fn_cb(lfcActor_t *self isAnUnused_param, lfcActorMessage_t *msg isAnUnused_param) {}

    lfcActorSystem_t *tto_system = lfcActorSystem_ctor("jkljkl");
    lfcActorRef_t *tto_actor_01 = lfcActorSystem_createActor(tto_system, "sumsi_01", my_receive_fn_cb);
    lfcActorRef_t *tto_actor_02 = lfcActorSystem_createActor(tto_system, "sumsi_02", my_receive_fn_cb);
    const char *msg = ";lköklö";
    lfcActorMessage_t *tto = lfcActorMessage_ctor(tto_actor_01, tto_actor_02, msg, 0);

    should_be_null(tto);

    delete(tto_system);
    delete(tto_actor_01);
    delete(tto_actor_02);
    delete(tto_system);
}

Test(
    TEST_SUITE_NAME,
    all_valid
) {
    void my_receive_fn_cb(lfcActor_t *self isAnUnused_param, lfcActorMessage_t *msg isAnUnused_param) {}

    lfcActorSystem_t *tto_system = lfcActorSystem_ctor("jkljkl");
    lfcActorRef_t *tto_actor_01 = lfcActorSystem_createActor(tto_system, "sumsi_01", my_receive_fn_cb);
    lfcActorRef_t *tto_actor_02 = lfcActorSystem_createActor(tto_system, "sumsi_02", my_receive_fn_cb);
    const char *msg = ";lköklö";
    lfcActorMessage_t *tto = lfcActorMessage_ctor(tto_actor_01, tto_actor_02, msg, strlen(msg));

    should_not_be_null(tto);

    delete(tto_system);
    delete(tto_actor_01);
    delete(tto_actor_02);
    delete(tto_system);
}

Test(
    TEST_SUITE_NAME,
    all_valid_noSender
) {
    void my_receive_fn_cb(lfcActor_t *self isAnUnused_param, lfcActorMessage_t *msg isAnUnused_param) {}

    lfcActorSystem_t *tto_system = lfcActorSystem_ctor("jkljkl");
    lfcActorRef_t *tto_actor_02 = lfcActorSystem_createActor(tto_system, "sumsi_02", my_receive_fn_cb);
    const char *msg = ";lköklö";
    lfcActorMessage_t *tto = lfcActorMessage_ctor(NULL, tto_actor_02, msg, strlen(msg));

    should_not_be_null(tto);

    delete(tto_system);
    delete(tto_actor_02);
    delete(tto_system);
}
