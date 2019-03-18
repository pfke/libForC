#ifndef LIBFORC_TOOLS_LOGINSTANCE_H_
#define LIBFORC_TOOLS_LOGINSTANCE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "core/lfcObject.h"
#include <pthread.h>
#include <syslog.h>


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
DEFINE_CLASS(lfcLogInstance);


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
typedef enum lfcLogInstance_loglevel {
    LOGLEVEL_EMERG      = LOG_EMERG,   // system is unusable
    LOGLEVEL_ALERT      = LOG_ALERT,   // action must be taken immediately
    LOGLEVEL_CRIT       = LOG_CRIT,    // critical conditions
    LOGLEVEL_ERR        = LOG_ERR,     // error conditions
    LOGLEVEL_WARNING    = LOG_WARNING, // warning conditions
    LOGLEVEL_NOTICE     = LOG_NOTICE,  // normal, but significant, condition
    LOGLEVEL_INFO       = LOG_INFO,    // informational message
    LOGLEVEL_DEBUG      = LOG_DEBUG,
} lfcLogInstance_loglevel_e;

typedef enum lfcLogInstance_redirectTo {
    LOG_REDIRECTTO_STDERR,
    LOG_REDIRECTTO_SYSLOG,
} lfcLogInstance_redirectTo_e;

/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
struct lfcLogInstance { const struct lfcObject _;
    lfcLogInstance_loglevel_e current_log_level;
    lfcLogInstance_redirectTo_e redirectTo;
};

struct lfcLogInstance_class { const struct lfcObject_class _;
    method_t log;

    method_t redirectToStderr;
    method_t redirectToSyslog;

    method_t setDebugLevel;
};

struct lfcLogInstance_methods {
    int (*log)(lfcLogInstance_t *self, lfcLogInstance_loglevel_e log_level, const char *prefix, const char *format, va_list *args);

    int (*redirectToStderr)(lfcLogInstance_t *self);
    int (*redirectToSyslog)(lfcLogInstance_t *self);

    int (*setDebugLevel)(lfcLogInstance_t *self, lfcLogInstance_loglevel_e level);

    // super
    const lfcObject_methods_t *base;
};

/**
 * Gibt eine lfcLogInstance Instanz zurueck.
 */
const lfcLogInstance_t *lfcLogInstance_singleton(void);

int lfcLogInstance_log (
    lfcLogInstance_t *self,
    lfcLogInstance_loglevel_e log_level,
    const char *prefix,
    const char *format,
    va_list *args
);

int lfcLogInstance_redirectToStderr(lfcLogInstance_t *self);
int lfcLogInstance_redirectToSyslog(lfcLogInstance_t *self);

int lfcLogInstance_setDebugLevel(lfcLogInstance_t *self, lfcLogInstance_loglevel_e level);

#ifdef __cplusplus
}
#endif

#endif /*--- #ifndef LIBFORC_TOOLS_LOGINSTANCE_H_ ---*/
