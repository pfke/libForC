#include "threading/actor/lfcActorSystem.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcActorSystem__lfcActorSystem_ctor

Test(
    TEST_SUITE_NAME,
    passing_1stArg_null
) {
    lfcActorSystem_t *tto = lfcActorSystem_ctor(NULL);

    should_be_null(tto);
}

Test(
    TEST_SUITE_NAME,
    passing_valid__returnNotNull
) {
    lfcActorSystem_t *tto = lfcActorSystem_ctor("sumsi");

    should_not_be_null(tto);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    passing_valid__name_isSet
) {
    lfcActorSystem_t *tto = lfcActorSystem_ctor("sumsi");

    should_not_be_null(tto->name);
    should_be_same_str(tto->name, "sumsi");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    passing_valid__name_isNotPassedPtr
) {
    const char *inName = "jkljkl";
    lfcActorSystem_t *tto = lfcActorSystem_ctor(inName);

    should_not_be_null(tto->name);
    should_not_be_same_ptr(tto->name, inName);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    passing_valid__actorList_isInit
) {
    lfcActorSystem_t *tto = lfcActorSystem_ctor("sumsi");

    should_not_be_null(tto->actorList);
    should_be_same_int(lfcIIterable_count(tto->actorList), 0);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    passing_valid__threadPool_isInit
) {
    lfcActorSystem_t *tto = lfcActorSystem_ctor("sumsi");

    should_not_be_null(tto->threadPool);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    passing_valid__isInActorSystemHandlerList
) {
    lfcActorSystem_t *tto = lfcActorSystem_ctor("sumsi");
    lfcActorSystemHandler_t *ash = lfcActorSystemHandler_singleton();

    should_be_true(lfcIIterable_exists(lfcActorSystemHandler_getRegisteredActorSystems(ash), tto));

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    nameTesting_passing_validLocalName
) {
    lfcActorSystem_t *tto = lfcActorSystem_ctor("sUm-m_8950-si");

    should_not_be_null(tto->threadPool);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    nameTesting_passing_invalidLocalName_leadingUppercase
) {
    lfcActorSystem_t *tto = lfcActorSystem_ctor("Sumsi");

    should_be_null(tto);
}

Test(
    TEST_SUITE_NAME,
    nameTesting_passing_invalidLocalName_leadingNumber
) {
    lfcActorSystem_t *tto = lfcActorSystem_ctor("012sumsi");

    should_be_null(tto);
}

Test(
    TEST_SUITE_NAME,
    nameTesting_passing_invalidLocalName_invalidChars
) {
    lfcActorSystem_t *tto = lfcActorSystem_ctor("su?msi");

    should_be_null(tto);
}

Test(
    TEST_SUITE_NAME,
    nameTesting_passing_invalidLocalName_spaces
) {
    lfcActorSystem_t *tto = lfcActorSystem_ctor("su msi");

    should_be_null(tto);
}
