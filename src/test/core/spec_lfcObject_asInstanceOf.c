#include "mocks/ClassA.h"
#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcObject_asInstanceOf

Test(
    TEST_SUITE_NAME,
    passing_valid_instance
) {
    ClassA_t *tto = (ClassA_t *)new(ClassA(), 13, "Hello world");

    should_be_true(isInstanceOf(ClassA(), asInstanceOf(ClassA(), tto)))

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    passing_other_instance
) {
    ClassA_t *tto = (ClassA_t *)new(ClassA(), 13, "Hello world");

    should_be_false(isInstanceOf(ClassA(), asInstanceOf(lfcList(), tto)))

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    passing_0ptr
) {
    should_be_null(asInstanceOf(lfcList(), NULL))
}

Test(
    TEST_SUITE_NAME,
    passing_invalid_ptr
) {
    should_be_null(asInstanceOf(lfcList(), lfcList() + 156))
}
