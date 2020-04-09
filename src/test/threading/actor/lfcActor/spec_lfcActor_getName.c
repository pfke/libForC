#include "threading/actor/lfcActor.h"
#include "threading/actor/lfcActorSystem.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcActor__lfcActor_getName

Test(
    TEST_SUITE_NAME,
    passing_null
) {
    should_be_null(lfcActor_getName(NULL));
}

Test(
    TEST_SUITE_NAME,
    passing_valid
) {
    void my_receive_fn_cb(lfcActor_t *self isAnUnused_param, lfcActorMessage_t *msg isAnUnused_param) {}

    lfcActorSystem_t *system = lfcActorSystem_ctor("jkljkl");
    lfcActorRef_t *tto_ref = lfcActorSystem_createActor(system, "actor_01", my_receive_fn_cb);
    lfcActor_t *tto = asInstanceOf(lfcActor(), tto_ref);

    should_be_same_str(lfcActor_getName(tto), "actor_01");

    delete(system);
    delete(tto);
}
