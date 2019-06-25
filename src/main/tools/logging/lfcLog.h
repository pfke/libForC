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
    char *txt;

    ,
    //-----------------------------------------------------------------------------
    // PUBLIC METHOD
    //-----------------------------------------------------------------------------
    lfcLogging_loglevel_e, getLogLevel, ()
    )

/**
 * Erzeugt eine lfcLogger Instanz.
 */
lfcLog_t *lfcLog_ctor (
    time_t timeStamp,
    lfcLogging_loglevel_e logLevel,
    const char *logger_prefix,
    const char *txt
);

#ifdef __cplusplus
}
#endif

#endif /*--- #ifndef LIBFORC_TOOLS_LOGGING_LOG_H_ ---*/
