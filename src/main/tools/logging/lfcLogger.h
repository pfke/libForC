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
    const char *, getPrefix, (),

    int, log_va,         (lfcLogging_loglevel_e logLevel, const char* method, int methodLine, const char *format, va_list *args),

    int, log_EMERG_va,   (const char* method, int methodLine, const char *format, va_list *args),
    int, log_ALERT_va,   (const char* method, int methodLine, const char *format, va_list *args),
    int, log_CRIT_va,    (const char* method, int methodLine, const char *format, va_list *args),
    int, log_ERR_va,     (const char* method, int methodLine, const char *format, va_list *args),
    int, log_WARNING_va, (const char* method, int methodLine, const char *format, va_list *args),
    int, log_NOTICE_va,  (const char* method, int methodLine, const char *format, va_list *args),
    int, log_INFO_va,    (const char* method, int methodLine, const char *format, va_list *args),
    int, log_DEBUG_va,   (const char* method, int methodLine, const char *format, va_list *args),

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
#define lfcLogger_ctor_wGlobalLogHandler(prefix, ...)       lfcLogger_ctor(lfcLogHandler_singleton(), prefix, ## __VA_ARGS__, 0)

#define lfcLogger_ALL(    self, logLevel, fmt, ...)   lfcLogger_log(self, logLevel,   __func__, __LINE__, fmt, __VA_ARGS__)
#define lfcLogger_EMERG(  self, fmt, ...)   lfcLogger_ALL(self, LOGLEVEL_EMERG,   fmt, ## __VA_ARGS__, 0)
#define lfcLogger_ALERT(  self, fmt, ...)   lfcLogger_ALL(self, LOGLEVEL_ALERT,   fmt, ## __VA_ARGS__, 0)
#define lfcLogger_CRIT(   self, fmt, ...)   lfcLogger_ALL(self, LOGLEVEL_CRIT,    fmt, ## __VA_ARGS__, 0)
#define lfcLogger_ERR(    self, fmt, ...)   lfcLogger_ALL(self, LOGLEVEL_ERR,     fmt, ## __VA_ARGS__, 0)
#define lfcLogger_WARNING(self, fmt, ...)   lfcLogger_ALL(self, LOGLEVEL_WARNING, fmt, ## __VA_ARGS__, 0)
#define lfcLogger_NOTICE( self, fmt, ...)   lfcLogger_ALL(self, LOGLEVEL_NOTICE,  fmt, ## __VA_ARGS__, 0)
#define lfcLogger_INFO(   self, fmt, ...)   lfcLogger_ALL(self, LOGLEVEL_INFO,    fmt, ## __VA_ARGS__, 0)
#define lfcLogger_DEBUG(  self, fmt, ...)   lfcLogger_ALL(self, LOGLEVEL_DEBUG,   fmt, ## __VA_ARGS__, 0)

/**
 * Global loggers, whose does not need a separate initialized logger instance.
 */
#define lfcLogger_EMERG_g(  fmt, ...)   lfcLogger_EMERG(lfcLogger_global(), fmt, ## __VA_ARGS__, 0)
#define lfcLogger_ALERT_g(  fmt, ...)   lfcLogger_ALERT(lfcLogger_global(), fmt, ## __VA_ARGS__, 0)
#define lfcLogger_CRIT_g(   fmt, ...)   lfcLogger_CRIT(lfcLogger_global(), fmt, ## __VA_ARGS__, 0)
#define lfcLogger_ERR_g(    fmt, ...)   lfcLogger_ERR(lfcLogger_global(), fmt, ## __VA_ARGS__, 0)
#define lfcLogger_WARNING_g(fmt, ...)   lfcLogger_WARNING(lfcLogger_global(), fmt, ## __VA_ARGS__, 0)
#define lfcLogger_NOTICE_g( fmt, ...)   lfcLogger_NOTICE(lfcLogger_global(), fmt, ## __VA_ARGS__, 0)
#define lfcLogger_INFO_g(   fmt, ...)   lfcLogger_INFO(lfcLogger_global(), fmt, ## __VA_ARGS__, 0)
#define lfcLogger_DEBUG_g(  fmt, ...)   lfcLogger_DEBUG(lfcLogger_global(), fmt, ## __VA_ARGS__, 0)

/**
 * Global logger instance
 */
lfcLogger_t *lfcLogger_global();

void lfcLogger_log  (
    lfcLogger_t *self,
    lfcLogging_loglevel_e logLevel,
    const char *method,
    int methodLine,
    const char *format,
    ...
);

#ifdef __cplusplus
}
#endif

#endif /*--- #ifndef LIBFORC_TOOLS_LOGGING_LOGGER_H_ ---*/
