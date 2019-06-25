#ifndef LIBFORC_TOOLS_LOGGING_LOGGER_H_
#define LIBFORC_TOOLS_LOGGING_LOGGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lfcLogHandler.h"
#include "../../core/lfcObject.h"


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
lfcOOP_defineClass(lfcLogger, lfcObject,
    //-----------------------------------------------------------------------------
    // FIELDS
    //-----------------------------------------------------------------------------
    lfcLogHandler_t *logHandler;
    lfcLogging_loglevel_e logLevel;
    char *prefix;

    ,
    //-----------------------------------------------------------------------------
    // PUBLIC METHOD
    //-----------------------------------------------------------------------------
    int, log_EMERG_va,   (const char *format, va_list *args),
    int, log_ALERT_va,   (const char *format, va_list *args),
    int, log_CRIT_va,    (const char *format, va_list *args),
    int, log_ERR_va,     (const char *format, va_list *args),
    int, log_WARNING_va, (const char *format, va_list *args),
    int, log_NOTICE_va,  (const char *format, va_list *args),
    int, log_INFO_va,    (const char *format, va_list *args),
    int, log_DEBUG_va,   (const char *format, va_list *args),

    /**
     * Diese Methode setzt das persönliche Loglevel des Loggers.
     * Das im LogHandler bestimmt das globale Level, und mit diesem hier kann man
     * freingranularer steuern.
     */
    int, setLogLevel, (lfcLogging_loglevel_e logLevel)
    )

/**
 * Erzeugt eine lfcLogger Instanz.
 */
lfcLogger_t *lfcLogger_ctor(
    lfcLogHandler_t *logHandler,
    const char *prefix,
    ...
);

/**
 * Erzeugt eine lfcLogger Instanz.
 */
#define lfcLogger_ctor_wGlobalLogHandler(prefix, ...)       lfcLogger_ctor(lfcLogHandler_singleton(), prefix, __VA_ARGS__)

void lfcLogger_log_EMERG  (lfcLogger_t *self, const char *format, ...);
void lfcLogger_log_ALERT  (lfcLogger_t *self, const char *format, ...);
void lfcLogger_log_CRIT   (lfcLogger_t *self, const char *format, ...);
void lfcLogger_log_ERR    (lfcLogger_t *self, const char *format, ...);
void lfcLogger_log_WARNING(lfcLogger_t *self, const char *format, ...);
void lfcLogger_log_NOTICE (lfcLogger_t *self, const char *format, ...);
void lfcLogger_log_INFO   (lfcLogger_t *self, const char *format, ...);
void lfcLogger_log_DEBUG  (lfcLogger_t *self, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /*--- #ifndef LIBFORC_TOOLS_LOGGING_LOGGER_H_ ---*/
