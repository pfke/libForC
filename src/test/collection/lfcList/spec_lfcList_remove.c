#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcList__lfcList_remove

Test(
    TEST_SUITE_NAME,
    remove_null
) {
    lfcList_t *tto = lfcList_ctor();

    should_be_same_int_wText(lfcList_methods()->remove(tto, NULL), -1, "beim remove(.., NULL) soll ein Fehler zurückgegeben werden");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_return_on_existing
) {
    lfcList_t *tto = lfcList_ctor();
    lfcList_add(tto, "klölkö");

    should_be_same_int_wText(lfcList_remove(tto, "klölkö"), 0, "Methode soll 0 zurueckgeben, wenn das Element in der Liste war");

    delete(tto);
}

Test(
  TEST_SUITE_NAME,
  check_return_on_nonExisting
) {
  lfcList_t *tto = lfcList_ctor();
  lfcList_add(tto, "klölkös");

  should_be_same_int_wText(lfcList_remove(tto, "klölkö"), -1, "Methode soll -1 zurueckgeben, wenn das Element nicht in der Liste war");

  delete(tto);
}
