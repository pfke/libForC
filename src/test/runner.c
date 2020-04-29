#include <stdio.h>
#include <tools/lfcOptParser.h>
#include <data/lfcStringOps.h>
#include <malloc.h>
#include <mem/lfcMemPool.h>
#include <net/lfcSocket.h>
#include <net/lfcSocketHandler.h>
#include <net/lfcSocketJobReaderStream.h>
#include <threading/lfcThreadPool.h>
#include <threading/actor/lfcActorSystem.h>
#include <net/lfcSocketJobAcceptConn.h>

#include "tools/logging/lfcLogConsoleAppender.h"
#include "tools/logging/lfcLogger.h"
#include "tools/logging/lfcLog.h"
#include "tools/logging/lfcLogFormatter.h"
#include "tools/logging/lfcLogHandler.h"

#include "io/lfcMXMLParser.h"

void runner_fn () {
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


    fprintf(stdout, "%s:%d '%s'\n", __func__, __LINE__, result);


    free(result);
    delete(log);
}

int main (
    int argc __attribute__((unused)),
    char *argv[] __attribute__((unused))
) {
    runner_fn();

    return 0;
}
