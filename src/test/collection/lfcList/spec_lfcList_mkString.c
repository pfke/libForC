#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcList__lfcList_mkString

Test(
    TEST_SUITE_NAME,
    call_on_empty_list
) {
    lfcList_t *tto = lfcList_ctor();

    char *result = lfcList_mkString(tto, " ");
    should_be_same_str(" ", result);

    free(result);
    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    return_null_on_illegal_sep
) {
    lfcList_t *tto = lfcList_ctor();

    should_be_null(lfcList_mkString(tto, NULL));

    delete(tto);
}

Test(
  TEST_SUITE_NAME,
  list_with_one_element
) {
  lfcList_t *tto = lfcList_ctor();
  lfcList_add(tto, "klölkös");

  char *result = lfcList_mkString(tto, "aaa");
  should_be_same_str("klölkös", result);

  free(result);
  delete(tto);
}

Test(
  TEST_SUITE_NAME,
  list_with_more_element
) {
  lfcList_t *tto = lfcList_ctor();
  lfcList_add(tto, "klölkös");
  lfcList_add(tto, "cccc");
  lfcList_add(tto, "ddd");

  char *result = lfcList_mkString(tto, "aaa");
  should_be_same_str("klölkösaaaccccaaaddd", result);

  free(result);
  delete(tto);
}

