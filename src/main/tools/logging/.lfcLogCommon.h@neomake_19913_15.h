#ifndef LIBFORC_TOOLS_LOGGING_LOGCOMMON_H_
#define LIBFORC_TOOLS_LOGGING_LOGCOMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../../core/lfcObject.h"
#include <stdlib.h>
#include <syslog.h>


/*--------------------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------------------*/
DEFINE_CLASS(lfcLogAppender)

struct lfcLogAppender { const struct lfcObject _;
};

struct lfcLogAppender_class { const struct lfcObject_class _;
    method_t print;
};

struct lfcLogAppender_methods {
    int (*print)(lfcLogAppender_t *self, lfcLog_t *log);

    // super
    const lfcObject_methods_t *base;
};
typedef enum lfcLogging_loglevel {
    LOGLEVEL_EMERG      = LOG_EMERG,   // system is unusable
    LOGLEVEL_ALERT      = LOG_ALERT,   // action must be taken immediately
    LOGLEVEL_CRIT       = LOG_CRIT,    // critical conditions
    LOGLEVEL_ERR        = LOG_ERR,     // error conditions
    LOGLEVEL_WARNING    = LOG_WARNING, // warning conditions
    LOGLEVEL_NOTICE     = LOG_NOTICE,  // normal, but significant, condition
    LOGLEVEL_INFO       = LOG_INFO,    // informational message
    LOGLEVEL_DEBUG      = LOG_DEBUG,
} lfcLogging_loglevel_e;

const char *lfcLogCommon_logLevel_to_longStr (lfcLogging_loglevel_e log_level);
const char *lfcLogCommon_logLevel_to_shortStr (lfcLogging_loglevel_e log_level);

const char *lfcLogCommon_logLevel_to_ansiEscCode (lfcLogging_loglevel_e log_level);
const char *lfcLogCommon_logLevel_reset_ansiEscCode (void);

lfcLogging_loglevel_e lfcLogCommon_longStr_to_loglevel (const char *in);
lfcLogging_loglevel_e lfcLogCommon_shortStr_to_loglevel (const char *in);

typedef struct lfcLog lfcLog_t;


#ifdef __cplusplus
}
#endif

#endif /*--- #ifndef LIBFORC_TOOLS_LOGGING_LOGCOMMON_H_ ---*/
