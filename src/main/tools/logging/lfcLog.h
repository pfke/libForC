#ifndef LIBFORC_TOOLS_LOGGING_LOG_H_
#define LIBFORC_TOOLS_LOGGING_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../../core/lfcObject.h"
#include "lfcLogCommon.h"
#include <stdlib.h>


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
lfcOOP_defineClass(lfcLog, lfcObject,
    //-----------------------------------------------------------------------------
    // FIELDS
    //-----------------------------------------------------------------------------
    time_t timeStamp;
    lfcLogging_loglevel_e logLevel;
    char *loggerPrefix;
    char *message;

    const char *method;
    int methodLine;
    pthread_t threadId;
    char *threadName;

    ,
    //-----------------------------------------------------------------------------
    // PUBLIC METHOD
    //-----------------------------------------------------------------------------
    const char *,          getLoggerPrefix, (),
    lfcLogging_loglevel_e, getLogLevel, (),
    const char *,          getMessage, (),
    const char *,          getMethod, (),
    int,                   getMethodLine, (),
    time_t,                getTimeStamp, (),
    pthread_t,             getThreadId, (),
    const char *,          getThreadName, ()
    )

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

#ifdef __cplusplus
}
#endif

#endif /*--- #ifndef LIBFORC_TOOLS_LOGGING_LOG_H_ ---*/
