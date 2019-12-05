#include "tools/logging/lfcLogCommon.h"
#include "testing/lfcCriterionHelper.h"


#define TEST_SUITE_NAME               spec_lfcLogCommon_longStr_to_loglevel

Test(
    TEST_SUITE_NAME,
    EMERG
) {
    should_be_same_int(lfcLogCommon_longStr_to_loglevel("EMERG"), LOGLEVEL_EMERG);
}
Test(
    TEST_SUITE_NAME,
    EMERG_more
) {
    should_be_same_int(LOGLEVEL_DEBUG, lfcLogCommon_longStr_to_loglevel("EMERGs"));
}
Test(
    TEST_SUITE_NAME,
    EMERG_lowerCase
) {
    should_be_same_int(lfcLogCommon_longStr_to_loglevel("EmErG"), LOGLEVEL_EMERG);
}

Test(
    TEST_SUITE_NAME,
    ALERT
) {
    should_be_same_int(lfcLogCommon_longStr_to_loglevel("ALERT"), LOGLEVEL_ALERT);
}
Test(
    TEST_SUITE_NAME,
    ALERT_more
) {
    should_be_same_int(LOGLEVEL_DEBUG, lfcLogCommon_longStr_to_loglevel("ALERTs"));
}
Test(
    TEST_SUITE_NAME,
    ALERT_lowerCase
) {
    should_be_same_int(lfcLogCommon_longStr_to_loglevel("alert"), LOGLEVEL_ALERT);
}

Test(
    TEST_SUITE_NAME,
    CRIT
) {
    should_be_same_int(lfcLogCommon_longStr_to_loglevel("CRIT"), LOGLEVEL_CRIT);
}
Test(
    TEST_SUITE_NAME,
    CRIT_more
) {
    should_be_same_int(LOGLEVEL_DEBUG, lfcLogCommon_longStr_to_loglevel("CRITs"));
}
Test(
    TEST_SUITE_NAME,
    CRIT_lowerCase
) {
    should_be_same_int(lfcLogCommon_longStr_to_loglevel("crit"), LOGLEVEL_CRIT);
}

Test(
    TEST_SUITE_NAME,
    ERR
) {
    should_be_same_int(lfcLogCommon_longStr_to_loglevel("ERR"), LOGLEVEL_ERR);
}
Test(
    TEST_SUITE_NAME,
    ERR_more
) {
    should_be_same_int(LOGLEVEL_DEBUG, lfcLogCommon_longStr_to_loglevel("ERRs"));
}
Test(
    TEST_SUITE_NAME,
    ERR_lowerCase
) {
    should_be_same_int(lfcLogCommon_longStr_to_loglevel("erR"), LOGLEVEL_ERR);
}

Test(
    TEST_SUITE_NAME,
    WARNING
) {
    should_be_same_int(lfcLogCommon_longStr_to_loglevel("WARN"), LOGLEVEL_WARNING);
}
Test(
    TEST_SUITE_NAME,
    WARNING_more
) {
    should_be_same_int(LOGLEVEL_DEBUG, lfcLogCommon_longStr_to_loglevel("WARNs"));
}
Test(
    TEST_SUITE_NAME,
    WARNING_lowerCase
) {
    should_be_same_int(lfcLogCommon_longStr_to_loglevel("warn"), LOGLEVEL_WARNING);
}

Test(
    TEST_SUITE_NAME,
    NOTICE
) {
    should_be_same_int(lfcLogCommon_longStr_to_loglevel("NOTE"), LOGLEVEL_NOTICE);
}
Test(
    TEST_SUITE_NAME,
    NOTICE_more
) {
    should_be_same_int(LOGLEVEL_DEBUG, lfcLogCommon_longStr_to_loglevel("NOTEs"));
}
Test(
    TEST_SUITE_NAME,
    NOTICE_lowerCase
) {
    should_be_same_int(lfcLogCommon_longStr_to_loglevel("note"), LOGLEVEL_NOTICE);
}

Test(
    TEST_SUITE_NAME,
    INFO
) {
    should_be_same_int(lfcLogCommon_longStr_to_loglevel("INFO"), LOGLEVEL_INFO);
}
Test(
    TEST_SUITE_NAME,
    INFO_more
) {
    should_be_same_int(LOGLEVEL_DEBUG, lfcLogCommon_longStr_to_loglevel("INFOs"));
}
Test(
    TEST_SUITE_NAME,
    INFO_lowerCase
) {
    should_be_same_int(lfcLogCommon_longStr_to_loglevel("info"), LOGLEVEL_INFO);
}

Test(
    TEST_SUITE_NAME,
    DEBUG
) {
    should_be_same_int(lfcLogCommon_longStr_to_loglevel("DEBUG"), LOGLEVEL_DEBUG);
}
Test(
    TEST_SUITE_NAME,
    DEBUG_more
) {
    should_be_same_int(LOGLEVEL_DEBUG, lfcLogCommon_longStr_to_loglevel("DEBUGs"));
}
Test(
    TEST_SUITE_NAME,
    DEBUG_lowerCase
) {
    should_be_same_int(lfcLogCommon_longStr_to_loglevel("debug"), LOGLEVEL_DEBUG);
}

