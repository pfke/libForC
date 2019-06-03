#include "threading/actor/lfcActorSystem.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcActorSystem__lfcActorSystem_create

Test(
    TEST_SUITE_NAME,
    passing_all_null
) {
    lfcActorSystem_t *system = lfcActorSystem_ctor("jkljkl");
    lfcActorRef_t *tto = lfcActorSystem_create(system, NULL, NULL);

    should_be_null(tto);

    delete(system);
}

Test(
    TEST_SUITE_NAME,
    passing_1st_null
) {
    lfcActorSystem_t *system = lfcActorSystem_ctor("jkljkl");
    lfcActorRef_t *tto = lfcActorSystem_create(system, "actor_01", NULL);

    should_be_null(tto);

    delete(system);
}

Test(
    TEST_SUITE_NAME,
    passing_valid__returnNotNull
) {
    void my_receive_fn_cb(lfcActor_t *self, lfcActorMessage_t *msg) {}

    lfcActorSystem_t *system = lfcActorSystem_ctor("jkljkl");
    lfcActorRef_t *tto = lfcActorSystem_create(system, "actor_01", my_receive_fn_cb);

    should_not_be_null(tto);

    delete(system);
}

Test(
    TEST_SUITE_NAME,
    passing_valid__addedToList
) {
    void my_receive_fn_cb(lfcActor_t *self, lfcActorMessage_t *msg) {}

    lfcActorSystem_t *tto_system = lfcActorSystem_ctor("jkljkl");
    lfcActorRef_t *tto_actor = lfcActorSystem_create(tto_system, "actor_01", my_receive_fn_cb);

    should_not_be_null(tto_actor);
    should_be_same_int(lfcIIterable_count(tto_system->actorList), 1);

    delete(tto_system);
    delete(tto_actor);
}

Test(
    TEST_SUITE_NAME,
    passing_valid__systemIsPassedToActor
) {
    void my_receive_fn_cb(lfcActor_t *self, lfcActorMessage_t *msg) {}

    lfcActorSystem_t *tto_system = lfcActorSystem_ctor("jkljkl");
    lfcActorRef_t *tto_actorRef = lfcActorSystem_create(tto_system, "actor_01", my_receive_fn_cb);
    lfcActor_t *tto_actor = asInstanceOf(lfcActor(), tto_actorRef);

    should_not_be_null(tto_actor);
    should_be_same_ptr(lfcActor_getActorSystem(tto_actor), tto_system);

    delete(tto_system);
    delete(tto_actor);
}
