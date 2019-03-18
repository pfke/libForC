#include "collection/lfcQueue.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcQueue__lfcQueue_dtor

#ifdef CHECK_ALLOC_VIA_MPROBE
Test(
    TEST_SUITE_NAME,
    return_not_NIL
) {
    lfcQueue_t *tto = lfcQueue_ctor();

    delete(tto);
    should_be_freed(tto);
}
#endif // #ifdef CHECK_ALLOC_VIA_MPROBE
