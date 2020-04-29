#include "tools/logging/lfcLogFormatter.h"
#include "testing/lfcCriterionHelper.h"
#include "tools/logging/lfcLog.h"

#include <sys/prctl.h>


#define TEST_SUITE_NAME               spec_lfcLogFormatter_formatAsString

Test(
    TEST_SUITE_NAME,
    pattern_Message
) {
    lfcLog_t *log = lfcLog_ctor(
        123456789,
        LOGLEVEL_DEBUG,
        "myLogger",
        "method1",
        13,
        1325,
        "simple message"
    );

    char *result = lfcLogFormatter_formatAsString(
        "%m",
        log
    );

    should_be_same_str("simple message", result);

    free(result);
    delete(log);
}

Test(
    TEST_SUITE_NAME,
    pattern_Level_short
) {
    lfcLog_t *log = lfcLog_ctor(
        123456789,
        LOGLEVEL_DEBUG,
        "myLogger",
        "method1",
        13,
        1325,
        "simple message"
    );

    char *result = lfcLogFormatter_formatAsString(
        "%l",
        log
    );

    should_be_same_str("DD", result);

    free(result);
    delete(log);
}

Test(
    TEST_SUITE_NAME,
    pattern_Level_long
) {
    lfcLog_t *log = lfcLog_ctor(
        123456789,
        LOGLEVEL_ALERT,
        "myLogger",
        "method1",
        13,
        1325,
        "simple message"
    );

    char *result = lfcLogFormatter_formatAsString(
        "%l:%L",
        log
    );

    should_be_same_str("AL:ALERT", result);

    free(result);
    delete(log);
}

Test(
    TEST_SUITE_NAME,
    pattern_threadId
) {
    lfcLog_t *log = lfcLog_ctor(
        123456789,
        LOGLEVEL_ALERT,
        "myLogger",
        "method1",
        13,
        1325,
        "simple message"
    );

    char *result = lfcLogFormatter_formatAsString(
        "(%t)",
        log
    );

    should_be_same_str("(1325)", result);

    free(result);
    delete(log);
}

Test(
    TEST_SUITE_NAME,
    pattern_threadName
) {
    prctl(PR_SET_NAME, "threaddy", 0, 0, 0);
    lfcLog_t *log = lfcLog_ctor(
        123456789,
        LOGLEVEL_ALERT,
        "myLogger",
        "method1",
        13,
        1325,
        "simple message"
    );

    char *result = lfcLogFormatter_formatAsString(
        "(%T)",
        log
    );

    should_be_same_str("(threaddy)", result);

    free(result);
    delete(log);
}

Test(
    TEST_SUITE_NAME,
    pattern_loggerPrefix_long
) {
    lfcLog_t *log = lfcLog_ctor(
        123456789,
        LOGLEVEL_ALERT,
        "myLogger",
        "method1",
        13,
        1325,
        "simple message"
    );

    char *result = lfcLogFormatter_formatAsString(
        "{%P}",
        log
    );

    should_be_same_str("{myLogger}", result);

    free(result);
    delete(log);
}

Test(
    TEST_SUITE_NAME,
    pattern_loggerPrefix_long_isAChild
) {
    lfcLog_t *log = lfcLog_ctor(
        123456789,
        LOGLEVEL_ALERT,
        "myLogger.stage2.andso",
        "method1",
        13,
        1325,
        "simple message"
    );

    char *result = lfcLogFormatter_formatAsString(
        "{%P}",
        log
    );

    should_be_same_str("{myLogger.stage2.andso}", result);

    free(result);
    delete(log);
}

Test(
    TEST_SUITE_NAME,
    pattern_loggerPrefix_short
) {
    lfcLog_t *log = lfcLog_ctor(
        123456789,
        LOGLEVEL_ALERT,
        "myLogger",
        "method1",
        13,
        1325,
        "simple message"
    );

    char *result = lfcLogFormatter_formatAsString(
        "{%p}",
        log
    );

    should_be_same_str("{myLogger}", result);

    free(result);
    delete(log);
}

Test(
    TEST_SUITE_NAME,
    pattern_loggerPrefix_short_isAChild
) {
    lfcLog_t *log = lfcLog_ctor(
        123456789,
        LOGLEVEL_ALERT,
        "myLogger.stage2.andso",
        "method1",
        13,
        1325,
        "simple message"
    );

    char *result = lfcLogFormatter_formatAsString(
        "{%p}",
        log
    );

    should_be_same_str("{m.s.andso}", result);

    free(result);
    delete(log);
}

Test(
    TEST_SUITE_NAME,
    pattern_loggerPrefix_short_isAChild_dotAtEnd
) {
    lfcLog_t *log = lfcLog_ctor(
        123456789,
        LOGLEVEL_ALERT,
        "myLogger.stage2.andso.",
        "method1",
        13,
        1325,
        "simple message"
    );

    char *result = lfcLogFormatter_formatAsString(
        "{%p}",
        log
    );

    should_be_same_str("{m.s.a.}", result);

    free(result);
    delete(log);
}
Test(
    TEST_SUITE_NAME,
    pattern_method
) {
    lfcLog_t *log = lfcLog_ctor(
        123456789,
        LOGLEVEL_ALERT,
        "myLogger",
        "method1",
        13,
        1325,
        "simple message"
    );

    char *result = lfcLogFormatter_formatAsString(
        "%F",
        log
    );

    should_be_same_str("method1", result);

    free(result);
    delete(log);
}

Test(
    TEST_SUITE_NAME,
    pattern_methodLine
) {
    lfcLog_t *log = lfcLog_ctor(
        123456789,
        LOGLEVEL_ALERT,
        "myLogger",
        "method1",
        13,
        1325,
        "simple message"
    );

    char *result = lfcLogFormatter_formatAsString(
        "%F@%f",
        log
    );

    should_be_same_str("method1@13", result);

    free(result);
    delete(log);
}

Test(
    TEST_SUITE_NAME,
    pattern_timeStamp
) {
    lfcLog_t *log = lfcLog_ctor(
        123456789,
        LOGLEVEL_ALERT,
        "myLogger",
        "method1",
        13,
        1325,
        "simple message"
    );

    char *result = lfcLogFormatter_formatAsString(
        "%D{%A %d %H:%M:%s:}",
        log
    );

    should_be_same_str("Thursday 29 22:33:123456789:", result);

    free(result);
    delete(log);
}

Test(
    TEST_SUITE_NAME,
    print_percent
) {
    lfcLog_t *log = lfcLog_ctor(
        123456789,
        LOGLEVEL_ALERT,
        "myLogger",
        "method1",
        13,
        1325,
        "simple message"
    );

    char *result = lfcLogFormatter_formatAsString(
        "%D{%A %d %H:%M:%s:}%%",
        log
    );

    should_be_same_str("Thursday 29 22:33:123456789:%", result);

    free(result);
    delete(log);
}

Test(
    TEST_SUITE_NAME,
    pattern_MessageMessage
) {
    lfcLog_t *log = lfcLog_ctor(
        123456789,
        LOGLEVEL_DEBUG,
        "myLogger",
        "method1",
        13,
        1325,
        "simple message"
    );

    char *result = lfcLogFormatter_formatAsString(
        "%m_:;%m",
        log
    );

    should_be_same_str("simple message_:;simple message", result);

    free(result);
    delete(log);
}
