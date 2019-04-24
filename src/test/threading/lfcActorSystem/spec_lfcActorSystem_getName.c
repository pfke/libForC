#include "threading/lfcActor.h"
#include "threading/lfcActorSystem.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcActorSystem__lfcActorSystem_getName

Test(
    TEST_SUITE_NAME,
    passing_null
) {
    should_be_null(lfcActorSystem_getName(NULL));
}

Test(
    TEST_SUITE_NAME,
    passing_valid
) {
    lfcActorSystem_t *tto = lfcActorSystem_ctor("jkljkl");

    should_be_same_str(lfcActorSystem_getName(tto), "jkljkl");

    delete(tto);
}
