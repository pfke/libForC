#include "threading/actor/lfcActorSystem.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcActorSystem__lfcActorSystem_isRemoteActorName_viaMQ

Test(
    TEST_SUITE_NAME,
    invalid_validLocalName_oneLetter
) {
    should_be_false(lfcActorSystem_isRemoteActorName_viaMQ("z"));
}

Test(
    TEST_SUITE_NAME,
    invalid_validLocalName_onlyLetters
) {
    should_be_false(lfcActorSystem_isRemoteActorName_viaMQ("zsSsdlkjSdds"));
}

Test(
    TEST_SUITE_NAME,
    invalid_validLocalName_lettersAndDigits
) {
    should_be_false(lfcActorSystem_isRemoteActorName_viaMQ("z5464612"));
}

Test(
    TEST_SUITE_NAME,
    invalid_validLocalName
) {
    should_be_false(lfcActorSystem_isRemoteActorName_viaMQ("sUm-m_8950-si"));
}

Test(
    TEST_SUITE_NAME,
    invalid_validLocalName_endingHyphen
) {
    should_be_false(lfcActorSystem_isRemoteActorName_viaMQ("m-m_8950-si-"));
}

Test(
    TEST_SUITE_NAME,
    invalid_validRemoteTCPName
) {
    should_be_false(lfcActorSystem_isRemoteActorName_viaMQ("lfcAA.tcp://m-m_8950-si@192.168.178.1:1231"));
}

Test(
    TEST_SUITE_NAME,
    valid
) {
    should_be_true(lfcActorSystem_isRemoteActorName_viaMQ("lfcAA.mq://m-m_8950-si@saddsa:asdasddsa:das23-s_s"));
}
