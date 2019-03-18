#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcList__lfcList_removeAt

Test(
    TEST_SUITE_NAME,
    remove_on_empty_list
) {
    lfcList_t *tto = lfcList_ctor();

    should_be_same_int_wText(lfcList_methods()->removeAt(tto, 14), -1, "soll den Fehlerwert -1 zurueck geben");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    existing_check_return
) {
    lfcList_t *tto = lfcList_ctor();
    lfcList_add(tto, "klölkö");

    should_be_same_int_wText(lfcList_removeAt(tto, 0), 0, "Methode soll 0 zurueckgeben, wenn das Element in der Liste war");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    existing_check_count
) {
    lfcList_t *tto = lfcList_ctor();
    lfcList_add(tto, "klölkö");
    lfcList_removeAt(tto, 0);

    should_be_same_int_wText(lfcIIterable_count(tto), 0, "Methode soll 0 zurueckgeben, wenn das Element in der Liste war");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    existing_check_isStillExist
) {
    lfcList_t *tto = lfcList_ctor();
    lfcList_add(tto, "1");
    lfcList_add(tto, "2");
    lfcList_add(tto, "3");
    lfcList_removeAt(tto, 1);

    should_be_same_int_wText(lfcIIterable_exists(tto, "2"), 0, "Methode soll 0 zurueckgeben, wenn das Element in der Liste war");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    onHugeList_check_count
) {
    lfcList_t *tto = lfcList_ctor();
    lfcList_add(tto, "1");
    lfcList_add(tto, "2");
    lfcList_add(tto, "3");
    lfcList_add(tto, "4");
    lfcList_add(tto, "5");
    lfcList_add(tto, "6");
    lfcList_removeAt(tto, 1);
    lfcList_removeAt(tto, 4);

    should_be_same_int_wText(lfcIIterable_count(tto), 4, "Should have remove 7 items");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    onHugeList_check_item_1
) {
    lfcList_t *tto = lfcList_ctor();
    lfcList_add(tto, "1");
    lfcList_add(tto, "2");
    lfcList_add(tto, "3");
    lfcList_add(tto, "4");
    lfcList_add(tto, "5");
    lfcList_add(tto, "6");
    lfcList_removeAt(tto, 1);
    lfcList_removeAt(tto, 4);

    should_be_same_ptr_wText(lfcList_methods()->getAt(tto, 0), "1", "Should return the 1. item");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    onHugeList_check_item_2
) {
    lfcList_t *tto = lfcList_ctor();
    lfcList_add(tto, "1");
    lfcList_add(tto, "2");
    lfcList_add(tto, "3");
    lfcList_add(tto, "4");
    lfcList_add(tto, "5");
    lfcList_add(tto, "6");
    lfcList_removeAt(tto, 1);
    lfcList_removeAt(tto, 4);

    should_be_same_ptr_wText(lfcList_methods()->getAt(tto, 1), "3", "Should return the 2. item");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    onHugeList_check_item_3
) {
    lfcList_t *tto = lfcList_ctor();
    lfcList_add(tto, "1");
    lfcList_add(tto, "2");
    lfcList_add(tto, "3");
    lfcList_add(tto, "4");
    lfcList_add(tto, "5");
    lfcList_add(tto, "6");

    lfcList_removeAt(tto, 1);
    lfcList_removeAt(tto, 4);

    should_be_same_ptr_wText(lfcList_methods()->getAt(tto, 2), "4", "Should return the 3. item");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    onHugeList_check_item_4
) {
    lfcList_t *tto = lfcList_ctor();
    lfcList_add(tto, "1");
    lfcList_add(tto, "2");
    lfcList_add(tto, "3");
    lfcList_add(tto, "4");
    lfcList_add(tto, "5");
    lfcList_add(tto, "6");
    lfcList_removeAt(tto, 1);
    lfcList_removeAt(tto, 4);

    should_be_same_ptr_wText(lfcList_methods()->getAt(tto, 3), "5", "Should return the 4. item");

    delete(tto);
}
