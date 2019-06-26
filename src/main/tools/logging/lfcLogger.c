#include "lfcLogger.h"

#include <stdlib.h>
#include <time.h>
#include "lfcLog.h"


/******************************************************************************************/
/* INITIALIZATION                                                                         */
/******************************************************************************************/

lfcOOP_implementClass(lfcLogger, lfcObject,
    int, log_va,         (lfcLogging_loglevel_e, const char *, int, const char *, va_list *),
    int, log_EMERG_va,   (const char *, int, const char *, va_list *),
    int, log_ALERT_va,   (const char *, int, const char *, va_list *),
    int, log_CRIT_va,    (const char *, int, const char *, va_list *),
    int, log_ERR_va,     (const char *, int, const char *, va_list *),
    int, log_WARNING_va, (const char *, int, const char *, va_list *),
    int, log_NOTICE_va,  (const char *, int, const char *, va_list *),
    int, log_INFO_va,    (const char *, int, const char *, va_list *),
    int, log_DEBUG_va,   (const char *, int, const char *, va_list *),

    int, setLogLevel, (lfcLogging_loglevel_e)
)

/******************************************************************************************/
/* TYPES                                                                                  */
/******************************************************************************************/


/******************************************************************************************/
/* FIELDS                                                                                 */
/******************************************************************************************/


/******************************************************************************************/
/* PROTOTYPES                                                                             */
/******************************************************************************************/


/******************************************************************************************/
/* PRIVATE METHODS                                                                        */
/******************************************************************************************/


/******************************************************************************************/
/* PUBLIC METHODS                                                                         */
/******************************************************************************************/

/**
 * Diese Funktion erstellt eine Instanz.
 */
static lfcLogger_t *public_lfcLogger_ctor (
    void *_self,
    va_list *app
) {
    lfcLogger_t *self = (lfcLogger_t *) lfcObject_super_ctor(lfcLogger(), _self);

    ASSERT_PTR(self, err, "could not create instance");

    // read args
    self->logLevel = LOGLEVEL_EMERG; // erstmal alles durchlassen
    self->logHandler = va_arg(*app, lfcLogHandler_t *);
    const char *prefix = va_arg(*app, const char *);
    if (!prefix) { goto err; }
    self->prefix = strdup(prefix);

    return self;

err:
    if (self->prefix) free(self->prefix);

    return NULL;
}

/**
 * Destructor-Funktion.
 */
static lfcLogger_t *public_lfcLogger_dtor (
    lfcLogger_t *self
) {
    if (self->prefix) free(self->prefix);

    return lfcObject_super_dtor(lfcLogger(), self);
}

static int public_lfcLogger_log_va   (
    lfcLogger_t *self,
    lfcLogging_loglevel_e logLevel,
    const char *method, int methodLine,
    const char *format,
    va_list *args
) {
    // Ist speziell dieser Logger eingeschränkt?
    if (self->logLevel > logLevel) { return 1; }

    char message[100];

    vsnprintf(message, sizeof(message), format, *args);

    return lfcLogHandler_log(self->logHandler, lfcLog_ctor(time(NULL), logLevel, self->prefix, method, methodLine, pthread_self(), message));
}

static int public_lfcLogger_log_EMERG_va   (lfcLogger_t *self, const char *method, int methodLine, const char *format, va_list *args) { return lfcLogger_log_va(self, LOGLEVEL_EMERG,   method, methodLine, format, args); }
static int public_lfcLogger_log_ALERT_va   (lfcLogger_t *self, const char *method, int methodLine, const char *format, va_list *args) { return lfcLogger_log_va(self, LOGLEVEL_ALERT,   method, methodLine, format, args); }
static int public_lfcLogger_log_CRIT_va    (lfcLogger_t *self, const char *method, int methodLine, const char *format, va_list *args) { return lfcLogger_log_va(self, LOGLEVEL_CRIT,    method, methodLine, format, args); }
static int public_lfcLogger_log_ERR_va     (lfcLogger_t *self, const char *method, int methodLine, const char *format, va_list *args) { return lfcLogger_log_va(self, LOGLEVEL_ERR,     method, methodLine, format, args); }
static int public_lfcLogger_log_WARNING_va (lfcLogger_t *self, const char *method, int methodLine, const char *format, va_list *args) { return lfcLogger_log_va(self, LOGLEVEL_WARNING, method, methodLine, format, args); }
static int public_lfcLogger_log_NOTICE_va  (lfcLogger_t *self, const char *method, int methodLine, const char *format, va_list *args) { return lfcLogger_log_va(self, LOGLEVEL_NOTICE,  method, methodLine, format, args); }
static int public_lfcLogger_log_INFO_va    (lfcLogger_t *self, const char *method, int methodLine, const char *format, va_list *args) { return lfcLogger_log_va(self, LOGLEVEL_INFO,    method, methodLine, format, args); }
static int public_lfcLogger_log_DEBUG_va   (lfcLogger_t *self, const char *method, int methodLine, const char *format, va_list *args) { return lfcLogger_log_va(self, LOGLEVEL_DEBUG,   method, methodLine, format, args); }

static int public_lfcLogger_setLogLevel (
    lfcLogger_t *self,
    lfcLogging_loglevel_e logLevel
) {
    self->logLevel = logLevel;

    return !(self->logLevel == logLevel);
}


/******************************************************************************************/
/* STATIC METHODS                                                                         */
/******************************************************************************************/

/**
 * Erzeugt eine lfcLogger Instanz.
 */
lfcLogger_t *lfcLogger_ctor (
    lfcLogHandler_t *logHandler,
    const char *prefix,
    ...
) {
    char buf[100];
    va_list args;

    va_start (args, prefix);
    vsnprintf(buf, sizeof(buf), prefix, args);
    va_end (args);

    return (lfcLogger_t *)new(lfcLogger(), logHandler, buf);
}

void lfcLogger_log (
    lfcLogger_t *self,
    lfcLogging_loglevel_e logLevel,
    const char *method, int methodLine,
    const char *format,
    ...
) {
    va_list args;

    va_start (args, format);
    lfcLogger_log_va(self, logLevel, method, methodLine, format, &args);
    va_end (args);
}