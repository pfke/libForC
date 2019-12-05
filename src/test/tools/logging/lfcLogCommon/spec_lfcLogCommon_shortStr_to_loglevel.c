#include "tools/logging/lfcLogCommon.h"
#include "testing/lfcCriterionHelper.h"


#define TEST_SUITE_NAME               spec_lfcLogCommon_shortStr_to_loglevel

Test(
    TEST_SUITE_NAME,
    EMERG
) {
    should_be_same_int(lfcLogCommon_shortStr_to_loglevel("UR"), LOGLEVEL_EMERG);
}
Test(
    TEST_SUITE_NAME,
    EMERG_more
) {
    should_be_same_int(LOGLEVEL_DEBUG, lfcLogCommon_shortStr_to_loglevel("URs"));
}
Test(
    TEST_SUITE_NAME,
    EMERG_lowerCase
) {
    should_be_same_int(lfcLogCommon_shortStr_to_loglevel("ur"), LOGLEVEL_EMERG);
}

Test(
    TEST_SUITE_NAME,
    ALERT
) {
    should_be_same_int(lfcLogCommon_shortStr_to_loglevel("AL"), LOGLEVEL_ALERT);
}
Test(
    TEST_SUITE_NAME,
    ALERT_more
) {
    should_be_same_int(LOGLEVEL_DEBUG, lfcLogCommon_shortStr_to_loglevel("ALs"));
}
Test(
    TEST_SUITE_NAME,
    ALERT_lowerCase
) {
    should_be_same_int(lfcLogCommon_shortStr_to_loglevel("al"), LOGLEVEL_ALERT);
}

Test(
    TEST_SUITE_NAME,
    CRIT
) {
    should_be_same_int(lfcLogCommon_shortStr_to_loglevel("CR"), LOGLEVEL_CRIT);
}
Test(
    TEST_SUITE_NAME,
    CRIT_more
) {
    should_be_same_int(LOGLEVEL_DEBUG, lfcLogCommon_shortStr_to_loglevel("CRs"));
}
Test(
    TEST_SUITE_NAME,
    CRIT_lowerCase
) {
    should_be_same_int(lfcLogCommon_shortStr_to_loglevel("cr"), LOGLEVEL_CRIT);
}

Test(
    TEST_SUITE_NAME,
    ERR
) {
    should_be_same_int(lfcLogCommon_shortStr_to_loglevel("EE"), LOGLEVEL_ERR);
}
Test(
    TEST_SUITE_NAME,
    ERR_more
) {
    should_be_same_int(LOGLEVEL_DEBUG, lfcLogCommon_shortStr_to_loglevel("EEs"));
}
Test(
    TEST_SUITE_NAME,
    ERR_lowerCase
) {
    should_be_same_int(lfcLogCommon_shortStr_to_loglevel("ee"), LOGLEVEL_ERR);
}

Test(
    TEST_SUITE_NAME,
    WARNING
) {
    should_be_same_int(lfcLogCommon_shortStr_to_loglevel("WW"), LOGLEVEL_WARNING);
}
Test(
    TEST_SUITE_NAME,
    WARNING_more
) {
    should_be_same_int(LOGLEVEL_DEBUG, lfcLogCommon_shortStr_to_loglevel("WWs"));
}
Test(
    TEST_SUITE_NAME,
    WARNING_lowerCase
) {
    should_be_same_int(lfcLogCommon_shortStr_to_loglevel("ww"), LOGLEVEL_WARNING);
}

Test(
    TEST_SUITE_NAME,
    NOTICE
) {
    should_be_same_int(lfcLogCommon_shortStr_to_loglevel("NN"), LOGLEVEL_NOTICE);
}
Test(
    TEST_SUITE_NAME,
    NOTICE_more
) {
    should_be_same_int(LOGLEVEL_DEBUG, lfcLogCommon_shortStr_to_loglevel("NNs"));
}
Test(
    TEST_SUITE_NAME,
    NOTICE_lowerCase
) {
    should_be_same_int(lfcLogCommon_shortStr_to_loglevel("nn"), LOGLEVEL_NOTICE);
}

Test(
    TEST_SUITE_NAME,
    INFO
) {
    should_be_same_int(lfcLogCommon_shortStr_to_loglevel("II"), LOGLEVEL_INFO);
}
Test(
    TEST_SUITE_NAME,
    INFO_more
) {
    should_be_same_int(LOGLEVEL_DEBUG, lfcLogCommon_shortStr_to_loglevel("IIs"));
}
Test(
    TEST_SUITE_NAME,
    INFO_lowerCase
) {
    should_be_same_int(lfcLogCommon_shortStr_to_loglevel("ii"), LOGLEVEL_INFO);
}

Test(
    TEST_SUITE_NAME,
    DEBUG
) {
    should_be_same_int(lfcLogCommon_shortStr_to_loglevel("DD"), LOGLEVEL_DEBUG);
}
Test(
    TEST_SUITE_NAME,
    DEBUG_more
) {
    should_be_same_int(LOGLEVEL_DEBUG, lfcLogCommon_shortStr_to_loglevel("DDs"));
}
Test(
    TEST_SUITE_NAME,
    DEBUG_lowerCase
) {
    should_be_same_int(lfcLogCommon_shortStr_to_loglevel("dd"), LOGLEVEL_DEBUG);
}

