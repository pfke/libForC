#include "collection/lfcList.h"
#include "testing/lfcCriterionHelper.h"

#define TEST_SUITE_NAME               spec_lfcCore_lfcCORE_VARGSCOUNT_TUPLE1

Test(
    TEST_SUITE_NAME,
    _0Params
) {
    should_be_same_int(0, lfcCORE_VARGSCOUNT_TUPLE1());
}

Test(
    TEST_SUITE_NAME,
    _moreParams
) {
    should_be_same_int_wText( 1, lfcCORE_VARGSCOUNT_TUPLE1(1), "1 args");
    should_be_same_int_wText( 2, lfcCORE_VARGSCOUNT_TUPLE1(1, 2), "2 args");
    should_be_same_int_wText( 3, lfcCORE_VARGSCOUNT_TUPLE1(1, 2, 3), "3 args");
    should_be_same_int_wText( 4, lfcCORE_VARGSCOUNT_TUPLE1(1, 2, 3, 4), "4 args");
    should_be_same_int_wText( 5, lfcCORE_VARGSCOUNT_TUPLE1(1, 2, 3, 4, 5), "5 args");
    should_be_same_int_wText( 6, lfcCORE_VARGSCOUNT_TUPLE1(1, 2, 3, 4, 5, 6), "6 args");
    should_be_same_int_wText( 7, lfcCORE_VARGSCOUNT_TUPLE1(1, 2, 3, 4, 5, 6, 7), "7 args");
    should_be_same_int_wText( 8, lfcCORE_VARGSCOUNT_TUPLE1(1, 2, 3, 4, 5, 6, 7, 8), "8 args");
    should_be_same_int_wText( 9, lfcCORE_VARGSCOUNT_TUPLE1(1, 2, 3, 4, 5, 6, 7, 8, 9), "9 args");
    should_be_same_int_wText(10, lfcCORE_VARGSCOUNT_TUPLE1(1, 2, 3, 4, 5, 6, 7, 8, 9, 10), "10 args");
    should_be_same_int_wText(29, lfcCORE_VARGSCOUNT_TUPLE1(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29), "29 args");
}
