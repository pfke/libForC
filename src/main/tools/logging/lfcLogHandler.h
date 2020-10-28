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
DEFINE_CLASS(lfcLogHandler)

struct lfcLogHandler { const struct lfcObject _;
    lfcLogging_loglevel_e current_log_level;
    lfcList_t *appenders;

    /**
     * Wird genutzt die read-Funktion so lange zu blocken, bis alles geschrieben werden konnte.
     */
    pthread_mutex_t write_mutex;
};

struct lfcLogHandler_class { const struct lfcObject_class _;
    method_t addAppender;

    /**
     * Log zeuch to appenders.
     */
    method_t log;

    method_t redirectToStderr;
    method_t redirectToSyslog;

    method_t getLogLevel;
    method_t setLogLevel;
};

struct lfcLogHandler_methods {
    int (*addAppender) (lfcLogHandler_t *self, lfcLogAppender_t *logAppender);

    /**
     * Log zeuch to appenders.
     */
    int (*log) (lfcLogHandler_t *self, lfcLog_t *log);

    int (*redirectToStderr) (lfcLogHandler_t *self);
    int (*redirectToSyslog) (lfcLogHandler_t *self);

    lfcLogging_loglevel_e (*getLogLevel) (lfcLogHandler_t *self);
    int (*setLogLevel) (lfcLogHandler_t *self, lfcLogging_loglevel_e level);

    // super
    const lfcObject_methods_t *base;
};


/**
 * Gibt eine lfcLogHandler Instanz zurueck.
 */
lfcLogHandler_t *lfcLogHandler_ctor(void);
lfcLogHandler_t *lfcLogHandler_singleton(void);

int lfcLogHandler_addAppender (lfcLogHandler_t *self, lfcLogAppender_t *logAppender);

/**
 * Log zeuch to appenders.
 */
int lfcLogHandler_log (lfcLogHandler_t *self, lfcLog_t *log);

int lfcLogHandler_redirectToStderr (lfcLogHandler_t *self);
int lfcLogHandler_redirectToSyslog (lfcLogHandler_t *self);

lfcLogging_loglevel_e lfcLogHandler_getLogLevel (lfcLogHandler_t *self);
int lfcLogHandler_setLogLevel (lfcLogHandler_t *self, lfcLogging_loglevel_e level);

#ifdef __cplusplus
}
#endif

#endif /*--- #ifndef LIBFORC_TOOLS_LOGGING_LOGHANDLER_H_ ---*/
