#include "threading/lfcActor.h"
#include "threading/lfcActorSystem.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcActor__lfcActor_getName

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
    void my_receive_fn_cb(lfcActorSystem_t *system, lfcActor_t *self, lfcActorMessage_t *msg) {}

    lfcActorSystem_t *system = lfcActorSystem_ctor("jkljkl");
    lfcActor_t *tto = lfcActorSystem_create(system, "actor_01", my_receive_fn_cb);

    should_be_same_str(lfcActor_getName(tto), "actor_01");

    delete(system);
    delete(tto);
}
