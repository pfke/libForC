#include "threading/lfcActor.h"
#include "threading/lfcActorSystem.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcActor__lfcActor_getReceiveFn

Test(
    TEST_SUITE_NAME,
    passing_null
) {
    should_be_null(lfcActor_getReceiveFn(NULL));
}

Test(
    TEST_SUITE_NAME,
    passing_valid
) {
    void my_receive_fn_cb(lfcActor_t *self, lfcActorMessage_t *msg) {}

    lfcActorSystem_t *system = lfcActorSystem_ctor("jkljkl");
    lfcActorRef_t *tto_ref = lfcActorSystem_create(system, "actor_01", my_receive_fn_cb);
    lfcActor_t *tto = asInstanceOf(lfcActor(), tto_ref);

    should_be_same_ptr(lfcActor_getReceiveFn(tto), my_receive_fn_cb);

    delete(system);
    delete(tto);
}
