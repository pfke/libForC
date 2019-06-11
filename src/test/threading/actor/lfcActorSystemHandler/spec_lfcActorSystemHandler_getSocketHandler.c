#include "threading/actor/lfcActorSystemHandler.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcActorSystemHandler__lfcActorSystemHandler_getSocketHandler

Test(
    TEST_SUITE_NAME,
    simpleCall
) {
    lfcActorSystemHandler_t *tto = lfcActorSystemHandler_ctor();

    should_not_be_null(lfcActorSystemHandler_getSocketHandler(tto));
    should_be_true(isInstanceOf(lfcSocketHandler(), lfcActorSystemHandler_getSocketHandler(tto)));

    delete(tto);
}
