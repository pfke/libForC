#include "mocks/ClassA.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               Object_new

Test(
    TEST_SUITE_NAME,
    return_not_NIL,
    .description = "Test, ob die Methode überreagiert, wenn ihr ungültige Parameter übergeben werden (z.B. null-ptr)."
) {
    ClassA_t *tto = (ClassA_t *)new(ClassA(), 13, "Hello world");

    //classA_print(tto);

    should_not_be_null(tto);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    return_object_with_correct_class,
    .description = "Test, ob die Methode überreagiert, wenn ihr ungültige Parameter übergeben werden (z.B. null-ptr)."
) {
    ClassA_t *tto = (ClassA_t *)new(ClassA(), 13, "Hello world");

    should_be_same_int(isInstanceOf(ClassA(), tto), 1);

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    return_object_with_correct_super,
    .description = "Test, ob die Methode überreagiert, wenn ihr ungültige Parameter übergeben werden (z.B. null-ptr)."
) {
    ClassA_t *tto = (ClassA_t *)new(ClassA(), 13, "Hello world");

    should_be_same_int(isInstanceOf(lfcObject(), tto), 1);

    delete(tto);
}
