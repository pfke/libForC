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
#include "tools/logging/lfcLogHandler.h"

#define ALIGN(x)    (x + (__BIGGEST_ALIGNMENT__ - x % __BIGGEST_ALIGNMENT__))
#define CALLOC_STATIC_MEM(name, size, value) \
        char name[size]; \
        memset(name, value, sizeof(name));

void runner_fn () {
    lfcLogHandler_t *logHandler = lfcLogHandler_ctor();
    lfcLogger_t *logger = lfcLogger_ctor(logHandler, "dumi_%d", 13);

    lfcLogHandler_addAppender(logHandler, asInstanceOf(lfcLogAppender(), lfcLogConsoleAppender_ctor("%c%D{%F %T} [%l] %p --- [%t->%T] --- %F:%f : %m%C")));
    lfcLogHandler_setLogLevel(logHandler, LOGLEVEL_DEBUG);

    lfcLogger_EMERG(logger, "bin hier: %s", "achso");
    lfcLogger_ALERT(logger, "bin hier: %s", "achso");
    lfcLogger_CRIT(logger, "bin hier: %s", "achso");
    lfcLogger_ERR(logger, "err 5");
    lfcLogger_WARNING(logger, "debug 1");
    lfcLogger_NOTICE(logger, "debug 1");
    lfcLogger_INFO(logger, "debug 1");
    lfcLogger_DEBUG(logger, "debug 2");

//    lfcLog_t *log = lfcLog_ctor(
//        123456789,
//        LOGLEVEL_DEBUG,
//        "myLogger",
//        "method1",
//        13,
//        1325,
//        "simple message"
//    );
//
//    char *result = lfcLogFormatter_formatAsString(
//        "%T{%A %d %H:%M:%S:}",
//        log
//    );
//    fprintf(stderr, "%s@%d: '%s'\n", __func__,  __LINE__, result);

//    should_be_same_str("simple message", result);

//    free(result);
//    delete(log);
}

int main (
    int argc __attribute__((unused)),
    char *argv[] __attribute__((unused))
) {
    runner_fn();

    return 0;
}
