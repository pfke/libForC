#include <asm-generic/errno-base.h>
#include "collection/lfcQueue.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcQueue__lfcQueue_add

Test(
    TEST_SUITE_NAME,
    add_null
) {
    lfcQueue_t *tto = lfcQueue_ctor();

    should_be_same_int_wText(lfcQueue_methods()->enqueue(tto, NULL), -EINVAL, "beim enqueue(.., NULL) soll ein Fehler zurückgegeben werden");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_return
) {
    lfcQueue_t *tto = lfcQueue_ctor();

    should_be_same_int_wText(lfcQueue_enqueue(tto, "klölkö"), 0, "Method returned 0 wenn es hinzugefuegt werden konnte");

    delete(tto);
}
Test(
    TEST_SUITE_NAME,
    check_count_no_increase_needed
) {
    lfcQueue_t *tto = lfcQueue_ctor();
    lfcQueue_enqueue(tto, "klölkö");

    should_be_same_int_wText(lfcQueue_methods()->lfcIIterable->count(tto), 1, "size Variable nicht auf entsprechenden Wert erhöht");

    delete(tto);
}

Test(
    TEST_SUITE_NAME,
    check_count_increase_needed
) {
    lfcQueue_t *tto = lfcQueue_ctorWithSize(1, 1);
    lfcQueue_enqueue(tto, "1");
    lfcQueue_enqueue(tto, "2");
    lfcQueue_enqueue(tto, "3");

    should_be_same_int_wText(lfcIIterable_count(tto), 3, "interner array zaehler wurde nicht auf korrekten Wert gesetzt");

    delete(tto);
}
