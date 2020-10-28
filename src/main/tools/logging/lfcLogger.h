#ifndef LIBFORC_TOOLS_LOGGING_LOGGER_H_
#define LIBFORC_TOOLS_LOGGING_LOGGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lfcLogHandler.h"
#include "lfcLogCommon.h"
#include "../../core/lfcObject.h"


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
DEFINE_CLASS(lfcLogger)

struct lfcLogger { const struct lfcObject _;
    lfcLogHandler_t *logHandler;
    lfcLogging_loglevel_e logLevel;
    char *prefix;
};

struct lfcLogger_class { const struct lfcObject_class _;
    method_t createChild;

    method_t getPrefix;

    method_t log_va;

    method_t log_EMERG_va;
    method_t log_ALERT_va;
    method_t log_CRIT_va;
    method_t log_ERR_va;
    method_t log_WARNING_va;
    method_t log_NOTICE_va;
    method_t log_INFO_va;
    method_t log_DEBUG_va;

    method_t setLogLevel;
};

struct lfcLogger_methods {
    lfcLogger_t * (*createChild) (lfcLogger_t *self, const char *prefix);

    const char * (*getPrefix) (lfcLogger_t *self);

    int (*log_va) (lfcLogger_t *self, lfcLogging_loglevel_e logLevel, const char* method, int methodLine, const char *format, va_list *args);

    int (*log_EMERG_va)  (lfcLogger_t *self, const char* method, int methodLine, const char *format, va_list *args);
    int (*log_ALERT_va)  (lfcLogger_t *self, const char* method, int methodLine, const char *format, va_list *args);
    int (*log_CRIT_va)   (lfcLogger_t *self, const char* method, int methodLine, const char *format, va_list *args);
    int (*log_ERR_va)    (lfcLogger_t *self, const char* method, int methodLine, const char *format, va_list *args);
    int (*log_WARNING_va)(lfcLogger_t *self, const char* method, int methodLine, const char *format, va_list *args);
    int (*log_NOTICE_va) (lfcLogger_t *self, const char* method, int methodLine, const char *format, va_list *args);
    int (*log_INFO_va)   (lfcLogger_t *self, const char* method, int methodLine, const char *format, va_list *args);
    int (*log_DEBUG_va)  (lfcLogger_t *self, const char* method, int methodLine, const char *format, va_list *args);

    /**
     * Diese Methode setzt das persönliche Loglevel des Loggers.
     * Das im LogHandler bestimmt das globale Level, und mit diesem hier kann man
     * freingranularer steuern.
     */
    int (*setLogLevel) (lfcLogger_t *self, lfcLogging_loglevel_e logLevel);

    // super
    const lfcObject_methods_t *base;
};

/**
 * Erzeugt eine lfcLogger Instanz.
 */
lfcLogger_t *lfcLogger_ctor(
    lfcLogHandler_t *logHandler,
    const char *prefix,
    ...
);

lfcLogger_t * lfcLogger_createChild (lfcLogger_t *self, const char *prefix);

const char * lfcLogger_getPrefix (lfcLogger_t *self);

int lfcLogger_log_va (lfcLogger_t *self, lfcLogging_loglevel_e logLevel, const char* method, int methodLine, const char *format, va_list *args);

int lfcLogger_log_EMERG_va  (lfcLogger_t *self, const char* method, int methodLine, const char *format, va_list *args);
int lfcLogger_log_ALERT_va  (lfcLogger_t *self, const char* method, int methodLine, const char *format, va_list *args);
int lfcLogger_log_CRIT_va   (lfcLogger_t *self, const char* method, int methodLine, const char *format, va_list *args);
int lfcLogger_log_ERR_va    (lfcLogger_t *self, const char* method, int methodLine, const char *format, va_list *args);
int lfcLogger_log_WARNING_va(lfcLogger_t *self, const char* method, int methodLine, const char *format, va_list *args);
int lfcLogger_log_NOTICE_va (lfcLogger_t *self, const char* method, int methodLine, const char *format, va_list *args);
int lfcLogger_log_INFO_va   (lfcLogger_t *self, const char* method, int methodLine, const char *format, va_list *args);
int lfcLogger_log_DEBUG_va  (lfcLogger_t *self, const char* method, int methodLine, const char *format, va_list *args);

