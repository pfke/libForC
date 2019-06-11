#include "threading/actor/lfcActorSystemRemoteConnector.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcActorSystemRemoteConnector__lfcActorSystemRemoteConnector_ctor

Test(
    TEST_SUITE_NAME,
    _1stParam_null
) {
    lfcActorSystemRemoteConnector_t *tto = lfcActorSystemRemoteConnector_ctor(NULL);

    should_be_null(tto);
}
