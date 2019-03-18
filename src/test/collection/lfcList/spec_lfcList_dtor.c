#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcList__lfcList_dtor

#ifdef CHECK_ALLOC_VIA_MPROBE
Test(
    TEST_SUITE_NAME,
    return_not_NIL
) {
    lfcList_t *tto = lfcList_ctor();

    delete(tto);
    should_be_freed(tto);
}
#endif // #ifdef CHECK_ALLOC_VIA_MPROBE
