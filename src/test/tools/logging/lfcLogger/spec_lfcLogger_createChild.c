#include "tools/logging/lfcLogger.h"
#include "tools/logging/lfcLogHandler.h"
#include "testing/lfcCriterionHelper.h"
#include "tools/logging/lfcLog.h"

#include <sys/prctl.h>


#define TEST_SUITE_NAME               spec_lfcLogger_createChild

Test(
    TEST_SUITE_NAME,
    pattern_Message
) {
    lfcLogHandler_t *logHandler = lfcLogHandler_ctor();
    lfcLogger_t *logger = lfcLogger_ctor(logHandler, "stage1");
    lfcLogger_t *tto = lfcLogger_createChild(logger, "dingens12");

    should_be_same_str("stage1.dingens12", lfcLogger_getPrefix(tto));

    delete(tto);
    delete(logger);
    delete(logHandler);

}