/**
 * Diese Methode setzt das persönliche Loglevel des Loggers.
 * Das im LogHandler bestimmt das globale Level, und mit diesem hier kann man
 * freingranularer steuern.
 */
int lfcLogger_setLogLevel (lfcLogger_t *self, lfcLogging_loglevel_e logLevel);

/**
 * Erzeugt eine lfcLogger Instanz.
 */
#define lfcLogger_ctor_wGlobalLogHandler(prefix, ...)       lfcLogger_ctor(lfcLogHandler_singleton(), prefix, ## __VA_ARGS__, 0)

#define lfcLogger_ALL(self, logLevel, fmt, ...)     lfcLogger_log(self, logLevel,   __func__, __LINE__, fmt, __VA_ARGS__)
#define lfcLogger_EMERG(self, fmt, ...)             lfcLogger_ALL(self, LOGLEVEL_EMERG,   fmt, ## __VA_ARGS__, 0)
#define lfcLogger_ALERT(self, fmt, ...)             lfcLogger_ALL(self, LOGLEVEL_ALERT,   fmt, ## __VA_ARGS__, 0)
#define lfcLogger_CRIT(self, fmt, ...)              lfcLogger_ALL(self, LOGLEVEL_CRIT,    fmt, ## __VA_ARGS__, 0)
#define lfcLogger_ERR(self, fmt, ...)               lfcLogger_ALL(self, LOGLEVEL_ERR,     fmt, ## __VA_ARGS__, 0)
#define lfcLogger_WARNING(self, fmt, ...)           lfcLogger_ALL(self, LOGLEVEL_WARNING, fmt, ## __VA_ARGS__, 0)
#define lfcLogger_NOTICE(self, fmt, ...)            lfcLogger_ALL(self, LOGLEVEL_NOTICE,  fmt, ## __VA_ARGS__, 0)
#define lfcLogger_INFO(self, fmt, ...)              lfcLogger_ALL(self, LOGLEVEL_INFO,    fmt, ## __VA_ARGS__, 0)
#define lfcLogger_DEBUG(self, fmt, ...)             lfcLogger_ALL(self, LOGLEVEL_DEBUG,   fmt, ## __VA_ARGS__, 0)

/**
 * Global loggers, whose does not need a separate initialized logger instance.
 */
#define lfcLogger_EMERG_g(fmt, ...)                 lfcLogger_EMERG(lfcLogger_global(), fmt, ## __VA_ARGS__, 0)
#define lfcLogger_ALERT_g(fmt, ...)                 lfcLogger_ALERT(lfcLogger_global(), fmt, ## __VA_ARGS__, 0)
#define lfcLogger_CRIT_g(fmt, ...)                  lfcLogger_CRIT(lfcLogger_global(), fmt, ## __VA_ARGS__, 0)
#define lfcLogger_ERR_g(fmt, ...)                   lfcLogger_ERR(lfcLogger_global(), fmt, ## __VA_ARGS__, 0)
#define lfcLogger_WARNING_g(fmt, ...)               lfcLogger_WARNING(lfcLogger_global(), fmt, ## __VA_ARGS__, 0)
#define lfcLogger_NOTICE_g(fmt, ...)                lfcLogger_NOTICE(lfcLogger_global(), fmt, ## __VA_ARGS__, 0)
#define lfcLogger_INFO_g(fmt, ...)                  lfcLogger_INFO(lfcLogger_global(), fmt, ## __VA_ARGS__, 0)
#define lfcLogger_DEBUG_g(fmt, ...)                 lfcLogger_DEBUG(lfcLogger_global(), fmt, ## __VA_ARGS__, 0)

/**
 * Global logger instance
 */
lfcLogger_t *lfcLogger_global(void);

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
