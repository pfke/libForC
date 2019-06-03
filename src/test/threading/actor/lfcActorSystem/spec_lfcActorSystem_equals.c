#include "threading/actor/lfcActor.h"
#include "threading/actor/lfcActorSystem.h"
#include "testing/lfcCriterionHelper.h"
#include <unistd.h>

#define TEST_SUITE_NAME               spec_lfcActorSystem__lfcActorSystem_equals

Test(
    TEST_SUITE_NAME,
    pass_null
) {
    lfcActorSystem_t *tto_system = lfcActorSystem_ctor("jkljkl");

    should_be_false(lfcActorSystem_equals(tto_system, NULL));

    delete(tto_system);
}

Test(
    TEST_SUITE_NAME,
    passing_sameSystem
) {
    lfcActorSystem_t *tto_system = lfcActorSystem_ctor("jkljkl");

    should_be_true(lfcActorSystem_equals(tto_system, tto_system));

    delete(tto_system);
}

Test(
    TEST_SUITE_NAME,
    passing_differentSystem
) {
    lfcActorSystem_t *tto_system_01 = lfcActorSystem_ctor("jkljkl");
    lfcActorSystem_t *tto_system_02 = lfcActorSystem_ctor("jkljkl");

    should_be_false(lfcActorSystem_equals(tto_system_01, tto_system_02));

    delete(tto_system_01);
    delete(tto_system_02);
}

Test(
    TEST_SUITE_NAME,
    passing_something
) {
    lfcActorSystem_t *tto_system = lfcActorSystem_ctor("jkljkl");

    should_be_false(lfcActorSystem_equals(tto_system, (lfcActorSystem_t *) "jkljklj"));

    delete(tto_system);
}
