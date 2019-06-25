#ifndef LIBFORC_TOOLS_LOGGING_LOGHANDLER_H_
#define LIBFORC_TOOLS_LOGGING_LOGHANDLER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../../core/lfcObject.h"
#include "../../collection/lfcList.h"
#include "lfcLog.h"
#include "lfcLogAppender.h"
#include "lfcLogCommon.h"
#include <pthread.h>
#include <syslog.h>


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
lfcOOP_defineClass(lfcLogHandler, lfcObject,
    //-----------------------------------------------------------------------------
    // FIELDS
    //-----------------------------------------------------------------------------
    lfcLogging_loglevel_e current_log_level;
    lfcList_t *appenders;

    /**
     * Wird genutzt die read-Funktion so lange zu blocken, bis alles geschrieben werden konnte.
     */
    pthread_mutex_t write_mutex;

    ,
    //-----------------------------------------------------------------------------
    // PUBLIC METHOD
    //-----------------------------------------------------------------------------
    int, addAppender, (lfcLogAppender_t *logAppender),

    /**
     * Log zeuch to appenders.
     */
    int, log, (lfcLog_t *log),

    int, redirectToStderr, (),
    int, redirectToSyslog, (),

    int, setLogLevel,      (lfcLogging_loglevel_e level)
)

/**
 * Gibt eine lfcLogHandler Instanz zurueck.
 */
lfcLogHandler_t *lfcLogHandler_ctor(void);
const lfcLogHandler_t *lfcLogHandler_singleton(void);


#ifdef __cplusplus
}
#endif

#endif /*--- #ifndef LIBFORC_TOOLS_LOGGING_LOGHANDLER_H_ ---*/
