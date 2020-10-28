#ifndef LIBFORC_TOOLS_LOGGING_LOG_H_
#define LIBFORC_TOOLS_LOGGING_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../../core/lfcObject.h"
#include "lfcLogCommon.h"
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
DEFINE_CLASS(lfcLog)

struct lfcLog { const struct lfcObject _;
    time_t timeStamp;
    lfcLogging_loglevel_e logLevel;
    char *loggerPrefix;
    char *message;

    const char *method;
    int methodLine;
    pthread_t threadId;
    char *threadName;
};

struct lfcLog_class { const struct lfcObject_class _;
    method_t getLoggerPrefix;
    method_t getLogLevel;
    method_t getMessage;
    method_t getMethod;
    method_t getMethodLine;
    method_t getTimeStamp;
    method_t getThreadId;
    method_t getThreadName;
};

struct lfcLog_methods {
    const char *          (*getLoggerPrefix)(lfcLog_t *self);
    lfcLogging_loglevel_e (*getLogLevel)    (lfcLog_t *self);
    const char *          (*getMessage)     (lfcLog_t *self);
    const char *          (*getMethod)      (lfcLog_t *self);
    int                   (*getMethodLine)  (lfcLog_t *self);
    time_t                (*getTimeStamp)   (lfcLog_t *self);
    pthread_t             (*getThreadId)    (lfcLog_t *self);
    const char *          (*getThreadName)  (lfcLog_t *self);

    // super
    const lfcObject_methods_t *base;
};

/**
 * Erzeugt eine lfcLogger Instanz.
 */
lfcLog_t *lfcLog_ctor (
    time_t timeStamp,
    lfcLogging_loglevel_e logLevel,
    const char *loggerPrefix,
    const char *method,
    int methodLine,
    pthread_t threadId,
    const char *message
);

const char *          lfcLog_getLoggerPrefix(lfcLog_t *self);
lfcLogging_loglevel_e lfcLog_getLogLevel    (lfcLog_t *self);
const char *          lfcLog_getMessage     (lfcLog_t *self);
const char *          lfcLog_getMethod      (lfcLog_t *self);
int                   lfcLog_getMethodLine  (lfcLog_t *self);
time_t                lfcLog_getTimeStamp   (lfcLog_t *self);
pthread_t             lfcLog_getThreadId    (lfcLog_t *self);
const char *          lfcLog_getThreadName  (lfcLog_t *self);

#ifdef __cplusplus
}
#endif

#endif /*--- #ifndef LIBFORC_TOOLS_LOGGING_LOG_H_ ---*/
