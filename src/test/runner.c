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

#include "tools/logging/lfcLogger.h"
#include "tools/logging/lfcLogHandler.h"

#define ALIGN(x)    (x + (__BIGGEST_ALIGNMENT__ - x % __BIGGEST_ALIGNMENT__))
#define CALLOC_STATIC_MEM(name, size, value) \
        char name[size]; \
        memset(name, value, sizeof(name));

void runner_fn () {
    lfcLogHandler_t *logHandler = lfcLogHandler_ctor();
    lfcLogger_t *logger = lfcLogger_ctor(logHandler, "dumi_%d", 13);

    lfcLogHandler_addAppender(logHandler, lfcLogConsoleAppender_ctor("lköklö"));

    lfcLogger_ALERT(logger, "bin hier: %s", "achso");

}

int main (
    int argc __attribute__((unused)),
    char *argv[] __attribute__((unused))
) {
    runner_fn();

    return 0;
}
