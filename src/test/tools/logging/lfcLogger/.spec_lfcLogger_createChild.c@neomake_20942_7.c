#include "tools/logging/lfcLogger.h"
#include "tools/logging/lfcLoggerHandler.h"
#include "testing/lfcCriterionHelper.h"
#include "tools/logging/lfcLog.h"

#include <sys/prctl.h>


#define TEST_SUITE_NAME               spec_lfcLogger_createChild

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
