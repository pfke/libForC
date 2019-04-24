#include "threading/lfcActor.h"
#include "threading/lfcActorSystem.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcActor__lfcActor_ctor

Test(
    TEST_SUITE_NAME,
    passing_1stArg_null
) {
    lfcActor_t *tto = lfcActor_ctor(NULL, NULL, NULL);

    should_be_null(tto);
}

Test(
    TEST_SUITE_NAME,
    passing_2ndArg_null
) {
    lfcActor_t *tto = lfcActor_ctor("kljlkjl", NULL, NULL);

    should_be_null(tto);
}

Test(
    TEST_SUITE_NAME,
    passing_3rdArg_null
) {
    lfcActor_t *tto = lfcActor_ctor("kljlkjl", NULL, NULL);

    should_be_null(tto);
}

Test(
    TEST_SUITE_NAME,
    passing_valid__returnNotNull
) {
    void my_receive_fn_cb(lfcActor_t *self, lfcActorMessage_t *msg) {}

    lfcActorSystem_t *tto_system = lfcActorSystem_ctor("jkljkl");
    lfcActor_t *tto = lfcActor_ctor("sumsi", tto_system, my_receive_fn_cb);

    should_not_be_null(tto);
    should_be_same_ptr(tto->actorSystem, tto_system);
    should_be_same_ptr(tto->receive_fn, my_receive_fn_cb);

    delete(tto);
    delete(tto_system);
}

Test(
    TEST_SUITE_NAME,
    no_spaces_in_name
) {
    void my_receive_fn_cb(lfcActor_t *self, lfcActorMessage_t *msg) {}

    lfcActorSystem_t *tto_system = lfcActorSystem_ctor("jkljkl");
    lfcActor_t *tto = lfcActor_ctor("su msi", tto_system, my_receive_fn_cb);

    should_be_null(tto);

    delete(tto_system);
}
