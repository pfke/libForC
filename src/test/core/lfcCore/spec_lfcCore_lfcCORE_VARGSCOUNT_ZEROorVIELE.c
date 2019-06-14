#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcCore_lfcCORE_VARGSCOUNT_ZEROorVIELE

Test(
    TEST_SUITE_NAME,
    _0Params
) {
    should_be_same_int(0, lfcCORE_VARGSCOUNT_ZEROorVIELE());
}
