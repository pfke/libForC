#include <asm-generic/errno-base.h>
#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcList__lfcList_add

Test(
    TEST_SUITE_NAME,
    add_null
) {
    lfcList_t *tto = lfcList_ctor();

    should_be_same_int_wText(lfcList_methods()->add(tto, NULL), -EINVAL, "beim add(.., NULL) soll ein Fehler zurückgegeben werden");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_return
) {
    lfcList_t *tto = lfcList_ctor();

    should_be_same_int_wText(lfcList_add(tto, "klölkö"), 0, "Method returned 0 wenn es hinzugefuegt werden konnte");

    delete(tto);
}
Test(
    TEST_SUITE_NAME,
    check_count_no_increase_needed
) {
    lfcList_t *tto = lfcList_ctor();
    lfcList_add(tto, "klölkö");

    should_be_same_int_wText(tto->size, 1, "size Variable nicht auf entsprechenden Wert erhöht");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_count_increase_needed
) {
    lfcList_t *tto = lfcList_ctorWithSize(1, 1);
    lfcList_add(tto, "1");
    lfcList_add(tto, "2");
    lfcList_add(tto, "3");

    should_be_same_int_wText(lfcIIterable_count(tto), 3, "interner array zaehler wurde nicht auf korrekten Wert gesetzt");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_size_increase_needed
) {
    lfcList_t *tto = lfcList_ctorWithSize(1, 5);
    lfcList_add(tto, "1");
    lfcList_add(tto, "2");
    lfcList_add(tto, "3");

    should_be_same_int_wText(tto->table_size, 6, "size Variable nicht auf entsprechenden Wert erhöht");

    delete(tto);
}

/**
 * --single=lfcList__lfcList_add::check_value_onAdd
 */
Test(
    TEST_SUITE_NAME,
    check_value_onAdd
) {
    lfcList_t *tto = lfcList_ctorWithSize(1, 5);
    lfcList_add(tto, "1");
    lfcList_add(tto, "2");
    lfcList_add(tto, "3");
    lfcList_add(tto, "33");
    lfcList_add(tto, "78");

    should_be_same_ptr_wText(lfcList_getAt(tto, 0), "1", "Value an pos 0 stimmt nicht");
    should_be_same_ptr_wText(lfcList_getAt(tto, 1), "2", "Value an pos 1 stimmt nicht");
    should_be_same_ptr_wText(lfcList_getAt(tto, 2), "3", "Value an pos 2 stimmt nicht");
    should_be_same_ptr_wText(lfcList_getAt(tto, 3), "33", "Value an pos 3 stimmt nicht");
    should_be_same_ptr_wText(lfcList_getAt(tto, 4), "78", "Value an pos 4 stimmt nicht");

    delete(tto);
}
