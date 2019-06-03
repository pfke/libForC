#include "threading/actor/lfcActor.h"
#include "threading/actor/lfcActorSystem.h"
#include "testing/lfcCriterionHelper.h"
#include <unistd.h>

#define TEST_SUITE_NAME               spec_lfcActorSystem__lfcActorSystem_equals_byActorRef

Test(
    TEST_SUITE_NAME,
    pass_null
) {
    lfcActorSystem_t *tto_system = lfcActorSystem_ctor("jkljkl");

    should_be_false(lfcActorSystem_equals_byActorRef(tto_system, NULL));

    delete(tto_system);
}

Test(
    TEST_SUITE_NAME,
    passing_sameSystem
) {
    void my_receive_fn_cb_actor(lfcActor_t *self, lfcActorMessage_t *msg) {}

    lfcActorSystem_t *tto_system = lfcActorSystem_ctor("jkljkl");

    lfcActorRef_t *tto_actor = lfcActorSystem_create(tto_system, "actor_01", my_receive_fn_cb_actor);

    should_be_true(lfcActorSystem_equals_byActorRef(tto_system, tto_actor));

    delete(tto_system);
}

Test(
    TEST_SUITE_NAME,
    passing_differentSystem
) {
    void my_receive_fn_cb_actor(lfcActor_t *self, lfcActorMessage_t *msg) {}

    lfcActorSystem_t *tto_system_01 = lfcActorSystem_ctor("jkljkl");
    lfcActorSystem_t *tto_system_02 = lfcActorSystem_ctor("jkljkl");
    lfcActorRef_t *tto_actor = lfcActorSystem_create(tto_system_02, "actor_01", my_receive_fn_cb_actor);

    should_be_false(lfcActorSystem_equals_byActorRef(tto_system_01, tto_actor));

    delete(tto_system_01);
    delete(tto_system_02);
}

Test(
    TEST_SUITE_NAME,
    passing_something
) {
    lfcActorSystem_t *tto_system = lfcActorSystem_ctor("jkljkl");

    should_be_false(lfcActorSystem_equals_byActorRef(tto_system, (lfcActorRef_t *) "jkljklj"));

    delete(tto_system);
}
