#include "threading/actor/lfcActorSystem.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcActorSystem__lfcActorSystem_isRemoteActorName_viaTCP

Test(
    TEST_SUITE_NAME,
    invalid_validLocalName_oneLetter
) {
    should_be_false(lfcActorSystem_isRemoteActorName_viaTCP("z"));
}

Test(
    TEST_SUITE_NAME,
    invalid_validLocalName_onlyLetters
) {
    should_be_false(lfcActorSystem_isRemoteActorName_viaTCP("zsSsdlkjSdds"));
}

Test(
    TEST_SUITE_NAME,
    invalid_validLocalName_lettersAndDigits
) {
    should_be_false(lfcActorSystem_isRemoteActorName_viaTCP("z5464612"));
}

Test(
    TEST_SUITE_NAME,
    invalid_validLocalName
) {
    should_be_false(lfcActorSystem_isRemoteActorName_viaTCP("sUm-m_8950-si"));
}

Test(
    TEST_SUITE_NAME,
    invalid_validLocalName_endingHyphen
) {
    should_be_false(lfcActorSystem_isRemoteActorName_viaTCP("m-m_8950-si-"));
}

Test(
    TEST_SUITE_NAME,
    invalid_validRemoteMQName
) {
    should_be_false(lfcActorSystem_isRemoteActorName_viaTCP("lfcAA.mq://m-m_8950-si@saddsa:asdasddsa:das23-s_s"));
}

Test(
    TEST_SUITE_NAME,
    valid
) {
    should_be_true(lfcActorSystem_isRemoteActorName_viaTCP("lfcAA.tcp://m-m_8950-si@192.168.178.1:1231"));
}
