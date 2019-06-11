#include "threading/actor/lfcActorSystemHandler.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcActorSystemHandler__lfcActorSystemHandler_ctor

Test(
    TEST_SUITE_NAME,
    simpleCall
) {
    lfcActorSystemHandler_t *tto = lfcActorSystemHandler_ctor();

    should_not_be_null(tto);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    simpleCall_checkSocketHandler
) {
    lfcActorSystemHandler_t *tto = lfcActorSystemHandler_ctor();

    should_be_null(tto->socketHandler);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    simpleCall_checkregisteredActorSystems
) {
    lfcActorSystemHandler_t *tto = lfcActorSystemHandler_ctor();

    should_be_same_int(0, lfcIIterable_count(tto->registeredActorSystems));

    delete(tto);
}
