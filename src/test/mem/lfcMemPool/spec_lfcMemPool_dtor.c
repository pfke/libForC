#include <mem/lfcMemPool.h>
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               lfcMemPool__lfcMemPool_dtor

#ifdef CHECK_ALLOC_VIA_MPROBE
Test(
    TEST_SUITE_NAME,
    return_not_NIL
) {
    char buf[10];
    lfcMemPool_t *tto = lfcMemPool_ctor(buf, sizeof(buf), memPool_WRITE_W_INITIALIZE);

    delete(tto);
    should_be_freed(tto);
}
#endif // #ifdef CHECK_ALLOC_VIA_MPROBE
