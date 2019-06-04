#include "threading/actor/lfcActorSystem.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcActorSystem__lfcActorSystem_isLocalActorName

Test(
    TEST_SUITE_NAME,
    valid_oneLetter
) {
    should_be_true(lfcActorSystem_isLocalActorName("z"));
}

Test(
    TEST_SUITE_NAME,
    valid_onlyLetters
) {
    should_be_true(lfcActorSystem_isLocalActorName("zsSsdlkjSdds"));
}

Test(
    TEST_SUITE_NAME,
    valid_lettersAndDigits
) {
    should_be_true(lfcActorSystem_isLocalActorName("z5464612"));
}

Test(
    TEST_SUITE_NAME,
    valid_allAllowedLiterals
) {
    should_be_true(lfcActorSystem_isLocalActorName("sUm-m_8950-si"));
}

Test(
    TEST_SUITE_NAME,
    invalid_leadingUppercase
) {
    should_be_false(lfcActorSystem_isLocalActorName("Um-m_8950-si"));
}

Test(
    TEST_SUITE_NAME,
    invalid_leadingNumber
) {
    should_be_false(lfcActorSystem_isLocalActorName("98Um-m_8950-si"));
}

Test(
    TEST_SUITE_NAME,
    invalid_leadingHyphen
) {
    should_be_false(lfcActorSystem_isLocalActorName("-m-m_8950-si"));
}

Test(
    TEST_SUITE_NAME,
    valid_endingHyphen
) {
    should_be_true(lfcActorSystem_isLocalActorName("m-m_8950-si-"));
}

Test(
    TEST_SUITE_NAME,
    invalid_interrogationMark
) {
    should_be_false(lfcActorSystem_isLocalActorName("m-m?8950-si"));
}

Test(
    TEST_SUITE_NAME,
    invalid_punctuationMark
) {
    should_be_false(lfcActorSystem_isLocalActorName("m-m.8950-si"));
}

Test(
    TEST_SUITE_NAME,
    invalid_exclamationMark
) {
    should_be_false(lfcActorSystem_isLocalActorName("m-m!8950-si"));
}

Test(
    TEST_SUITE_NAME,
    invalid_spaces
) {
    should_be_false(lfcActorSystem_isLocalActorName("m-m 8950-si"));
}
